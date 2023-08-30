#pragma once

#ifndef _QUERIER_QUERY_MANAGER_H
#define _QUERIER_QUERY_MANAGER_H
#include "interface/LanguageModule.h"
#include "AppWindow.h"
#include "ModuleManager.h"
#include "resource.h"

namespace querier {
    class Query {
    public:
        Query() {};
        Query(std::string, std::string, std::string);
        Query(std::string, std::string, std::string, std::string);

        std::string Name;
        std::string Path;
        std::string ModuleName;
        std::string SourceFile;
        std::vector<std::string> Libraries;
        bool UnsavedChanges = false;

        LanguageModule* LoadedLanguageModule = nullptr;
        path queryConfigFile();
        Query LoadQueryConfigFile();
        void SaveQueryConfigFile();

        static Query LoadQueryConfigFile(path file);
    };

    class QueryManager {
    public:
        QueryManager(AppWindow*, ModuleManager*);
        ~QueryManager();

        void Initialize(path, path);

        std::map<std::string, Query*> Queries;
        std::map<std::string, Query> get_Queries() {
            std::map<std::string, Query> map;
            if (Queries.size() > 0) {
                for (auto q : Queries) {
                    map.insert({ q.first, *q.second });
                }
            }
            return map;
        }

        json get_QueriesAsJson() {
            return json::parse(get_Queries());
        }

        path WorkingDirectory,
             WorkspaceDirectory;
        inline static std::string DefaultModule = "Nodejs";
        inline static std::string DefaultSourceFileExtension = ".mjs";
    private:
        AppWindow* m_MainWindow;
        ModuleManager* m_ModuleManager;
        std::vector<path> m_WorkspaceItems;

        bool load_Workspace();

        Query* load_Query(std::string);
        Query* new_Query(std::string, std::string);

        void SaveQueries();
        void CleanupQueries();
    };

    void to_json(json& j, const Query& q);
    void from_json(const json& j, Query& q);

    void to_json(json& j, const std::vector<Query>& q);
    void from_json(const json& j, std::vector<Query>& q);
} //namespace querier

#endif  //_QUERIER_QUERY_MANAGER_H