#ifndef _SCRIPT_PAD_NPM_CPP
#define _SCRIPT_PAD_NPM_CPP

#include "Npm.h"

namespace scriptpad {

    NPM::NPM() : scriptpad::CmdModule(L"npm", L"--version"), m_FileFinder() {
        
    }

    bool NPM::Install(std::wstring package) {
        return false;
    }

    bool NPM::Install(std::wstring pkg, bool global) {
        if (global) {
            //std::string ret = Run(L"install", L"-g", pkg.c_str());
        }
        return false;
    }

}   //namespace scriptpad

#endif  //_SCRIPT_PAD_NPM_CPP