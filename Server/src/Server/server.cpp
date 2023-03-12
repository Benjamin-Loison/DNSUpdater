#ifdef __linux__ 
    #include <sys/types.h> 
    #include <sys/socket.h> 
    #include <netinet/in.h> 
    #include <netinet/ip.h> 
    #include <netinet/tcp.h> 
    #include <unistd.h> 
    #include <stdlib.h> 
    #include <string.h> 
    #include <fcntl.h> 
    #include <errno.h> 
    #include <arpa/inet.h> 
    #include <netinet/in.h>
    #include <netdb.h>
    #define SOCKET int 
    #define SOCKADDR_IN struct sockaddr_in 
#endif 
#ifdef _WIN32 
    #include <winsock2.h> 
#endif 
#include <cstdio> 
#include <iostream> 
#include <thread> 
#include <vector> 
#include <string> 
#include <map> 
#include <tuple> 
#include "server.h" 
#include "../Main/main.h"
#include "../Logger/logger.h" 
#include "../StringPlus/string_plus.h"
#include "../FilePlus/file_plus.h"
#include "../Date/date.h"
#include <unistd.h>
// first digits of CRC32 "DNSUpdater"
#define SERVER_PORT 57563
#define CONNECTED 0
#define IP 1
#define PORT 2
#define NETWORK_LENGTH 1000000
#define DEBUG false
#define PASSWORD "A_PASSWORD"
#define BIND_FILE_PATH "/etc/bind/db.lemnoslife.com"
#define SUB_DNS "secretSubDomain"
// can concatenate strings in macros ?
#define BIND_RELOAD "service bind9 reload"
using namespace std; 

void connectEvent(), receive(SOCKET, string, string), closeConnection(), manageMsg(string*, SOCKET, string, string);
map<SOCKET, tuple<bool, string, unsigned short>> clients;
map<SOCKET, thread> threads; 
SOCKET socketId;
SOCKADDR_IN source; 

void initializeNetwork() 
{
    #ifdef _WIN32 
        WSADATA initWin32; 
        WSAStartup(MAKEWORD(2, 2), &initWin32); 
    #endif 
    socketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP
    #ifdef __linux__
        int enable = 1;
        if(setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
            print("setsockopt(SO_REUSEADDR) failed");
    #endif
    source.sin_family = AF_INET; 
    source.sin_addr.s_addr = INADDR_ANY; 
    source.sin_port = htons(SERVER_PORT); 
    bind(socketId, (struct sockaddr*)&source, sizeof(source)); 
    
    thread connection(&connectEvent); 
    connection.join(); // why using a thread ? 
    closeConnection(); 
} 

// could update codes (also client with ll version (more stable))
void connectEvent() 
{
    print("Server is listening on port " + convertNbToStr<int>(SERVER_PORT)); 
    int error; 
    while(true)
    { 
        error = 99; 
        while(error != 0)
        {
            print("Listen");
            error = listen(socketId, 1);
        }
        #ifdef _WIN32 
            int tempo = sizeof(source);
        #elif  __linux__ 
            socklen_t tempo = sizeof(source); 
        #endif 
        SOCKET newSocketId = accept(socketId, (struct sockaddr*)&source, &tempo), newSocket = newSocketId;
        archive("Handshake done: " + convertNbToStr<int>(newSocket));
        map<SOCKET, thread>::iterator it = threads.find(newSocket);
        if(it != threads.end())
        {
            it->second.detach();
            threads.erase(it->first);
        }
        
        map<SOCKET, tuple<bool, string, unsigned short>>::iterator ite = clients.find(newSocket);
        if(ite != clients.end())
            clients.erase(newSocket);
        clients[newSocket] = make_tuple(true, "0.0.0.0", 0);
        get<IP>(clients[newSocket]) = string(inet_ntoa(source.sin_addr));
        get<PORT>(clients[newSocket]) = source.sin_port;
        threads[newSocket] = thread(&receive, newSocket, get<IP>(clients[newSocket]), convertNbToStr(get<PORT>(clients[newSocket])));
    } 
} 

void receive(SOCKET client, string ip, string port)
{
    string msg;
    thread manageMsgs(&manageMsg, &msg, client, ip, port); 
    manageMsgs.detach();
    char bufferRcv[NETWORK_LENGTH] = {0}; 
    tuple<bool, string, unsigned short> *tuple = &clients[client]; 
    string lastMsg = "";
    while(get<CONNECTED>(*tuple)) 
    { 
        bufferRcv[NETWORK_LENGTH] = {0};
        if(recv(client, bufferRcv, NETWORK_LENGTH, 0) > 0) 
        {
            string rcv = bufferRcv;
            if(rcv.back() == '#')
            {
                msg += rcv;
                memset(bufferRcv, 0, NETWORK_LENGTH);
            }
            else if(rcv == lastMsg)
            {
                rcv = rcv.substr(0, rcv.length() - rcv.find_last_of("#"));
                msg += rcv;
                memset(bufferRcv, 0, NETWORK_LENGTH);
            }
        }
        else 
        { 
            get<CONNECTED>(*tuple) = false;
            break;
        } 
        memset(bufferRcv, 0, NETWORK_LENGTH);
        sleep(1); // otherwise 100 % CPU
    } 
    shutdown(client, 2); 
    #ifdef _WIN320 
        closesocket(client); 
    #elif __linux__ 
        close(client); 
    #endif 
    print("Disconnected !");
    debug("Thread for the client " + convertNbToStr<int>(client) + " finished");
} 

string getIPFromLine(string line)
{
    vector<string> lineParts = split(line, " ");
    unsigned short linePartsSize = lineParts.size();
    return linePartsSize > 0 ? lineParts[linePartsSize - 1] : "";
}

void manageMsg(string *msg, SOCKET client, string ip, string port) 
{
    print(ip + " " + port); 
    while(true) 
    { 
        if(*msg != "") 
        {
            string message = *msg;
            msg->assign("");
            vector<string> msgs = split(message, "#");
            archive("Received from " + ip + "@" + port + ": "/* + message*/); 
            for(unsigned int j = 0; j < msgs.size(); j++) 
            {
                string message = msgs[j];
                if(message == PASSWORD)
                {
                    print("Password matched !");
                    vector<string> lines = getFileContent(BIND_FILE_PATH);
                    string oldIP = "";
                    for(string line : lines)
                    {
                        if(startsWith(line, SUB_DNS))
                        {
                            oldIP = getIPFromLine(line);
                            break;
                        }
                    }
                    bool same = ip == oldIP;
                    if(!same)
                    {
                        for(unsigned short linesIndex = 0; linesIndex < lines.size(); linesIndex++)
                        {
                            string line = lines[linesIndex];
                            if(getIPFromLine(line) == oldIP)
                            {
                                string newLine = replace(line, oldIP, ip);
                                if(lines[linesIndex] != newLine)
                                    lines[linesIndex] = newLine;
                            }
                        }
                        print("Changing IP from " + oldIP + " to: " + ip);
                        writeFile(BIND_FILE_PATH, "w", lines);
                        system(BIND_RELOAD); // need to run this code with the associated permissions
                    }
                    else
                    {
                        print("Not changing IP (same) to: " + ip);
                    }
                }
            } 
        } 
        else if(get<CONNECTED>(clients[client]) == false) 
            break; 
    } 
} 

void closeConnection() 
{ 
    debug("closeConnection"); 
    for(map<SOCKET, tuple<bool, string, unsigned short>>::iterator it = clients.begin(); it != clients.end(); it++) 
    { 
        SOCKET client = it->first; 
        shutdown(client, 2); 
        #ifdef _WIN32 
            closesocket(client); 
        #elif __linux__ 
            close(client); 
        #endif 
    } 
    #ifdef _WIN32 
        closesocket(socketId); 
        WSACleanup(); 
    #elif __linux__ 
        close(socketId); 
    #endif 
} 

void freeNetwork() 
{ 
    debug("freeNetwork"); 
    closeConnection(); 
} 
