#pragma once

#ifndef _SCRIPT_PAD_MODULE_HANDLER_H
#define _SCRIPT_PAD_MODULE_HANDLER_H

#include "interface/LanguageModule.h"
#include "AppWindow.h"
#include <filesystem>

namespace scriptpad {
    using namespace std::filesystem;
    class ModuleHandler {
    public:
        ModuleHandler(AppWindow*);

        std::map<std::wstring, LanguageModule*> Modules;
        std::wstring ActiveModule;
        LanguageModule* Module = nullptr;

        std::wstring WorkingDirectory;
        std::wstring ModulesDirectory;
        std::wstring WorkspaceDirectory;

        path m_WorkingDirectory;
        path m_ModulesDirectory;
        path m_WorkspaceDirectory;

        path workdir;

    private:
        AppWindow* m_MainWindow;
        std::vector<std::wstring> m_ModulePaths;

        void get_ModuleList() {

        }

        std::wstring HandleWebMessage(Message* msg);
        void HandleOutputReceived(std::string ret);
        void HandleErrorReceived(std::string ret);
    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_MODULE_HANDLER_H