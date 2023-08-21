#pragma once

#ifndef _SCRIPT_PAD_NPM_H
#define _SCRIPT_PAD_NPM_H

#include "Module.h"
#include "File.h"
#include <utility>

namespace scriptpad {
    class NPM : public scriptpad::CmdModule {
    public:
        NPM();

        bool Install(std::wstring package);
        bool Install(std::wstring, bool);

    private:
        std::wstring m_npmVersion;
        const std::wstring m_baseArg = L"/C npm";
        const std::wstring m_localInstallArg = L"install";
        const std::wstring m_globalInstallArg = L"install -g";
        scriptpad::FileFinder m_FileFinder;
    };

}   //namespace scriptpad

#endif  //_SCRIPT_PAD_NPM_H