#pragma once

#ifndef _SCRIPT_PAD_IMODULE_H
#define _SCRIPT_PAD_IMODULE_H

#include <concepts>
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

    class IBaseType {
    public:
        virtual void Invoke() = 0;
    };

    class Interpreter : public IBaseType {

    };

    class Compiler : public IBaseType {
    public:
        virtual void Compile() = 0;
    };

    template<class T>
    concept IType = requires(T Sub) {
        std::derived_from<T, IBaseType>;
    };



}   //namespace scriptpad

#endif  //_SCRIPT_PAD_IMODULE_H