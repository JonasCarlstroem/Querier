#include "pch.h"
#include "Npm.h"

NPM::NPM() : CmdModule(L"npm", L"--version") {

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