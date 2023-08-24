#ifndef _SCRIPT_PAD_MODULE_HANDLER_CPP
#define _SCRIPT_PAD_MODULE_HANDLER_CPP

#include "ModuleHandler.h"
//#include "Node.h"

namespace scriptpad {

    std::wstring ModuleConfig::get_Name() {
        return str_to_wstr(name);
    }

    std::wstring ModuleConfig::get_SourceFileExtensions() {
        return str_to_wstr(sourceFileExtension);
    }

    std::wstring ModuleConfig::get_Library() {
        return str_to_wstr(library);
    }

    std::wstring ModuleConfig::get_LibraryExtension() {
        return str_to_wstr(libraryExtension);
    }

    path QueryConfig::queryConfigFile() {
        return queryPath / path(name + ".json");
    }

    ModuleHandler::Query::Query(QueryConfig conf) : config(conf) {

    }

    ModuleHandler::Query ModuleHandler::Query::load_Query(ModuleHandler* handler, std::string queryName) {
        path queryDir = handler->WorkspaceDirectory / path(queryName);
        path configFile = queryDir / path(queryName + ".json");
        QueryConfig config = get_QueryConfig(configFile);

        // load language module
        return Query(config);
    }

    ModuleHandler::Query ModuleHandler::Query::new_Query(ModuleHandler* handler, std::wstring _module) {
        int workspaceItemCount = handler->m_WorkspaceItems.size();
        std::string queryName = std::string("query" + workspaceItemCount);

        path queryDir = path(handler->WorkspaceDirectory) / path(queryName);
        if (!Directory::Create(queryDir)) 
            print_msg(L"Failed creating directory");

        if (!File::Create(queryDir / path(ModuleHandler::s_DefaultSourceFileName + ModuleHandler::s_DefaultSourceFileExtension)))
            PRINT_ERROR(L"CreateFile");

        ModuleConfig config = handler->s_ModuleConfigs[_module];

        DLL createModuleFn = handler->LoadModule(_module, handler->m_ModulePaths[_module]);
        handler->Modules.insert({ _module, (LanguageModule*)createModuleFn() });
        handler->Module = handler->Modules[_module];
        handler->ActiveModule = _module;

        QueryConfig qConf = QueryConfig{ queryName, queryDir.string(), wstr_to_str(_module) };
        set_QueryConfig(qConf);

        return Query(qConf);
    }

    QueryConfig ModuleHandler::Query::get_QueryConfig(path configFile) {
        json content = json::parse(File::ReadAllTextW(configFile));
        return content.template get<QueryConfig>();
    }

    void ModuleHandler::Query::set_QueryConfig(QueryConfig conf) {
        json cont = json(conf);
        File::Create(conf.queryConfigFile());
        File f(conf.queryConfigFile());
        f.WriteFile(cont.dump());
    }

    ModuleHandler::ModuleHandler(AppWindow* mainWin) {
        WorkingDirectory = Directory::CurrentWorkingDirectory();
        ModulesDirectory = WorkingDirectory / path(L"modules");
        WorkspaceDirectory = WorkingDirectory / path(L"workspace");

        //m_InstalledModules = Directory::GetDirectories(ModulesDirectory);
        s_ModuleConfigs = get_Modules();
        init_Workspace();
        //m_WorkspaceItems = Directory::GetDirectories(WorkspaceDirectory);
        init_Modules();

        m_MainWindow = mainWin;
        m_MainWindow->AddWebMessageHandler(std::bind(&ModuleHandler::HandleWebMessage, this, std::placeholders::_1));
        
        /*if (Modules.count(L"nodejs") == 0)
            Modules.insert({ L"nodejs", new Nodejs(ESM, L"langs\\NodeJS\\_eval_", L".mjs")});

        Module = Modules[L"nodejs"];
        Module->OnOutputReceived = std::bind(&ModuleHandler::HandleOutputReceived, this, std::placeholders::_1);
        Module->OnErrorReceived = std::bind(&ModuleHandler::HandleErrorReceived, this, std::placeholders::_1);*/
        //Module->Initialize(mainWin);
    }

    void ModuleHandler::Initialize() {

    }

    std::map<std::wstring, ModuleConfig> ModuleHandler::get_Modules() {
        if (s_ModuleConfigs.size() > 0)
            return s_ModuleConfigs;

        std::map<std::wstring, ModuleConfig> result;
        auto mods = Directory::GetDirectories(ModulesDirectory);
        for (auto it = mods.begin(); it < mods.end(); it++) {
            const auto moduleDir = ModulesDirectory / path(*it);
            const auto moduleFile = moduleDir / path(it->wstring() + L".dll");
            if (File::Exists(moduleFile)) {
                m_ModulePaths.insert({ *it, moduleFile });
                const auto configFile = ModulesDirectory / path(*it) / path(L"data") / path(L"config.json");
                if (File::Exists(configFile)) {
                    result.insert({ it->wstring(), get_ModuleConfiguration(configFile)});
                    result[it->wstring()].moduleDirectory = moduleDir;
                }
                else {
                    result.insert({ it->wstring(), ModuleConfig{ it->string(), L"C:\\Program Files\\nodejs\\node.exe", moduleDir, s_DefaultSourceFileExtension, "_lib_", ".mjs", Interpreter}});
                }
            }
        }

        return result;
    }

    void ModuleHandler::init_Workspace() {
        m_WorkspaceItems = Directory::GetDirectories(WorkspaceDirectory);
        if (m_WorkspaceItems.empty()) {
            Queries.push_back(Query::new_Query(this, s_wDefaultModule));
        }
        else {
            for (auto it = m_WorkspaceItems.begin(); it < m_WorkspaceItems.end(); it++) {
                Queries.push_back(Query::load_Query(this, it->string()));
            }
        }
    }

    void ModuleHandler::init_Modules() {
        for (auto it = m_WorkspaceItems.begin(); it < m_WorkspaceItems.end(); it++) {
            path workspaceItem = WorkspaceDirectory / path(*it);
            const auto files = Directory::GetFiles(workspaceItem);

        }
    }

    //void ModuleHandler::new_Query(std::wstring _module) {
    //    if (m_WorkspaceItems.empty()) {
    //        if (s_ModuleConfigs.count(_module) > 0) {
    //            Query newQuery = Query::new_Query(WorkspaceDirectory, _module);
    //            Directory::Create(WorkspaceDirectory / path(L"query1"));
    //            File::Create(WorkspaceDirectory / path(L"query1") / path(s_DefaultSourceFileName + s_DefaultSourceFileExtension));
    //            ModuleConfig modConf = s_ModuleConfigs[_module];

    //            DLL createModuleFn = LoadModule(_module, m_ModulePaths[_module]);
    //            Modules.insert({ _module, (LanguageModule*)createModuleFn() });
    //            Module = Modules[_module];
    //            ActiveModule = _module;
    //        }
    //    }
    //}

    ModuleConfig ModuleHandler::get_ModuleConfiguration(path mod) {
        json data = json::parse(File::ReadAllText(mod));
        return data.template get<ModuleConfig>();
    }

    DLL ModuleHandler::LoadModule(std::wstring moduleName, path _path) {
        HINSTANCE hLib;
        DLL ProcAddr{ 0 };
        BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;

        hLib = LoadLibrary(_path.c_str());
        if (hLib != NULL) {
            ProcAddr = (DLL)GetProcAddress(hLib, "CreateModule");

            if (NULL != ProcAddr) {
                fRunTimeLinkSuccess = TRUE;
                m_LoadedModules.insert({ moduleName, LoadedModule{moduleName, _path, hLib, ProcAddr } });
            }
        }

        return ProcAddr;
    }

    std::wstring ModuleHandler::HandleWebMessage(Message* msg) {
        switch (msg->cmd) {
            case AppCommand::INITIALIZE:
                Initialize();
                msg->respond = true;
                Module->GetFileContent(&msg->message);
                break;
            case AppCommand::CONFIG:
                break;
            case AppCommand::CODESYNC:
                Module->SetFileContent(msg->message);
                break;
            case AppCommand::INVOKE:
                Module->Invoke();
                break;
        }
        return str_to_wstr(json(*msg).dump());
    }

    void ModuleHandler::HandleOutputReceived(std::string ret) {
        Message* response = new Message{ AppCommand::RESULT, ResultType::ISSUCCESS };
        response->message = ret;
        PostMessage(m_MainWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(response), NULL);
    }

    void ModuleHandler::HandleErrorReceived(std::string ret) {
        Message* response = new Message{ AppCommand::RESULT, ResultType::ISERROR };
        response->error = ret;
        PostMessage(m_MainWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(response), NULL);
    }

    std::wstring ModuleHandler::s_wDefaultModule = L"Nodejs";
    std::string ModuleHandler::s_DefaultSourceFileName = "_eval_";
    std::string ModuleHandler::s_DefaultSourceFileExtension = ".mjs";
    std::wstring ModuleHandler::s_wDefaultSourceFileName = L"_eval_";
    std::wstring ModuleHandler::s_wDefautSourceFileExtension = L".mjs";

    std::map<std::wstring, ModuleConfig> ModuleHandler::s_ModuleConfigs{};

    void to_json(json& j, const ModuleConfig& m) {
        j = json{
                { "name", m.name },
                { "mainModulePath", m.mainModulePath },
                { "sourceFileExtension", m.sourceFileExtension },
                { "library", m.library },
                { "libraryExtension", m.libraryExtension },
                { "engineType", m.engineType }
        };
    }

    void from_json(const json& j, ModuleConfig& m) {
        j.at("name").get_to(m.name);
        j.at("mainModulePath").get_to(m.mainModulePath);
        j.at("sourceFileExtension").get_to(m.sourceFileExtension);
        j.at("library").get_to(m.library);
        j.at("libraryExtension").get_to(m.libraryExtension);
        j.at("engineType").get_to(m.engineType);
    }

    void to_json(json& j, const QueryConfig& q) {
        j = json{
            { "name", q.name },
            { "queryPath", q.queryPath },
            { "queryModule", q.queryModule },
            { "unsavedChanges", (bool)q.unsavedChanges }
        };
    }

    void from_json(const json& j, QueryConfig& q) {
        j.at("name").get_to(q.name);
        j.at("queryPath").get_to(q.queryPath);
        j.at("queryModule").get_to(q.queryModule);
        j.at("unsavedChanges").get_to(q.unsavedChanges);
    }
}

#endif