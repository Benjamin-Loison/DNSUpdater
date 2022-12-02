#ifdef _WIN32
    #include <windows.h>
#endif
#include <sys/stat.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "../Logger/logger.h"
#include "../StringPlus/string_plus.h"
#include "../DNSUpdater/Main/main.h"
using namespace std;

bool createOneDirectory(string path)
{
    #ifdef _WIN32
        /*if(!CreateDirectory(path, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
        {
            print("error while creating folder: " + path + " error: " + GetLastError());
            return false;
        }*/
    #elif __linux__
        if(mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
        {
            print("error while creating folder: " + path);
            return false;
        }
    #endif
    return true;
}

bool createDirectory(string path)
{
    if(endsWith(path, pathSeparatorStr))
        path = path.substr(0, path.length() - pathSeparatorStr.length());
    vector<string> directories = split(path, pathSeparatorStr.c_str());
    string directory = "";
    for(unsigned int i = 0; i < directories.size(); i++)
    {
        directory = directory + directories[i] + pathSeparator;
        createOneDirectory(directory);
    }
    return false;
}

bool directoryExists(string path)
{
    #ifdef _WIN32
        DWORD ftyp = GetFileAttributesA(path.c_str());
        if(ftyp & FILE_ATTRIBUTE_DIRECTORY)
            return true;
    #elif __linux__
        struct stat st;
        if(stat(path.c_str(), &st) == 0)
            if(st.st_mode & S_IFDIR != 0)
                return true;
    #endif
    return false;
}

bool file_exists(string name)
{
    struct stat buffer;
    return stat(name.c_str(), &buffer) == 0;
}

vector<string> getFileContent(string path)
{
    vector<string> vec;
    ifstream infile(path.c_str());
    string line;
    while(getline(infile, line))
        vec.push_back(line);
    return vec;
}

bool writeFile(string filePath, string option, string toWrite)
{
    FILE* file = fopen(filePath.c_str(), option.c_str());
    if(file != NULL)
    {
        fputs(toWrite.c_str(), file);
        fclose(file);
        return true;
    }
    return false;
}
