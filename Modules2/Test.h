#pragma once

#include "pch.h"
#include "../interface/LanguageModule.h"

class Test : public scriptpad::LanguageModule {
public:
    Test();
    void Initialize();
};

__declspec(dllexport) scriptpad::LanguageModule* CreateModule() {
    scriptpad::LanguageModule* mod = new Test();
    return mod;
}