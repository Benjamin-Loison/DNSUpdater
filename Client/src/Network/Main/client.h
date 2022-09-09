#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include <thread>
#include <string>

extern std::thread network;
void initializeNetwork(), sendStr(std::string), closeConnection(), freeNetwork();

#endif
