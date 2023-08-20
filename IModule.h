#pragma once

#ifndef _SCRIPT_PAD_IMODULE_H
#define _SCRIPT_PAD_IMODULE_H

#include <concepts>
#include <string>

namespace mod {
    
    // rename to IModule
    class IBaseModule {
    public:
        IBaseModule() : m_ModulePath{ 0 } {}
        IBaseModule(std::wstring path) : m_ModulePath(path) {};

        virtual std::string Run(std::wstring) = 0;
    protected:
        std::wstring m_ModulePath;
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



}

#endif