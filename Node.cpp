#ifndef _SCRIPT_PAD_NODE_CPP
#define _SCRIPT_PAD_NODE_CPP

#include "Node.h"

namespace scriptpad {

    Nodejs::Nodejs() : LanguageModule<Interpreter>(L"C:\\Program Files\\nodejs\\node.exe"), m_file(m_esmFileName, std::fstream::in | std::fstream::out) {
        if (m_bIsModuleInstalled) {
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;

            m_bRunAsModule = true;
            m_wszModuleVersion = scriptpad::str_to_wstr(Run(L"-v"));
        }
    }

    Nodejs::Nodejs(std::wstring modulePath) : LanguageModule<Interpreter>(modulePath), m_file(m_esmFileName, std::fstream::in | std::fstream::out) {
        if (m_bIsModuleInstalled) {
            StartInfo.wFileName = modulePath;
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;
            
            m_bRunAsModule = false;
            m_wszModuleVersion = scriptpad::str_to_wstr(Run(L"-v"));
        }
    };
    Nodejs::~Nodejs() {};

    void Nodejs::Initialize(scriptpad::AppWindow* mainWin) {
        m_AppWindow = mainWin;
        m_nodeEnv.push_back(Env{ L"NODE_OPTIONS", L"--import \"./langs/NodeJS/_dump_.mjs\"" });
        SetEnv();

        OnOutputReceived = std::bind(&Nodejs::HandleOutputReceived, this, std::placeholders::_1);

        OnErrorReceived = std::bind(&Nodejs::HandleErrorReceived, this, std::placeholders::_1);

        m_AppWindow->AddWebMessageHandler(std::bind(&Nodejs::HandleWebMessage, this, std::placeholders::_1));

        //InitProcess();
    };

    void Nodejs::Invoke() {
        StartInfo.wCommandLine = std::format(L"{0} {1}", StartInfo.wFileName, *m_activeFileName);
        OnProcessExited = [this]() -> void {
            EndOutputRead();
            EndErrorRead();
        };
        BeginOutputRead();
        BeginErrorRead();
        Start();
    };

    void Nodejs::SyncFileContent(std::wstring content) {
        m_file.wWriteFile(content);
    };

    void Nodejs::SyncFileContent(std::string content) {
        m_file.WriteFile(content);
    };

    void Nodejs::SetNodeType(NodeJSType type) {
        m_type = type;
    };

    void Nodejs::AddNodeOption(std::wstring option) {
        if (!SetEnvironmentVariable(L"NODE_OPTIONS", option.c_str())) {
            scriptpad::PrintError(L"SetEnvironmentVariable");
        }
    };

    bool Nodejs::GetInitialFileContent(std::string* ret) {
        return m_file.ReadFile(ret);
    };

    std::wstring Nodejs::HandleWebMessage(scriptpad::Message* msg) {
        switch (msg->cmd) {
            case scriptpad::AppCommand::INITIALIZE:
                msg->respond = true;
                GetInitialFileContent(&msg->message);
                break;
            case scriptpad::AppCommand::CONFIG:
                break;
            case scriptpad::AppCommand::CODESYNC:
                SyncFileContent(msg->message);
                break;
            case scriptpad::AppCommand::INVOKE:
                Invoke();
                break;
        }
        return scriptpad::str_to_wstr(json(*msg).dump());
    }

    void Nodejs::HandleOutputReceived(std::string ret) {
        scriptpad::Message* response = new scriptpad::Message{ scriptpad::AppCommand::RESULT, scriptpad::ResultType::ISSUCCESS };
        response->message = ret;
        PostMessage(m_AppWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(response), NULL);
    }

    void Nodejs::HandleErrorReceived(std::string ret) {
        scriptpad::Message* response = new scriptpad::Message{ scriptpad::AppCommand::RESULT, scriptpad::ResultType::ISERROR };
        response->error = ret;
        PostMessage(m_AppWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(response), NULL);
    }

    void Nodejs::SetEnv() {
        for (auto it = m_nodeEnv.begin(); it < m_nodeEnv.end(); it++) {
            if (!SetEnvironmentVariable(it->key.c_str(), it->value.c_str())) {
                scriptpad::PrintError(L"SetEnvironmentVariable");
            }
        }
    };

    bool Nodejs::FindNodeJSInstallation(scriptpad::File* file) {
        return m_fileFinder.SearchFile(L"node.exe", file);
    }

}   //namespace scriptpad

#endif  //_SCRIPT_PAD_NODE_JS_CPP