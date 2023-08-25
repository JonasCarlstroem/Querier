#ifndef _SCRIPT_PAD_MODULE_HANDLER_CPP
#define _SCRIPT_PAD_MODULE_HANDLER_CPP

#include "ModuleHandler.h"
//#include "Node.h"

namespace scriptpad {

    Module::Module() {};

    Module::Module(std::string _name, path _path, std::string _lib, LanguageType _type) : Name(_name), Path(_path), Library(_lib), Type(_type) {

    }

    path Query::queryConfigFile() {
        return Path / path(Name + ".json");
    }

    Query::Query(std::string name, std::string path, std::string module) : Name(name), Path(path), ModuleName(module) {
    
    }

    Query::Query(std::string name, std::string path, std::string module, std::string source) : Name(name), Path(path), ModuleName(module), SourceFile(source) {

    }

    Query ModuleHandler::load_Query(std::string queryName) {
        path queryDir = WorkspaceDirectory / path(queryName);
        path configFile = queryDir / path(queryName + ".json");
        Query loadQuery = Query::LoadQueryConfigFile(configFile);

        std::string moduleName = loadQuery.ModuleName.c_str();
        Module* mod = m_mModules[moduleName];
        mod->SourceFile = loadQuery.SourceFile;

        if (LoadModule(*mod))
            loadQuery.loadedLanguageModule = ActiveModule;

        return loadQuery;
    }

    Query ModuleHandler::new_Query(std::string _module) {
        int workspaceItemCount = m_WorkspaceItems.size();
        std::string queryName = std::string("query" + workspaceItemCount);
        Module* mod = m_mModules[_module];
        path queryDirectory = path(WorkspaceDirectory) / path(queryName);
        mod->SourceFile = path(queryDirectory / path(mod->SourceFile)).string();

        if (!Directory::Create(queryDirectory)) 
            print_msg(L"Failed creating directory");

        if (!File::Create(mod->SourceFile))
            PRINT_ERROR(L"CreateFile");

        Query newQuery{ queryName, queryDirectory.string(), _module, mod->SourceFile };
        
        if(LoadModule(*mod))
            newQuery.loadedLanguageModule = ActiveModule;
        
        newQuery.SaveQueryConfigFile();

        return newQuery;
    }

    Query Query::LoadQueryConfigFile() {
        return LoadQueryConfigFile(queryConfigFile());
    }

    void Query::SaveQueryConfigFile() {
        json cont = *this;
        File::Create(queryConfigFile());
        File f(queryConfigFile());
        f.WriteFile(cont.dump(1));
    }

    Query Query::LoadQueryConfigFile(path file) {
        json content = json::parse(File::ReadAllTextW(file));
        return content.template get<Query>();
    }

    ModuleHandler::ModuleHandler(AppWindow* mainWin) {
        WorkingDirectory = Directory::CurrentWorkingDirectory();
        ModulesDirectory = WorkingDirectory / path(L"modules");
        WorkspaceDirectory = WorkingDirectory / path(L"workspace");

        init_Modules();
        init_Workspace();

        m_MainWindow = mainWin;
        m_MainWindow->AddWebMessageHandler(std::bind(&ModuleHandler::HandleWebMessage, this, std::placeholders::_1));
    }

    void ModuleHandler::Initialize() {

    }

    bool ModuleHandler::init_Modules() {
        std::vector<path> mods = Directory::GetDirectories(ModulesDirectory);
        for (auto it = mods.begin(); it < mods.end(); it++) {
            path moduleDirectory = ModulesDirectory / path(*it);
            path moduleFile = moduleDirectory / path(it->wstring() + L".dll");
            path moduleConfig = moduleDirectory / path("module_config.json");
            if (File::Exists(moduleFile)) {
                if (File::Exists(moduleConfig)) {
                    json sd = json::parse(File::ReadAllText(moduleConfig));
                    Module* mod = sd.template get<Module*>();
                    mod->Path = moduleFile;
                    m_mModules.insert({ it->string(), mod });
                }
            }
        }
        return m_mModules.size() > 0;
    }

    bool ModuleHandler::init_Workspace() {
        m_WorkspaceItems = Directory::GetDirectories(WorkspaceDirectory);
        if (m_WorkspaceItems.empty()) {
            Queries.push_back(new_Query(s_DefaultModule));
        }
        else {
            for (auto it = m_WorkspaceItems.begin(); it < m_WorkspaceItems.end(); it++) {
                Queries.push_back(load_Query(it->string()));
            }
        }

        return Queries.size() > 0;
    }

    bool ModuleHandler::LoadModule(Module _module) {
        HINSTANCE hLib;
        DLL ProcAddr{ 0 };
        BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;

        hLib = LoadLibrary(_module.Path.c_str());
        if (hLib != NULL) {
            ProcAddr = (DLL)GetProcAddress(hLib, "CreateModule");

            if (NULL != ProcAddr) {
                fRunTimeLinkSuccess = TRUE;
                m_LoadedModules.insert({ _module.Name, LoadedModule{_module.Name, _module.Path, hLib, ProcAddr } });
                ActiveModuleName = _module.Name;
                ActiveModule = (LanguageModule*)ProcAddr(_module.SourceFile);
                Modules.insert({ ActiveModuleName, ActiveModule});
            }
        }


        return fRunTimeLinkSuccess;
    }

    std::wstring ModuleHandler::HandleWebMessage(Message* msg) {
        switch (msg->cmd) {
            case AppCommand::INITIALIZE:
                Initialize();
                msg->respond = true;
                ActiveModule->GetFileContent(&msg->message);
                break;
            case AppCommand::CONFIG:
                break;
            case AppCommand::CODESYNC:
                ActiveModule->SetFileContent(msg->message);
                break;
            case AppCommand::INVOKE:
                ActiveModule->Invoke();
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

    std::string ModuleHandler::s_DefaultModule = "Nodejs";
    std::wstring ModuleHandler::s_wDefaultModule = L"Nodejs";
    std::string ModuleHandler::s_DefaultSourceFileName = "_eval_";
    std::string ModuleHandler::s_DefaultSourceFileExtension = ".mjs";
    std::wstring ModuleHandler::s_wDefaultSourceFileName = L"_eval_";
    std::wstring ModuleHandler::s_wDefautSourceFileExtension = L".mjs";

    std::map<std::wstring, path> ModuleHandler::m_wModules;
    std::map<std::string, path> ModuleHandler::m_Modules;
    std::map<std::string, Module*> ModuleHandler::m_mModules;

    void to_json(json& j, const Query& q) {
        j = json{
            { "name", q.Name },
            { "queryPath", q.Path },
            { "queryModule", q.ModuleName },
            { "querySource", q.SourceFile },
            { "unsavedChanges", (bool)q.UnsavedChanges }
        };
    }

    void from_json(const json& j, Query& q) {
        j.at("name").get_to(q.Name);
        j.at("queryPath").get_to(q.Path);
        j.at("queryModule").get_to(q.ModuleName);
        j.at("querySource").get_to(q.SourceFile);
        j.at("unsavedChanges").get_to(q.UnsavedChanges);
    }

    void to_json(json& j, const Module& m) {
        j = json{
            { "name", m.Name },
            { "sourceFile", m.SourceFile },
            { "library", m.Library },
            { "type", m.Type }
        };
    }

    void from_json(const json& j, Module& m) {
        j.at("name").get_to(m.Name);
        j.at("sourceFile").get_to(m.SourceFile);
        j.at("library").get_to(m.Library);
        j.at("type").get_to(m.Type);
    }
}

#endif