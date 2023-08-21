#ifndef _SCRIPT_PAD_MODULE_CPP
#define _SCRIPT_PAD_MODULE_CPP

#include "Module.h"

namespace scriptpad {

    BaseModule::BaseModule(std::wstring applicationName) : IModule(applicationName), m_FFinder() {
        scriptpad::File file;
        if (_FindInstallation(applicationName, &file)) {
            m_bIsModuleInstalled = true;
        }
    }

    bool BaseModule::_FindInstallation(std::wstring searchFile, scriptpad::File* retFile) {
        return m_FFinder.SearchFile(searchFile, retFile);
    }

    CmdModule::CmdModule(std::wstring appName) : BaseModule(L"C:\\windows\\system32\\cmd.exe"), m_wszAppName(appName) {
        if (m_bIsModuleInstalled) {
            m_wszModuleVersion = scriptpad::str_to_wstr(Run(L"-v"));
        }
    }

    CmdModule::CmdModule(std::wstring appName, std::wstring versionArg) : BaseModule(L"C:\\windows\\system32\\cmd.exe"), m_wszAppName(appName) {
        if (m_bIsModuleInstalled) {
            StartInfo.wFileName = m_wszModulePath;
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;

            m_wszVersionArg = versionArg;
            m_wszModuleVersion = scriptpad::str_to_wstr(Run(versionArg));
        }
    }
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_MODULE_CPP