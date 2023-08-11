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
}

#endif