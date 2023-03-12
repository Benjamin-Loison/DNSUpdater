#include "../Server/server.h"
#include "../Logger/logger.h"
using namespace std;

string ext, name = "DNSUpdater";
bool DEBUG = true;
string pathSeparatorStr =
#ifdef _WIN32
    "\\";
#else
    "/";
#endif
char pathSeparator =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

int main()
{
    ext = string("Extensions") + pathSeparator;
    
	initializeLogger();
    initializeNetwork();
    
	freeNetwork();
}

