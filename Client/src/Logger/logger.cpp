#include <fstream>
#include <ctime>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include "../Date/date.h"
#include "../DNSUpdater/Main/main.h"
#include "../StringPlus/string_plus.h"
#include "../FilePlus/file_plus.h"
#include "logger.h"
#include <string>
using namespace std;

string logFolder = "", logPath = "";
ofstream logFile;

void initializeLogger()
{
    logFolder = ext + "Logger" + pathSeparator + "Logs" + pathSeparator;
    logPath = logFolder + getDate() + ".log";
}

void display(string toDisplay)
{
    cout << toDisplay << endl;
}

void write(string toWrite)
{
    writeFile(logPath, "a", toWrite + "\n");
}

void print(string toPrint)
{
    string date = "[" + getDate() + "] ";
    toPrint = date + toPrint;
    write(toPrint);
    display(toPrint);
}

void exitWithMessage(string message)
{
    print(message);
    free();
}
