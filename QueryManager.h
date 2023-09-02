#pragma once

#ifndef _QUERIER_QUERY_MANAGER_H
#define _QUERIER_QUERY_MANAGER_H
#include "interface/LanguageModule.h"
#include "ModuleManager.h"
#include "AppWindow.h"
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
        std::string ModuleVersion;
        std::string SourceFile;
        std::string SourceFileContent;
        std::vector<std::string> Libraries;
        bool UnsavedChanges = false;

        LanguageModule* QueryLanguageModule = nullptr;
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

        std::vector<Query> get_QueriesV();

        json get_QueriesAsJson();

        path WorkingDirectory,
             WorkspaceDirectory;
        inline static std::string DefaultModule = "Nodejs";
        inline static std::string DefaultSourceFileExtension = ".mjs";

        void HandleCommand(QueryMessage*);

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
} //namespace querier

#endif  //_QUERIER_QUERY_MANAGER_H