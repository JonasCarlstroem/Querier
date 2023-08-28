#pragma once

#ifndef _SCRIPT_PAD_IMODULE_H
#define _SCRIPT_PAD_IMODULE_H

#include "../resource.h"
#include <string>

namespace querier {
    
    class IModule {
    public:
        IModule() : m_wszMainModulePath{ 0 }, m_bIsModuleInstalled{ 0 }, m_wszModuleVersion{ 0 } {};
        IModule(std::wstring path) : m_wszMainModulePath(path), m_bIsModuleInstalled{ 0 }, m_wszModuleVersion{ 0 } {};
        IModule(std::string path) : m_szMainModulePath(path), m_bIsModuleInstalled{ 0 }, m_szModuleVersion{ 0 } {};

    protected:
        std::string m_szMainModulePath;
        std::wstring m_wszMainModulePath;
        bool m_bIsModuleInstalled;
        std::string m_szModuleVersion;
        std::wstring m_wszModuleVersion;

    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_IMODULE_H