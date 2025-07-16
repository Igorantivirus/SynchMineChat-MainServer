#pragma once

#include <vector>
#include <string>

#include <locale>
#include <codecvt>

std::string to_utf8(const std::wstring &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(str);
}

namespace StringUtils
{

    std::vector<std::string> split(const std::string &s, const char c)
    {
        std::vector<std::string> res;
        std::size_t last = 0;
        for (std::size_t ind = s.find(c); ind != std::string::npos; last = ind + 1, ind = s.find(c, last))
            res.push_back(s.substr(last, ind - last));
        res.push_back(s.substr(last));
        return res;
    }

}