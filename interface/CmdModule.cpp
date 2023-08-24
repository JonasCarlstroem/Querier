#ifndef _SCRIPT_PAD_CMD_MODULE_CPP
#define _SCRIPT_PAD_CMD_MODULE_CPP

#include "CmdModule.h"

namespace scriptpad {
    CmdModule::CmdModule(std::wstring appName, std::wstring versionArg, bool runAsModule) : BaseModule(L"C:\\windows\\system32\\cmd.exe", versionArg, runAsModule), m_wszAppName(appName) {
        if (m_bIsModuleInstalled) {
            StartInfo.wFileName = m_wszMainModulePath;
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;

            m_wszVersionArg = versionArg;
            m_wszModuleVersion = scriptpad::str_to_wstr(Run(versionArg));
        }
    }

    CmdModule::~CmdModule() {

    }
}

#endif