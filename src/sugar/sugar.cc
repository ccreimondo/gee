#include <string>
#include "sugar.h"

using std::string;

string GetTimeNow(const string fmt)
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, fmt.c_str(), timeinfo);
    string str(buffer);

    return str;
}
