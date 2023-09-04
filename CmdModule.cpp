#ifndef _SCRIPT_PAD_CMD_MODULE_CPP
#define _SCRIPT_PAD_CMD_MODULE_CPP

#include "CmdModule.h"

namespace querier {
    CmdModule::CmdModule(std::wstring wAppName, std::wstring wVersionArg, bool runAsModule) : BaseModule(L"C:\\windows\\system32\\cmd.exe", wVersionArg, runAsModule), m_wszAppName(wAppName) {
        if (m_bIsModuleInstalled) {
            StartInfo.wFileName = m_wszMainModulePath;
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;

            m_wszVersionArg = wVersionArg;
            m_wszModuleVersion = str_to_wstr(wRun(wVersionArg));
        }
    }

    CmdModule::CmdModule(std::string appName, std::string versionArg, bool runAsModule) : BaseModule("C:\\windows\\system32\\cmd.exe", versionArg, runAsModule), m_szAppName(appName) {
        if (m_bIsModuleInstalled) {
            StartInfo.FileName = m_szMainModulePath;
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;

            m_szVersionArg = versionArg;
            m_szModuleVersion = Run(versionArg);
        }
    }

    CmdModule::~CmdModule() {

    }
}

#endif