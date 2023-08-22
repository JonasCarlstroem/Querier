#pragma once

#ifndef _SCRIPT_PAD_LANGUAGE_MODULE_H
#define _SCRIPT_PAD_LANGUAGE_MODULE_H

#include "IBaseType.h"
#include "BaseModule.h"
#include "AppWindow.h"

namespace scriptpad {
    template<IType U>
    class LanguageModule : public U, public BaseModule {
    public:
        LanguageModule(std::wstring path) : U(), BaseModule(path), m_AppWindow{ 0 } {};

        virtual void Initialize(AppWindow* appWin) = 0;

    protected:
        AppWindow* m_AppWindow;
    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_LANGUAGE_MODULE_H