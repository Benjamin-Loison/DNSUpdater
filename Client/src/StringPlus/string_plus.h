#ifndef STRING_PLUS_H_INCLUDED
#define STRING_PLUS_H_INCLUDED

#include <vector>
#include <string>
#include <sstream>
#include <map>

std::vector<std::string> split(std::string, std::string);
std::string replace(std::string, const std::string&, const std::string&), replaceAll(std::string, const std::string&, const std::string&),
            convertDoubleToStr(double);
int convertStrToInt(std::string);
double convertStrToDouble(std::string);
bool isANumber(std::string), isAnInteger(std::string), isAnInteger(double), startsWith(std::string, std::string), endsWith(const std::string&, const std::string&),
     contains(std::string, std::string), contains(std::string, char);
long convertStrToLong(std::string);


template<typename T>
std::string convertNbToStr(const T& number)
{
    std::ostringstream convert;
    convert << number;
    return convert.str();
}

#endif
