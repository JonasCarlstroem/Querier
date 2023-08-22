#ifndef _SCRIPT_PAD_MODULE_HANDLER_CPP
#define _SCRIPT_PAD_MODULE_HANDLER_CPP

#include "ModuleHandler.h"
#include "Node.h"

namespace scriptpad {
    ModuleHandler::ModuleHandler(AppWindow* mainWin) {
        m_MainWindow = mainWin;
        m_MainWindow->AddWebMessageHandler(std::bind(&ModuleHandler::HandleWebMessage, this, std::placeholders::_1));
        
        if (Modules.count(L"nodejs") == 0)
            Modules.insert({ L"nodejs", new Nodejs(ESM, L"langs\\NodeJS\\_eval_", L".mjs")});

        Module = Modules[L"nodejs"];
        Module->OnOutputReceived = std::bind(&ModuleHandler::HandleOutputReceived, this, std::placeholders::_1);
        Module->OnErrorReceived = std::bind(&ModuleHandler::HandleErrorReceived, this, std::placeholders::_1);
        Module->Initialize(mainWin);
    }

    std::wstring ModuleHandler::HandleWebMessage(scriptpad::Message* msg) {
        switch (msg->cmd) {
            case scriptpad::AppCommand::INITIALIZE:
                msg->respond = true;
                Module->GetFileContent(&msg->message);
                break;
            case scriptpad::AppCommand::CONFIG:
                break;
            case scriptpad::AppCommand::CODESYNC:
                Module->SetFileContent(msg->message);
                break;
            case scriptpad::AppCommand::INVOKE:
                Module->Invoke();
                break;
        }
        return scriptpad::str_to_wstr(json(*msg).dump());
    }

    void ModuleHandler::HandleOutputReceived(std::string ret) {
        scriptpad::Message* response = new scriptpad::Message{ scriptpad::AppCommand::RESULT, scriptpad::ResultType::ISSUCCESS };
        response->message = ret;
        PostMessage(m_MainWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(response), NULL);
    }

    void ModuleHandler::HandleErrorReceived(std::string ret) {
        scriptpad::Message* response = new scriptpad::Message{ scriptpad::AppCommand::RESULT, scriptpad::ResultType::ISERROR };
        response->error = ret;
        PostMessage(m_MainWindow->get_MainWindow(), WM_WEBVIEW, reinterpret_cast<WPARAM>(response), NULL);
    }
}

#endif