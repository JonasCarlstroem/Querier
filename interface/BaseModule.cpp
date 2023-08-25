#ifndef _SCRIPT_PAD_BASE_MODULE_CPP
#define _SCRIPT_PAD_BASE_MODULE_CPP

#include "BaseModule.h"

namespace scriptpad {
    BaseModule::BaseModule() {

    }

    BaseModule::BaseModule(std::wstring applicationName, std::wstring versionArg, bool runAsModule) : IModule(applicationName), m_FFinder() {
        m_isUnicode = true;
        if (_FindInstallation(applicationName, nullptr)) {
            m_bRunAsModule = runAsModule;
            m_bIsModuleInstalled = true;

            StartInfo.wFileName = applicationName;
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;
        }
    }

    BaseModule::BaseModule(std::string applicationName, std::string versionArg, bool runAsModule) : IModule(applicationName) {
        m_isUnicode = false;
        if (_FindInstallation(applicationName, nullptr)) {
            m_bRunAsModule = runAsModule;
            m_bIsModuleInstalled = true;

            StartInfo.FileName = applicationName;
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;
        }
    }

    bool BaseModule::_FindInstallation(std::wstring searchFile, scriptpad::File* retFile) {
        return m_FFinder.SearchFile(searchFile, retFile);
    }

    bool BaseModule::_FindInstallation(path searchFile, File* retFile = nullptr) {
        return m_FFinder.SearchFile(searchFile, retFile);
    }
}

#endif