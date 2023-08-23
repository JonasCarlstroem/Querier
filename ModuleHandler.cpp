#ifndef _SCRIPT_PAD_MODULE_HANDLER_CPP
#define _SCRIPT_PAD_MODULE_HANDLER_CPP

#include "ModuleHandler.h"
//#include "Node.h"

namespace scriptpad {
    ModuleHandler::ModuleHandler(AppWindow* mainWin) {
        m_WorkingDirectory = Directory::wCurrentWorkingDirectory();
        m_ModulesDirectory = m_WorkingDirectory / path(L"modules");
        m_WorkspaceDirectory = m_WorkingDirectory / path(L"workspace");
        m_ModulePaths = Directory::wGetFiles(m_ModulesDirectory);

        m_MainWindow = mainWin;
        m_MainWindow->AddWebMessageHandler(std::bind(&ModuleHandler::HandleWebMessage, this, std::placeholders::_1));
        
        /*if (Modules.count(L"nodejs") == 0)
            Modules.insert({ L"nodejs", new Nodejs(ESM, L"langs\\NodeJS\\_eval_", L".mjs")});

        Module = Modules[L"nodejs"];
        Module->OnOutputReceived = std::bind(&ModuleHandler::HandleOutputReceived, this, std::placeholders::_1);
        Module->OnErrorReceived = std::bind(&ModuleHandler::HandleErrorReceived, this, std::placeholders::_1);*/
        //Module->Initialize(mainWin);
    }

    std::wstring ModuleHandler::HandleWebMessage(Message* msg) {
        switch (msg->cmd) {
            case AppCommand::INITIALIZE:
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
}

#endif