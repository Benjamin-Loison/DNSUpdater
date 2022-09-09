#include <fstream>
#include <stdio.h>
#include <iostream>
#include "../../Logger/logger.h"
#include "../../Network/Main/client.h"
#include "../../StringPlus/string_plus.h"
#include <string>
#include <thread>
using namespace std;

char pathSeparator =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif
string pathSeparatorStr =
#ifdef _WIN32
        "\\";
#else
        "/";
#endif

void initializeMain();

string name = "DNSUpdater",
    ext = string("Extensions") + pathSeparator;

/// nearly all of source code comes from LemnosLife video game, so comments were removed because duplicating them across projects is the best way to get a mess
/// I tried to keep only usefull functions... but I have keep useless functions because they may be usefull for enhanced versions...

int main()
{
    initializeLogger();
    initializeNetwork();

    return 0;
}

void free()
{
    print("freeing...");
    freeNetwork();
}
