#pragma once

#ifndef _SCRIPT_PAD_MODULE_HANDLER_H
#define _SCRIPT_PAD_MODULE_HANDLER_H

#include "interface/LanguageModule.h"
#include "interface/ILanguage.h"
#include "AppWindow.h"
#include "Util.h"
#include <filesystem>
#include "resource.h"
#include "ApplicationData.h"

namespace querier {
    using namespace std::filesystem;
    typedef LanguageModule*(_cdecl* DLL)(path);

    // Loaded language module resources
    struct LoadedModule {
        std::string name;
        // path to dll
        path modulePath;
        HINSTANCE hLibrary = nullptr;
        DLL createModule = nullptr;
    };

    using LoadedModules = std::map<std::string, LoadedModule*>;

    struct ModuleData {
        path Path;
        path Library;
        path SourceFile;
    };

    struct Module {
        Module();
        Module(std::string _name, path _path, std::string _lib, LanguageType _type);
        std::string Name;
        std::string SourceFileExtension;
        std::string Library;
        LanguageType Type{ Interpreter };

        LanguageModule* LanguageModule;
        bool UnicodeModule = true;

        ModuleData Data;
        path Path;
    };

    using AvailableModules = std::map<std::string, Module*>;

    class ModuleManager {
    public:

        ModuleManager(AppWindow*);
        ~ModuleManager();
        void Initialize(path, path);

        //bool LoadModule(Module);
        bool LoadModule(std::string, path, path, path, LanguageModule**);

        std::map<std::string, Module*> Modules;
        /*std::string ActiveModuleName;
        LanguageModule* ActiveModule = nullptr;*/

        path WorkingDirectory,
             ModulesDirectory;

        inline static std::string s_DefaultModule = "Nodejs",
                                  s_DefaultSourceFileName = "_eval_",
                                  s_DefaultSourceFileExtension = ".mjs";

        //static std::wstring s_wDefaultModule,
        //                    s_wDefaultSourceFileName,
        //                    s_wDefautSourceFileExtension;

        Module* get_Module(std::string name) {
            return Modules[name];
        }

        void HandleCommand(ModuleMessage*);

    private:
        AppWindow* m_MainWindow;
        std::vector<path> m_InstalledModules;
        std::vector<path> m_WorkspaceItems;
        LoadedModules m_LoadedModules;
        AvailableModules m_ModulesAvailable;

        static std::map<std::wstring, path> m_wModules;
        static std::map<std::string, path> m_Modules;
        static std::map<std::string, Module*> m_mModules;

        bool load_Modules();

        void HandleOutputReceived(std::string ret, std::string);
        void HandleErrorReceived(std::string ret, std::string);

        void PostApplicationMessage(ApplicationMessage*);

        void CleanupLoadedModules();
        void CleanupAvailableModules();
        void CleanupLanguageModules();
    };

    void to_json(json& j, const Module& m);
    void from_json(const json& j, Module& m);
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_MODULE_HANDLER_H