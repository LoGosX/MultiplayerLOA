#include "common/utils/ip.h"
#include <vector>

bool isNumber(const std::string& str)
{
    return !str.empty() &&
        (str.find_first_not_of("[0123456789]") == std::string::npos);
}
 
std::vector<std::string> split(const std::string& str, char delim)
{
    auto i = 0;
    std::vector<std::string> list;
    auto pos = str.find(delim);
    while (pos != std::string::npos)
    {
        list.push_back(str.substr(i, pos - i));
        i = ++pos;
        pos = str.find(delim, pos);
    }
    list.push_back(str.substr(i, str.length()));
    return list;
}
 
bool validateIP(std::string ip)
{
    std::vector<std::string> list = split(ip, '.');
    if (list.size() != 4)
        return false;
    for (std::string str : list)
    {
        if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0)
            return false;
    }
    return true;
}