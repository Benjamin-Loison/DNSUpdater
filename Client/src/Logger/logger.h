#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <string>

extern std::string logFolder;
void initializeLogger(), print(std::string = ""), write(std::string), display(std::string), exitWithMessage(std::string);

#endif
