#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "../Logger/logger.h"
#include "string_plus.h"
using namespace std;

vector<string> split(string s, string delimiter)
{
    vector<string> toReturn;
    size_t pos = 0;
    while((pos = s.find(delimiter)) != string::npos)
    {
        toReturn.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    toReturn.push_back(s);
    return toReturn;
}

long convertStrToLong(string str)
{
    return atol(str.c_str());
}

bool endsWith(string const& value, string const& ending)
{
    if(ending.size() > value.size()) return false;
    return equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool isAnInteger(string strToTest)
{
    for(unsigned short strToTestIndex = 0; strToTestIndex < strToTest.length(); strToTestIndex++)
    {
        char chr = strToTest[strToTestIndex];
        if(chr < 48 || chr > 57)
            return false;
    }
    return strToTest != "";
}

bool isAnInteger(double nb)
{
    return ((int)nb) == nb;
}

int convertStrToInt(string str)
{
    int number;
    sscanf(str.c_str(), "%d", &number);
    return number;
}

string replace(string subject, const string& search, const string& replace)
{
    unsigned int s = subject.find(search);
    if(s > subject.length())
        return subject;
    return subject.replace(s, search.length(), replace);
}
