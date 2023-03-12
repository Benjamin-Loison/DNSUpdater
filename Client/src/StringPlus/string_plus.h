#ifndef STRING_PLUS_H_INCLUDED
#define STRING_PLUS_H_INCLUDED

#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> split(std::string, std::string);
std::string replace(std::string, const std::string&, const std::string&);
int convertStrToInt(std::string);
bool isAnInteger(std::string), endsWith(const std::string&, const std::string&);
long convertStrToLong(std::string);

template<typename T>
std::string convertNbToStr(const T& number)
{
    std::ostringstream convert;
    convert << number;
    return convert.str();
}

#endif
