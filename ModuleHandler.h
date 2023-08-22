#pragma once

#ifndef _SCRIPT_PAD_MODULE_HANDLER_H
#define _SCRIPT_PAD_MODULE_HANDLER_H

#include "LanguageModule.h"

namespace scriptpad {
    class ModuleHandler {
    public:
        ModuleHandler(AppWindow*);

        std::map<std::wstring, LanguageModule*> Modules;
        std::wstring ActiveModule;
        LanguageModule* Module = nullptr;

        std::wstring HandleWebMessage(Message* msg);
        void HandleOutputReceived(std::string ret);
        void HandleErrorReceived(std::string ret);
    private:
        AppWindow* m_MainWindow;
    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_MODULE_HANDLER_H