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
string GetTimeNow(const string fmt="%Y-%m-%d %I:%M:%S");

// ip to hex string
//
string IP2HexStr(const string &ip);

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

// convert unsigned int to string with limited format
//
string FormatUnsignedInt(const size_t n, const size_t bits);

#endif // SUGAR_H
