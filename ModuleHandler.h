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
        std::wstring name;
        // path to dll
        path modulePath;
        HINSTANCE hLibrary;
        DLL createModule;
    };

    // Language module configurations
    class ModuleConfig {
    public:
        std::string name;
        // path to main module (interpreter / compiler executable)
        path mainModulePath;
        // path to the modules base directory (where the dll is)
        path moduleDirectory;
        // language specific source file extension (default is .mjs for nodejs ESM, for csharp it's .cs)
        std::string sourceFileExtension;
        // custom language specific library, containing language specific types and (D|d)ump() function
        std::string library;
        // extension for the language specific library
        std::string libraryExtension;
        // interpreter or compiler
        LanguageType engineType;

        std::wstring get_Name();
        std::wstring get_SourceFileExtensions();
        std::wstring get_Library();
        std::wstring get_LibraryExtension();
    };

    class QueryConfig {
    public:
        QueryConfig() {};
        QueryConfig(std::string name, std::string queryPath, std::string queryModule) : name(name), queryPath(queryPath), queryModule(queryModule) {};
        std::string name;
        path queryPath;
        path queryConfigFile();
        std::string queryModule;
        bool unsavedChanges = false;
    };

    class ModuleHandler {
    public:
        class Query {
        public:
            QueryConfig config;

            Query(QueryConfig conf);
            static Query load_Query(ModuleHandler*, std::string);
            static Query new_Query(ModuleHandler*, std::wstring);
            static QueryConfig get_QueryConfig(path);
            static void set_QueryConfig(QueryConfig);
        };

        ModuleHandler(AppWindow*);

        void Initialize();

        std::map<std::wstring, LanguageModule*> Modules;
        std::wstring ActiveModule;
        LanguageModule* Module = nullptr;

        std::vector<Query> Queries;
        std::map<Query, QueryConfig> QueryConfigs;

        path WorkingDirectory;
        path ModulesDirectory;
        path WorkspaceDirectory;

        static std::wstring s_wDefaultModule;
        static std::string s_DefaultSourceFileName;
        static std::string s_DefaultSourceFileExtension;
        static std::wstring s_wDefaultSourceFileName;
        static std::wstring s_wDefautSourceFileExtension;

        static std::map<std::wstring, ModuleConfig> s_ModuleConfigs;


    private:
        AppWindow* m_MainWindow;
        std::vector<path> m_InstalledModules;
        std::vector<path> m_WorkspaceItems;

        std::map<std::wstring, path> m_ModulePaths;
        std::map<std::wstring, LoadedModule> m_LoadedModules;

        std::map<std::wstring, ModuleConfig> get_Modules();
        void init_Workspace();
        void init_Modules();

        std::pair<std::wstring, std::wstring> get_PathsToModules(std::wstring _module) {

        }

        ModuleConfig get_ModuleConfiguration(path);

        DLL LoadModule(std::wstring, path);

        std::wstring HandleWebMessage(Message* msg);
        void HandleOutputReceived(std::string ret);
        void HandleErrorReceived(std::string ret);

    };

    void to_json(json& j, const ModuleConfig& m);
    void from_json(const json& j, ModuleConfig& m);

    void to_json(json& j, const QueryConfig& q);
    void from_json(const json& j, QueryConfig& q);
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_MODULE_HANDLER_H