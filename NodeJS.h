#pragma once

#ifndef _NODE_JS_H
    #define _NODE_JS_H
#include "Process.h"
#include "File.h"
#include "AppWindow.h"
#include "WebView2.h"
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
        NodeJS(std::wstring appPath) : m_appPath(appPath), m_file(m_esmFileName, std::fstream::in | std::fstream::out), mainWindow(0) {};
        ~NodeJS() {};

        void Initialize(app::AppWindow* mainWin) {
            mainWindow = mainWin;
            m_nodeEnv.push_back(Env{ L"NODE_OPTIONS", L"--import \"./langs/NodeJS/_dump_.mjs\"" });
            SetEnv();
            StartInfo.RedirectStdOutput = true;

            OnOutputReceived = [this](std::string ret) {
                app::Message* response = new app::Message{ app::AppCommand::RESULT, ret };
                PostMessage(mainWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(response), NULL);
            };
        };

        void Invoke() {
            StartInfo.wCommandLine = std::format(L"{0} {1}", m_appPath, *m_activeFileName);
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