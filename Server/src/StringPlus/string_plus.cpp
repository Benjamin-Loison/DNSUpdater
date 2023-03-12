#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "../Logger/logger.h"
#include "string_plus.h"
using namespace std;

template<typename Out>
void split(const string &s, const char *delim, Out result)
{
    stringstream ss;
    ss.str(s);
    string item = "";
    while(getline(ss, item, *delim))
        *(result++) = item;
}

vector<string> split(const string &s, const char *delim)
{
    //print("!" + s + "!" +  delim + "!");
    vector<string> elems;
    split(s,
			delim,
			back_inserter(elems));
    return elems;
}

bool startsWith(string subject, string test)
{
    return !subject.compare(0, test.size(), test);
}

string replace(string subject, const string& search, const string& replace)
{
    unsigned int s = subject.find(search);
    if(s > subject.length())
        return subject;
    return subject.replace(s, search.length(), replace);
}

string toString(vector<string> vecStr)
{
    string toReturn = "";
    unsigned int vecStrSize = vecStr.size();
    for(unsigned int vecStrIndex = 0; vecStrIndex < vecStrSize; vecStrIndex++)
    {
		toReturn += vecStr[vecStrIndex];
        if(vecStrIndex != vecStrSize - 1)
            toReturn += "\n";
    }
    return toReturn;
}
