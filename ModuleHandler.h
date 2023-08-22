#pragma once

#ifndef _SCRIPT_PAD_MODULE_HANDLER_H
#define _SCRIPT_PAD_MODULE_HANDLER_H

#include "LanguageModule.h"

namespace scriptpad {
    class ModuleHandler {
    public:
        std::vector<LanguageModule<Interpreter>> Interpreters;
        std::vector<LanguageModule<Compiler>> Compilers;
    private:

    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_MODULE_HANDLER_H