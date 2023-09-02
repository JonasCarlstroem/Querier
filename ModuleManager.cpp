#ifndef _SCRIPT_PAD_MODULE_HANDLER_CPP
#define _SCRIPT_PAD_MODULE_HANDLER_CPP

#include "ModuleManager.h"
//#include "Node.h"

namespace querier {

    Module::Module() {};

    Module::Module(std::string _name, path _path, std::string _lib, LanguageType _type) : Name(_name), Path(_path), Library(_lib), Type(_type) {

    }

    ModuleManager::ModuleManager(AppWindow* mainWin) : m_MainWindow(mainWin) {

    }

    ModuleManager::~ModuleManager() {
        CleanupLanguageModules();
        CleanupAvailableModules();
        CleanupLoadedModules();
        m_MainWindow = nullptr;
    }

    void ModuleManager::Initialize(path workingDirectory, path modulesDirectory) {
        WorkingDirectory = workingDirectory;
        ModulesDirectory = modulesDirectory;
        load_Modules();
    }

    bool ModuleManager::load_Modules() {
        std::vector<path> mods = Directory::GetDirectories(ModulesDirectory);
        for (auto it = mods.begin(); it < mods.end(); it++) {
            path moduleDirectory = ModulesDirectory / path(*it);
            path moduleData = moduleDirectory / path("data");
            path moduleFile = moduleDirectory / path(it->wstring() + L".dll");
            path moduleConfig = moduleDirectory / path("module_config.json");
            if (File::Exists(moduleFile)) {
                if (File::Exists(moduleConfig)) {
                    json sd = json::parse(File::ReadAllText(moduleConfig));
                    Module mod = sd.template get<Module>();
                    mod.Path = moduleFile;
                    mod.Data.Path = moduleData;
                    mod.Data.Library = mod.Data.Path / mod.Library;
                    mod.Data.SourceFile = std::string(s_DefaultSourceFileName + mod.SourceFileExtension);

                    m_ModulesAvailable.insert({ it->string(), new Module(mod)});
                }
            }
        }
        return m_ModulesAvailable.size() > 0;
    }

    bool ModuleManager::LoadModule(Module _module) {
        HINSTANCE hLib;
        DLL ProcAddr{ 0 };
        BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;

        hLib = LoadLibrary(_module.Path.c_str());
        if (hLib != NULL) {
            ProcAddr = (DLL)GetProcAddress(hLib, "CreateModule");

            if (NULL != ProcAddr) {
                fRunTimeLinkSuccess = TRUE;
                m_LoadedModules.insert({ _module.Name, new LoadedModule{_module.Name, _module.Path, hLib, ProcAddr } });

                ActiveModuleName = _module.Name;
                ActiveModule = (LanguageModule*)ProcAddr(_module.SourceFileExtension, "");
                ActiveModule->OnOutputReceived = std::bind(&ModuleManager::HandleOutputReceived, this, std::placeholders::_1);
                ActiveModule->OnErrorReceived = std::bind(&ModuleManager::HandleErrorReceived, this, std::placeholders::_1);

                Modules.insert({ ActiveModuleName, ActiveModule});
            }
        }


        return fRunTimeLinkSuccess;
    }

    bool ModuleManager::LoadModule(std::string moduleName, path modulePath, path querySourceFile, path libFilePath) {
        HINSTANCE hLib;
        DLL ProcAddr{ 0 };
        BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;

        hLib = LoadLibrary(modulePath.c_str());
        if (hLib != NULL) {
            ProcAddr = (DLL)GetProcAddress(hLib, "CreateModule");

            if (NULL != ProcAddr) {
                fRunTimeLinkSuccess = TRUE;
                m_LoadedModules.insert({ moduleName, new LoadedModule{moduleName, modulePath, hLib, ProcAddr } });
                ActiveModuleName = moduleName;
                ActiveModule = (LanguageModule*)ProcAddr(querySourceFile, libFilePath);
                ActiveModule->OnOutputReceived = std::bind(&ModuleManager::HandleOutputReceived, this, std::placeholders::_1);
                ActiveModule->OnErrorReceived = std::bind(&ModuleManager::HandleErrorReceived, this, std::placeholders::_1);
                Modules.insert({ ActiveModuleName, ActiveModule });
            }
        }


        return fRunTimeLinkSuccess;
    }

    void ModuleManager::HandleOutputReceived(std::string ret) {
        ApplicationMessage* response = new ApplicationMessage{
            MODULE,
            { ret, RESULT_MODULE, SUCCESS_RESULT, ActiveModuleName }
        };
        PostApplicationMessage(response);
        delete response;
    }

    void ModuleManager::HandleErrorReceived(std::string ret) {
        ApplicationMessage* response = new ApplicationMessage{
            MODULE,
            { ret, RESULT_MODULE, ERROR_RESULT, ActiveModuleName }
        };
        PostApplicationMessage(response);
        delete response;
    }

    void ModuleManager::PostApplicationMessage(ApplicationMessage *msg) {
        json* pjson = new json(*msg);
        PostMessage(m_MainWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(pjson), NULL);
    }

    void ModuleManager::HandleCommand(ModuleMessage* msg) {
        switch (msg->cmd) {
                break;
            case CONFIG_MODULE:
                break;
            case CODESYNC_MODULE:
                ActiveModule->SetFileContent(msg->content);
                break;
            case INVOKE_MODULE:
                ActiveModule->Invoke();
                break;
        }
    }

    void ModuleManager::CleanupLoadedModules() {
        if (m_LoadedModules.size() > 0) {
            for (auto it : m_LoadedModules) {
                if (it.second != nullptr) {
                    LoadedModule* lm = it.second;

                    if (FreeLibrary(lm->hLibrary)) {
                        delete lm;
                    }

                    it.second = nullptr;
                }

            }
        }
    }

    void ModuleManager::CleanupAvailableModules() {
        if (m_ModulesAvailable.size() > 0) {
            for (auto it : m_ModulesAvailable) {
                if (it.second != nullptr) {
                    delete it.second;
                    it.second = nullptr;
                }
            }
        }
    }

    void ModuleManager::CleanupLanguageModules() {
        if (Modules.size() > 0) {
            for (auto it : Modules) {
                if (it.second != nullptr) {
                    delete it.second;
                    it.second = nullptr;
                }
            }
        }
    }

    std::map<std::wstring, path> ModuleManager::m_wModules;
    std::map<std::string, path> ModuleManager::m_Modules;
    std::map<std::string, Module*> ModuleManager::m_mModules;

    void to_json(json& j, const Module& m) {
        j = json{
            { "name", m.Name },
            { "sourceFileExtension", m.SourceFileExtension },
            { "library", m.Library },
            { "type", m.Type }
        };
    }

    void from_json(const json& j, Module& m) {
        j.at("name").get_to(m.Name);
        j.at("sourceFileExtension").get_to(m.SourceFileExtension);
        j.at("library").get_to(m.Library);
        j.at("type").get_to(m.Type);
    }
}

#endif