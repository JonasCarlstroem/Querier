#pragma once

#ifndef _SCRIPT_PAD_MODULE_H
#define _SCRIPT_PAD_MODULE_H

#include "IModule.h"
#include "AppWindow.h"
#include "Process.h"

namespace mod {
    
    // implement Nodejs and Npm constructor functionality into base class

    class IModule : public IBaseModule, public process::Process {
    public:
        IModule() {};
        IModule(std::wstring path) : IBaseModule(path) {};
    };

    template<IType U>
    class Module : public U, public IModule {
    public:
        Module(std::wstring path) : U(), IModule(path), m_AppWindow{ 0 } {};

        virtual void Initialize(app::AppWindow* appWin) = 0;

    protected:
        app::AppWindow* m_AppWindow;
    };

    template<IType U>
    class LanguageModule : public Module<U> {
    public:
        LanguageModule(std::wstring modulePath) : Module<U>(modulePath) {};
    };

    class SubModule : public IModule {
    public:
        SubModule() {};
        SubModule(std::wstring modulePath) : IModule(modulePath) {};
    };
}

#endif