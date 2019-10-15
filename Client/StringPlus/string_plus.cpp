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

bool startsWith(string subject, string test)
{
    return !subject.compare(0, test.size(), test);
}

bool endsWith(string const& value, string const& ending)
{
    if(ending.size() > value.size()) return false;
    return equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool isADouble(string strToTest)
{
    strToTest = replace(strToTest, ",", ".");
    unsigned short dotsNumber = 0;
    for(unsigned short strToTestIndex = 0; strToTestIndex < strToTest.length(); strToTestIndex++)
    {
        char chr = strToTest[strToTestIndex];
        if(chr == 46)
            dotsNumber++;
        else if((chr < 48 || chr > 57) && chr != 45)
            return false;
    }
    return dotsNumber == 1;
}

bool isANaturalNonNullNumber(string strToTest)
{
    return isAnInteger(strToTest) && convertStrToInt(strToTest) > 0;
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

bool isAlphabetic(char chr)
{
    return (chr >= 65 && chr <= 90) || (chr >= 97 && chr <= 122); // return (chr >= 65 && chr <= 90) || (chr >= 90 && chr <= 123);
}

bool isAlphabetic(string str)
{
    for(unsigned short strIndex = 0; strIndex < str.length(); strIndex++)
        if(!isAlphabetic(str[strIndex])) return false;
    return true;
}

int convertStrToInt(string str)
{
    int number;
    sscanf(str.c_str(), "%d", &number);
    return number;
}

double convertStrToDouble(string str)
{
    return atof(replace(str, ",", ".").c_str());
}

string replaceAll(string str, const string& from, const string& to)
{
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

string replace(string subject, const string& search, const string& replace)
{
    unsigned int s = subject.find(search);
    if(s > subject.length())
        return subject;
    return subject.replace(s, search.length(), replace);
}

bool contains(string subject, char find)
{
    for(unsigned short subjectIndex = 0; subjectIndex < subject.length(); subjectIndex++)
        if(subject[subjectIndex] == find) return true;
    return false;
}

bool contains(string subject, string find)
{
    return subject.find(find) != string::npos;
}

string convertDoubleToStr(double nb) // this function is different from the "convertNbToString" one due to its capacity (of the following function) of returning a string ending with ".0" if the double was actually an integer
{
    if(isAnInteger(nb)) return convertNbToStr(((int)nb)) + ".0";
    return convertNbToStr(nb);
}

bool isANumber(string str)
{
    return isADouble(str) || isAnInteger(str);
}
