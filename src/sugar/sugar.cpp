#include <vector>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "sugar.h"
#include "gdebug.h"

using std::string;
using std::vector;
using std::stringstream;

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

string IP2HexStr(const string &ip)
{
    string ip_(ip);
    vector<string> strs;

    boost::split(strs, ip_, boost::is_any_of("."));

    string ip_hex_str;
    for (int i = 0; i < strs.size(); ++i) {
        int ip_sub_i = std::stoi(strs[i], nullptr, 10);
        stringstream ss;
        ss << std::hex << ip_sub_i;
        string str_sub = ss.str();
        if (str_sub.size() == 1)
            str_sub = "0" + str_sub;
        ip_hex_str += str_sub;
    }

    return ip_hex_str;
}
