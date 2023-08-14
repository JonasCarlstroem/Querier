#pragma once

#ifndef _UTIL_H
#define _UTIL_H

#include <string>
namespace util {
    struct strIteratorIndex {
        std::wstring::const_iterator it;
        int id{ 0 };
    };

    bool dup_quotes(std::wstring*);
    bool string_to_wstring(const std::string& source, std::wstring* target);
    std::wstring string_to_wstring(const std::string& source);
    bool wstring_to_string(const std::wstring& source, std::string* target);
    std::string wstring_to_string(const std::wstring& source);
    void print_message(std::wstring);
    void print_message(std::wstring, std::wstring);
}

#endif