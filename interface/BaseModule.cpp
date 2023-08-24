#ifndef _SCRIPT_PAD_BASE_MODULE_CPP
#define _SCRIPT_PAD_BASE_MODULE_CPP

#include "BaseModule.h"

namespace scriptpad {
    BaseModule::BaseModule() {

    }

    BaseModule::BaseModule(std::wstring applicationName, std::wstring versionArg, bool runAsModule) : IModule(applicationName), m_FFinder() {
        File file;
        if (_FindInstallation(applicationName, &file)) {
            m_bRunAsModule = runAsModule;
            m_bIsModuleInstalled = true;

            StartInfo.wFileName = applicationName;
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;
        }
    }

    bool BaseModule::_FindInstallation(std::wstring searchFile, scriptpad::File* retFile) {
        return m_FFinder.SearchFile(searchFile, retFile);
    }
}

#endif