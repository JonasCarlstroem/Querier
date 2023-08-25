#pragma once

#ifndef _SCRIPT_PAD_MODULE_HANDLER_H
#define _SCRIPT_PAD_MODULE_HANDLER_H

#include "interface/LanguageModule.h"
#include "interface/ILanguage.h"
#include "AppWindow.h"
#include "Util.h"
#include <filesystem>

namespace scriptpad {
    using namespace std::filesystem;
    typedef LanguageModule*(_cdecl* DLL)();
    
    // Loaded language module resources
    struct LoadedModule {
        std::string name;
        // path to dll
        path modulePath;
        HINSTANCE hLibrary = nullptr;
        DLL createModule = nullptr;
    };

    struct Module {
        std::string name;
        path path;
        std::string sourceExtension;
    };

    class Query {
    public:
        Query() {};
        Query(std::string, std::string, std::string);
        Query(std::string, std::string, std::string, std::string);
        Query(std::string, std::string, std::string, std::string, std::string);
        std::string Name;
        std::string Path;
        std::string ModuleName;
        std::string SourceFile;
        std::string SourceExtension;
        bool UnsavedChanges = false;

        LanguageModule* loadedLanguageModule = nullptr;
        LoadedModule loadedModule;
        path queryConfigFile();
        Query LoadQueryConfigFile();
        void SaveQueryConfigFile();

        static Query LoadQueryConfigFile(path file);
    };

    class ModuleHandler {
    public:
        ModuleHandler(AppWindow*);

        void Initialize();

        std::map<std::string, LanguageModule*> Modules;
        std::string ActiveModuleName;
        LanguageModule* ActiveModule = nullptr;

        std::vector<Query> Queries;

        path WorkingDirectory;
        path ModulesDirectory;
        path WorkspaceDirectory;

        static std::string s_DefaultModule;
        static std::wstring s_wDefaultModule;

        static std::string s_DefaultSourceFileName;
        static std::string s_DefaultSourceFileExtension;

        static std::wstring s_wDefaultSourceFileName;
        static std::wstring s_wDefautSourceFileExtension;

    private:
        AppWindow* m_MainWindow;
        std::vector<path> m_InstalledModules;
        std::vector<path> m_WorkspaceItems;

        std::map<std::string, LoadedModule> m_LoadedModules;
        static std::map<std::wstring, path> m_wModules;
        static std::map<std::string, path> m_Modules;
        static std::map<std::string, Module> m_mModules;

        void init_Modules();
        void init_Workspace();

        bool LoadModule(Module);

        Query load_Query(std::string);
        Query new_Query(std::string);

        std::wstring HandleWebMessage(Message* msg);
        void HandleOutputReceived(std::string ret);
        void HandleErrorReceived(std::string ret);

    };

    /*void to_json(json& j, const ModuleConfig& m);
    void from_json(const json& j, ModuleConfig& m);*/

    void to_json(json& j, const Query& q);
    void from_json(const json& j, Query& q);
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_MODULE_HANDLER_H