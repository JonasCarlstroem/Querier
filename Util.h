#pragma once
#include <string>

struct strIteratorIndex {
    std::wstring::const_iterator it;
    int id{ 0 };
};

bool dup_quotes(std::wstring*);