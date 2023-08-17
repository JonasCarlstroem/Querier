#pragma once

#ifndef _NODE_JS_H
    #define _NODE_JS_H
#include "Process.h"
#include "File.h"
#include "AppWindow.h"
#include "WebView2.h"
#include <functional>
#include <nlohmann/json.hpp>


namespace nodejs {
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


    class NodeJS : public process::Process {
    public:
    
        //rename to modulePath
        NodeJS(std::wstring appPath) : m_appPath(appPath), m_file(m_esmFileName, std::fstream::in | std::fstream::out), mainWindow(0) {};
        ~NodeJS() {};

        void Initialize(app::AppWindow* mainWin) {
            mainWindow = mainWin;
            m_nodeEnv.push_back(Env{ L"NODE_OPTIONS", L"--import \"./langs/NodeJS/_dump_.mjs\"" });
            SetEnv();
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;

            OnOutputReceived = [this](std::string ret) {
                app::Message* response = new app::Message{ app::AppCommand::RESULT, app::ResultType::ISSUCCESS };
                response->message = ret;
                PostMessage(mainWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(response), NULL);
            };

            OnErrorReceived = [this](std::string ret) {
                app::Message* response = new app::Message{ app::AppCommand::RESULT, app::ResultType::ISERROR };
                response->error = ret;
                PostMessage(mainWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(response), NULL);
            };

            mainWindow->AddWebMessageHandler(std::bind(&NodeJS::HandleWebMessage, this, std::placeholders::_1));
            //mainWindow->HandleWebMessage = std::bind(&NodeJS::HandleWebMessage, this, std::placeholders::_1);
        };

        void Invoke() {
            StartInfo.wCommandLine = std::format(L"{0} {1}", m_appPath, *m_activeFileName);
            OnProcessExited = [this]() -> void {
                EndOutputRead();
                EndErrorRead();
            };
            BeginOutputRead();
            BeginErrorRead();
            Start();
        };

        void SyncFileContent(std::wstring content) {
            m_file.wWriteFile(content);
        };

        void SyncFileContent(std::string content) {
            m_file.WriteFile(content);
        };

        void SetNodeType(NodeJSType) {
        
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

    private:
        app::AppWindow* mainWindow;

        NodeJSType m_type{ ESM };
        std::wstring m_esmFileName{ L"langs\\NodeJS\\_eval_.mjs" };
        std::wstring m_cjsFileName{ L"langs\\NodeJS\\_eval_.cjs" };
        std::wstring m_clFileName{ L"langs\\NodeJS\\_eval_.js" };
        std::wstring* m_activeFileName = &m_esmFileName;
        file::FileHandler m_file;
        std::wstring m_appPath{ L"C:\\Program Files\\nodejs\\node.exe" };

        std::vector<Env> m_nodeEnv;

        void SetEnv() {
            for (auto it = m_nodeEnv.begin(); it < m_nodeEnv.end(); it++) {
                if (!SetEnvironmentVariable(it->key.c_str(), it->value.c_str())) {
                    error::PrintError(L"SetEnvironmentVariable");
                }
            }
        };

        bool FindNodeJSInstallation();
    };
}

#endif