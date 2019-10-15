#include <string>
#include <clocale>
#include <locale>
#include "../StringPlus/string_plus.h"
#include <sys/time.h>
using namespace std;

string getNbZero(int);

string getDate() // IF DEBUG GIVE A TIME WITH MS PRECISION OR MORE
{
    time_t t = time(0);
    struct tm *now = localtime(&t);
    return getNbZero(now->tm_mday) + "-" + getNbZero(now->tm_mon + 1) + "-" + convertNbToStr<int>(now->tm_year - 100) + "#" + getNbZero(now->tm_hour) + "-" + getNbZero(now->tm_min) + "-" + getNbZero(now->tm_sec);
}

string getNbZero(int number)
{
    //if(config need zero date)
    // return number;
    string strNb = convertNbToStr<int>(number);
    if(strNb.length() < 2)
        return "0" + strNb;
    return strNb;
}

unsigned long getMillis()
{
	timeval curTime;
	gettimeofday(&curTime, NULL);
	return time(0) + curTime.tv_usec / 1000;
}
