#ifndef SUGAR_H
#define SUGAR_H

#include <sstream>
#include <string>

using std::ostringstream;
using std::string;

template <typename T>
string NumberToString(T Number)
{
    ostringstream ss;
    ss << Number;
    return ss.str();
}

string GetTimeNow(const string fmt="%d-%m-%Y_%I:%M:%S")
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

void ResetBuffer(char *buf, int size)
{

}

void LogInfo(const char *info_name, const char *info)
{
    fprintf(stdout, "INFO - %s - %s\n", info_name, info);
}

void LogError(const char *error)
{
    fprintf(stderr, "ERROR - %s\n", error);
}

#endif // SUGAR_H
