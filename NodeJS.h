#pragma once

#ifndef _NODE_JS_H
    #define _NODE_JS_H
#include "Module.h"
#include "Npm.h"
#include "File.h"
#include "WebView2.h"
#include <functional>
#include <nlohmann/json.hpp>


namespace nodejs {
    using namespace mod;
    using json = nlohmann::json;

    struct Env {
        std::wstring key;
        std::wstring value;
    };

    enum NodeJSType {
        CLASSIC,
        ESM,
        CJS
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(NodeJSType, {
        {CLASSIC, "classic"},
        {ESM, "esm"},
        {CJS, "cjs"}
        });

    class NodeJS : public mod::LanguageModule<mod::Interpreter> {
    public:
        NodeJS() : LanguageModule<Interpreter>(L"C:\\Program Files\\nodejs\\node.exe"), m_file(m_esmFileName, std::fstream::in | std::fstream::out) {
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;
            StartInfo.RunInCmd = true;

            file::File fileInfo;
            if (FindNodeJSInstallation(&fileInfo)) {
                m_isNodeInstalled = true;
                m_nodeVersion = util::string_to_wstring(Run(L"- v"));
            }
        }

        NodeJS(std::wstring modulePath) : LanguageModule<Interpreter>(modulePath), m_file(m_esmFileName, std::fstream::in | std::fstream::out) {
            StartInfo.wFileName = modulePath;
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;
            StartInfo.RunInCmd = true;
        };
        ~NodeJS() {};

        void Initialize(app::AppWindow* mainWin) {
            m_AppWindow = mainWin;
            m_nodeEnv.push_back(Env{ L"NODE_OPTIONS", L"--import \"./langs/NodeJS/_dump_.mjs\"" });
            SetEnv();

            OnOutputReceived = std::bind(&NodeJS::HandleOutputReceived, this, std::placeholders::_1);

            OnErrorReceived = std::bind(&NodeJS::HandleErrorReceived, this, std::placeholders::_1);

            m_AppWindow->AddWebMessageHandler(std::bind(&NodeJS::HandleWebMessage, this, std::placeholders::_1));

            InitProcess();
        };

        void Invoke() {
            StartInfo.wCommandLine = std::format(L"{0} {1}", StartInfo.wFileName, *m_activeFileName);
            OnProcessExited = [this]() -> void {
                EndOutputRead();
                EndErrorRead();
            };
            BeginOutputRead();
            BeginErrorRead();
            Start();
        };

        std::string mod::IBaseModule::Run(std::wstring cmd) {
            StartInfo.wCommandLine = std::format(L"{0} {1}", StartInfo.wFileName, cmd);
            Start();

            WaitForExit();

            std::string ret;
            ReadOutput(&ret);

            return ret;
        }

        void SyncFileContent(std::wstring content) {
            m_file.wWriteFile(content);
        };

        void SyncFileContent(std::string content) {
            m_file.WriteFile(content);
        };

        void SetNodeType(NodeJSType type) {
            m_type = type;
        };

        void AddNodeOption(std::wstring option) {
            if (!SetEnvironmentVariable(L"NODE_OPTIONS", option.c_str())) {
                error::PrintError(L"SetEnvironmentVariable");
            }
        };

        bool GetInitialFileContent(std::string* ret) {
            return m_file.ReadFile(ret);
        };

        std::wstring HandleWebMessage(app::Message* msg) {
            switch (msg->cmd) {
                case app::AppCommand::INITIALIZE:
                    msg->respond = true;
                    GetInitialFileContent(&msg->message);
                    break;
                case app::AppCommand::CONFIG:
                    break;
                case app::AppCommand::CODESYNC:
                    SyncFileContent(msg->message);
                    break;
                case app::AppCommand::INVOKE:
                    Invoke();
                    break;
            }
            return util::string_to_wstring(json(*msg).dump());
        }

        void HandleOutputReceived(std::string ret) {
            app::Message* response = new app::Message{ app::AppCommand::RESULT, app::ResultType::ISSUCCESS };
            response->message = ret;
            PostMessage(m_AppWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(response), NULL);
        }

        void HandleErrorReceived(std::string ret) {
            app::Message* response = new app::Message{ app::AppCommand::RESULT, app::ResultType::ISERROR };
            response->error = ret;
            PostMessage(m_AppWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(response), NULL);
        }

    private:
        bool m_isNodeInstalled = false;
        std::wstring m_nodeVersion;
        NodeJSType m_type{ ESM };
        std::wstring m_esmFileName{ L"langs\\NodeJS\\_eval_.mjs" };
        std::wstring m_cjsFileName{ L"langs\\NodeJS\\_eval_.cjs" };
        std::wstring m_clFileName{ L"langs\\NodeJS\\_eval_.js" };
        std::wstring* m_activeFileName = &m_esmFileName;
        file::FileHandler m_file;
        file::FileFinder m_fileFinder;

        std::vector<Env> m_nodeEnv;

        npm::NPM m_npm;

        void SetEnv() {
            for (auto it = m_nodeEnv.begin(); it < m_nodeEnv.end(); it++) {
                if (!SetEnvironmentVariable(it->key.c_str(), it->value.c_str())) {
                    error::PrintError(L"SetEnvironmentVariable");
                }
            }
        };

        bool FindNodeJSInstallation(file::File* file) {
            return m_fileFinder.SearchFile(L"node.exe", file);
        }
    };
}

#endif