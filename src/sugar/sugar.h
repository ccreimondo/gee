#ifndef SUGAR_H
#define SUGAR_H

#include <cstdio>
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

// get time now
//
string GetTimeNow(const string fmt="%d-%m-%Y %I:%M:%S");

// log handler
//
inline void LogInfo(const char *info_name, const char *info)
{
    fprintf(stdout, "INFO - %s - %s\n", info_name, info);
}

inline void LogError(const char *error)
{
    fprintf(stderr, "ERROR - %s\n", error);
}

#endif // SUGAR_H
