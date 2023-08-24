#pragma once

#ifndef _SCRIPT_PAD_IBASE_TYPE_H
#define _SCRIPT_PAD_IBASE_TYPE_H

#include "../resources.h"
#include <string>

namespace scriptpad {
    class ILanguage {
    public:
        virtual bool GetFileContent(std::string*) = 0;
        virtual void SetFileContent(std::string) = 0;
        virtual void Invoke() = 0;
    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_IBASE_TYPE_H