#ifndef FILE_PLUS_H_INCLUDED
#define FILE_PLUS_H_INCLUDED

#include <vector>
#include <string>

bool createDirectory(std::string), directoryExists(std::string), writeFile(std::string, std::string, std::vector<std::string>);
std::vector<std::string> getFileContent(std::string);

#endif

