#include <fstream>
#include <ctime>
#include <sstream>
#include <iostream>
#include "../Date/date.h"
#include "../Main/main.h"
#include "../FilePlus/file_plus.h"
#include "../StringPlus/string_plus.h"
#include "logger.h"
#include <unistd.h>
using namespace std;

/// log everytype /
/// log with date with zero "unnecessary"

string logPath, archivePath;
ofstream logFile;

void initializeLogger()
{
    string logFolder = ext + "Logger" + pathSeparatorStr + "Logs" + pathSeparatorStr;
    createDirectory(logFolder);
    string logWithDate = logFolder + getDate();
    logPath = logWithDate + ".log";
    archivePath = logWithDate + "-archive.log";
    archive("Archive file:");
}

string iso8859_1ToUtf8(string &str)
{
    string strOut;
    for(string::iterator it = str.begin(); it != str.end(); ++it)
    {
        uint8_t ch = *it;
        if(ch < 0x80)
            strOut.push_back(ch);
        else
        {
            strOut.push_back(0xc0 | ch >> 6);
            strOut.push_back(0x80 | (ch & 0x3f));
        }
    }
    return strOut;
}

bool canPrint = true;

void print(string toPrint)
{
	toPrint = iso8859_1ToUtf8(toPrint);	
	while(!canPrint) usleep(1);
	canPrint = false;
    time_t t = time(0);
    struct tm *now = localtime(&t);
    toPrint = "[" + getDate() + "] " + toPrint;
	logFile.close();
	logFile.open(logPath, ofstream::app); // write only if return true (not very easy) OR TODO: output on cout temporarily
    logFile << toPrint << endl;
	logFile.close();
	canPrint = true;
}

void debug(string toDebug)
{
    if(DEBUG)
        print(toDebug);
}

void archive(string toArchive)
{
    print(toArchive/*, archivePath*/);
}
