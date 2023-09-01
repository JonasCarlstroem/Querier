#ifndef _QUERIER_QUERY_MANAGER_CPP
#define _QUERIER_QUERY_MANAGER_CPP
#include "QueryManager.h"

namespace querier {
    Query::Query(std::string name, std::string path, std::string module) : Name(name), Path(path), ModuleName(module) {

    }

    Query::Query(std::string name, std::string path, std::string module, std::string source) : Name(name), Path(path), ModuleName(module), SourceFile(source) {

    }

    path Query::queryConfigFile() {
        return Path / path(Name + ".json");
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

    QueryManager::QueryManager(AppWindow* mainWin, ModuleManager* moduleManager) : m_MainWindow(mainWin), m_ModuleManager(moduleManager) {

    }

    QueryManager::~QueryManager() {
        CleanupQueries();
    }

    void QueryManager::Initialize(path workingDirectory, path workspaceDirectory) {
        WorkingDirectory = workingDirectory;
        WorkspaceDirectory = workspaceDirectory;
        load_Workspace();
    }

    bool QueryManager::load_Workspace() {
        m_WorkspaceItems = Directory::GetDirectories(WorkspaceDirectory);
        if (m_WorkspaceItems.empty()) {
            Queries.insert({ "query1", new_Query("query1", DefaultModule) });
        }
        else {
            for (auto it = m_WorkspaceItems.begin(); it < m_WorkspaceItems.end(); it++) {
                auto t = it->string();
                Queries.insert({ t, load_Query(it->string()) });
            }
        }

        return Queries.size() > 0;
    }

    Query* QueryManager::load_Query(std::string queryName) {
        path configFile = WorkspaceDirectory / path(queryName) / path(queryName + ".json");
        Query loadQuery = Query::LoadQueryConfigFile(configFile);

        const Module* mod = m_ModuleManager->get_Module(loadQuery.ModuleName);

        if (m_ModuleManager->LoadModule(mod->Name, mod->Path, loadQuery.SourceFile, mod->Data.Library))
            loadQuery.QueryLanguageModule = m_ModuleManager->ActiveModule;

        return new Query(loadQuery);
    }

    Query* QueryManager::new_Query(std::string queryName, std::string _module) {
        const Module* mod = m_ModuleManager->get_Module(_module);
        path queryDirectory = path(WorkspaceDirectory) / path(queryName);

        if (!Directory::Create(queryDirectory))
            PRINT_ERROR(L"Failed creating directory");

        if (!File::Create(queryDirectory / mod->Data.SourceFile))
            PRINT_ERROR(L"CreateFile");

        Query* newQuery = new Query{ queryName, queryDirectory.string(), _module, path(path(queryDirectory) / path(mod->Data.SourceFile)).string() };

        if (m_ModuleManager->LoadModule(_module, mod->Path, newQuery->SourceFile, mod->Data.Library))
            newQuery->QueryLanguageModule = m_ModuleManager->ActiveModule;

        newQuery->SaveQueryConfigFile();

        return newQuery;
    }

    void QueryManager::CleanupQueries() {
        if (Queries.size() > 0) {
            for (auto it : Queries) {
                if (it.second != nullptr) {
                    it.second->SaveQueryConfigFile();
                    delete it.second;
                    it.second = nullptr;
                }
            }
        }
    }

    std::vector<Query> QueryManager::get_QueriesV() {
        std::vector<Query> vec;
        if (Queries.size() > 0) {
            for (auto q : Queries) {
                vec.push_back(*q.second);
            }
        }
        return vec;
    }

    json QueryManager::get_QueriesAsJson() {
            //std::map<std::string, Query> map = get_Queries();
        std::vector<Query> vec = get_QueriesV();
        return json(vec);
    }

    void to_json(json& j, const Query& q) {
        j = json{
            { "name", q.Name },
            { "queryModule", q.ModuleName },
            { "queryPath", q.Path },
            { "querySource", q.SourceFile },
            { "queryLibraries", q.Libraries },
            { "unsavedChanges", (bool)q.UnsavedChanges }
        };
    }

    void from_json(const json& j, Query& q) {
        j.at("name").get_to(q.Name);
        j.at("queryModule").get_to(q.ModuleName);
        j.at("queryPath").get_to(q.Path);
        j.at("querySource").get_to(q.SourceFile);
        j.at("queryLibraries").get_to(q.Libraries);
        j.at("unsavedChanges").get_to(q.UnsavedChanges);
    }

    void QueryManager::HandleCommand(QueryCommand querycmd, Message* msg) {
        msg->msg_type = QUERY_RESPONSE;
        switch (querycmd) {
            case INIT_QUERY:
                msg->content = get_QueriesAsJson().dump();
                break;
            case NEW_QUERY:
            {
                int count = Queries.size() + 1;
                std::string name = std::string("query" + std::to_string(count));
                Query* q = new_Query(name, DefaultModule);
                Queries.insert({ name, q });
                msg->content = json(*q).dump();
                break;
            }
            case LOAD_QUERY:
                break;
            case CLOSE_QUERY:
                break;
            case REMOVE_QUERY:
                break;
        }
    }
}

#endif