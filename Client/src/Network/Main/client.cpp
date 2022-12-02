#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
#elif __linux__
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/ip.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <arpa/inet.h>
    #include <string.h>
    #include <fcntl.h>
    #include <netdb.h>
    #include <errno.h>
    #define SOCKET int
    #define SOCKADDR_IN struct sockaddr_in
#endif
#include <curl/curl.h>
#include <cstdio>
#include <thread>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "../../Logger/logger.h"
#include "../../DNSUpdater/Main/main.h"
#include "../../StringPlus/string_plus.h"
#include "../../FilePlus/file_plus.h"
#include "client.h"
#include <limits>
#define NETWORK_LENGTH 1000000
#define PASSWORD "A_PASSWORD"
#define WEBSITE_IP "https://ifconfig.me"
#define MS_TO_WAIT 60000
// 1000 * 60: every minute
#define US_TO_WAIT 60000000
#define DNS_IP "dns.lemnoslife.com"
// or domain
#define SERVER_PORT 57563
// first digits of CRC32 of DNSUpdater ;)
using namespace std;

void initializeNetwork(), checkIPChange(), sendIP(), closeConnection();
SOCKET socketId;
SOCKADDR_IN destination;
char buffere[NETWORK_LENGTH] = "";
vector<string> msg;
string lastIP = "", lastIPFilePath = "";
bool connection();

size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string*)userp)->append((char*)contents, size *nmemb);
    return size *nmemb;
}

string getIP()
{
    CURL *curl = curl_easy_init();
    string got;
    curl_easy_setopt(curl, CURLOPT_URL, WEBSITE_IP);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &got);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return got;
}

void initializeNetwork()
{
    checkIPChange();
}

bool isAnIPv4(string potentialIP)
{
    vector<string> parts = split(potentialIP, ".");
    unsigned short partsSize = parts.size();
    if(partsSize != 4) return false;
    for(unsigned short partsIndex = 0; partsIndex < partsSize; partsIndex++)
    {
        string part = parts[partsIndex];
        if(!isAnInteger(part)) return false;
        long partNumber = convertStrToLong(part);
        if(partNumber < 0 || partNumber > 255) return false;
    }
    return true;
}

void writeIP()
{
    sendStr(PASSWORD); // could add OTP verification
    writeFile(lastIPFilePath, "w", lastIP);
}

void checkIPChange()
{
    lastIPFilePath = ext + "lastIP.txt";
    if(file_exists(lastIPFilePath)) // assume no user changes inside the first line of the file
    {
        vector<string> lines = getFileContent(lastIPFilePath); // used to crash when forcing [0]
        unsigned short linesSize = lines.size();
        if(linesSize >= 1)
            lastIP = lines[0];
        else
        {
            print("lastIPFilePath (" + lastIPFilePath + ") hasn't any line !");
            lastIP = "";
        }
    }
    else
        lastIP = "";
    while(true)
    {
        string currentIP = getIP();
        if(currentIP != lastIP)
        {
            lastIP = currentIP;
            sendIP();
        }
        #ifdef _WIN32
            Sleep(MS_TO_WAIT);
        #elif __linux__
            usleep(US_TO_WAIT);
        #endif
    }
}

void sendIP()
{
    #ifdef _WIN32
        WSADATA initWin32;
        WSAStartup(MAKEWORD(2, 2), &initWin32);
    #endif
    socketId = socket(AF_INET, SOCK_STREAM, 0);
    destination.sin_family = AF_INET;
    string serverAddress = DNS_IP;
    if(isAnIPv4(serverAddress))
    {
        print("Consider server address as an ipv4");
        destination.sin_addr.s_addr = inet_addr(serverAddress.c_str());
    }
    else
    {
        print("Consider server address as a domain name");
        destination.sin_addr.s_addr = *(u_long*)gethostbyname(serverAddress.c_str())->h_addr_list[0];
    }

    destination.sin_port = htons(SERVER_PORT);
    if(connection())
    {
        print("Network is loaded !");
        writeIP();
    }
    else
    {
        exitWithMessage("Server not connected");
    }
    closeConnection();

}

void sendStr(string str)
{
    str += "#";
    print("Send: " + str + " !");
    strcpy(buffere, str.c_str());
    send(socketId, buffere, strlen(buffere), 0);
}

bool connection()
{
    return connect(socketId, (struct sockaddr*)&destination, sizeof(destination)) == 0;
}

void closeConnection()
{
    shutdown(socketId, 2);
    #ifdef _WIN32
        closesocket(socketId);
        WSACleanup();
    #elif __linux__
        close(socketId);
    #endif
}

void freeNetwork()
{
    closeConnection();
}
