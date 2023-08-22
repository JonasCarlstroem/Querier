#pragma once

#ifndef _SCRIPT_PAD_IMODULE_H
#define _SCRIPT_PAD_IMODULE_H

#include <string>

namespace scriptpad {
    
    class IModule {
    public:
        IModule() : m_wszModulePath{ 0 }, m_bIsModuleInstalled{ 0 }, m_wszModuleVersion{ 0 } {};
        IModule(std::wstring path) : m_wszModulePath(path), m_bIsModuleInstalled{ 0 }, m_wszModuleVersion{ 0 } {};

    protected:
        std::wstring m_wszModulePath;
        bool m_bIsModuleInstalled;
        std::wstring m_wszModuleVersion;

    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_IMODULE_H