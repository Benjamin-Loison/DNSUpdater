#include "../StringPlus/string_plus.h"
#include "date.h"
#include <ctime>
#include <sys/time.h>
using namespace std;

string getNbZero(int, unsigned short = 2);

string getDate()
{
    time_t t = time(0);
    struct tm *now = localtime(&t);

    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    return getNbZero(now->tm_mday) + "-" + getNbZero(now->tm_mon + 1) + "-" + convertNbToStr<int>(now->tm_year - 100) + "#" + getNbZero(now->tm_hour) + "-" + getNbZero(now->tm_min) + "-" + getNbZero(now->tm_sec) + "-" + getNbZero(ms % 1000, 3);
}

string getNbZero(int number, unsigned short numberOfDigits)
{
    string strNb = convertNbToStr<int>(number);
    for(unsigned short digit = strNb.length(); digit < numberOfDigits; digit++)
        strNb = "0" + strNb;
    return strNb;
}
