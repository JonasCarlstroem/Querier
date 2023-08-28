#pragma once

#ifndef _SCRIPT_PAD_IBASE_TYPE_H
#define _SCRIPT_PAD_IBASE_TYPE_H

#include "../resource.h"
#include <string>

namespace querier {
    class ILanguage {
    public:
        virtual bool GetFileContent(std::string*) = 0;
        virtual void SetFileContent(std::string) = 0;
        virtual void Invoke() = 0;
    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_IBASE_TYPE_H