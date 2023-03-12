#ifndef STRING_PLUS_H_INCLUDED
#define STRING_PLUS_H_INCLUDED

#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> split(const std::string&, const char *);
std::string replace(std::string, const std::string&, const std::string&), toString(std::vector<std::string>);
bool startsWith(std::string, std::string);

template<typename T>
std::string convertNbToStr(const T& number)
{
    std::ostringstream convert;
    convert << number;
    return convert.str();
}

#endif

