#pragma once

#ifndef _SCRIPT_PAD_NPM_H
#define _SCRIPT_PAD_NPM_H

#include "CmdModule.h"
#include "File.h"
#include <utility>

namespace scriptpad {
    class NPM : public CmdModule {
    public:
        NPM();

        bool Install(std::wstring package);
        bool Install(std::wstring, bool);

    private:
        std::wstring m_npmVersion;
        const std::wstring m_baseArg = L"/C npm";
        const std::wstring m_localInstallArg = L"install";
        const std::wstring m_globalInstallArg = L"install -g";
        File m_FileFinder;
    };

}   //namespace scriptpad

#endif  //_SCRIPT_PAD_NPM_H