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
#include "file_plus.h"
#include "../Main/main.h"
using namespace std;

/// permissions execute file...

bool createOneDirectory(string path)
{
    if(directoryExists(path)) return true;
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
    if(directoryExists(path)) return true;
    vector<string> directories = split(path, pathSeparatorStr.c_str());
    string directory = "";
    for(unsigned int i = 0; i < directories.size(); i++)
    {
        directory = directory + directories[i] + pathSeparator;
        createOneDirectory(directory);
    }
    return false; // same
}

/// optimize with const string& for arg
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

// does several includes of the same file are done ? ie: here we include something.h and anotherSomething.h and in aniot

bool file_exists(string name) // works with directory ?
{
    struct stat buffer;
    return stat(name.c_str(), &buffer) == 0;
}

vector<string> getFileContent(string path)
{
    ifstream infile(path.c_str());
    vector<string> vec;
    string line;
    while (getline(infile, line))
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

bool writeFile(string filePath, string option, vector<string> toWrite)
{
    return writeFile(filePath, option, toString(toWrite));
}
