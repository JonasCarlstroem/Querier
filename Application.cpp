#ifndef _SCRIPT_PAD_APPLICATION_CPP
#define _SCRIPT_PAD_APPLICATION_CPP

#include "Application.h"

namespace querier {
    Application::Application(HINSTANCE hInst, int nCmdShow) : MainWindow(new AppWindow(hInst, nCmdShow)), ModuleManager(MainWindow), QueryManager(MainWindow, &ModuleManager) {
        MainWindow->AddWebMessageHandler(std::bind(&Application::HandleWebMessage, this, std::placeholders::_1));
        path specialFolder;
        if (Directory::GetSpecialFolder(FOLDERID_RoamingAppData, &specialFolder)) {
            path appDataFolder = specialFolder / path("Querier");
            if (!Directory::Exists(appDataFolder))
                Directory::Create(appDataFolder);

            if (Directory::SetCurrentWorkingDirectory(appDataFolder)) {
                m_WorkingDirectory = appDataFolder;
            }
        }
        else {
            m_WorkingDirectory = Directory::GetCurrentWorkingDirectory();
        }
        m_ModulesDirectory = m_WorkingDirectory / path(L"modules");
        if (!Directory::Exists(m_ModulesDirectory))
            Directory::Create(m_ModulesDirectory);

        m_WorkspaceDirectory = m_WorkingDirectory / path(L"workspace");
        if (!Directory::Exists(m_WorkspaceDirectory))
            Directory::Create(m_WorkspaceDirectory);

        ModuleManager.Initialize(m_WorkingDirectory, m_ModulesDirectory);
        QueryManager.Initialize(m_WorkingDirectory, m_WorkspaceDirectory);
    }

    int Application::Start() {
        if (MainWindow->Show())
            return RunMessagePump();

        return -1;
    }

    int Application::RunMessagePump() {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return (int)msg.wParam;
    }

    std::wstring Application::HandleWebMessage(Message* msg) {
        switch (msg->cmd) {
            case AppCommand::INITIALIZE:
                msg->respond = true;
                ModuleManager.ActiveModule->GetFileContent(&msg->message);
                break;
            case AppCommand::CONFIG:
                break;
            case AppCommand::CODESYNC:
                ModuleManager.ActiveModule->SetFileContent(msg->message);
                break;
            case AppCommand::INVOKE:
                ModuleManager.ActiveModule->Invoke();
                break;
        }
        return str_to_wstr(json(*msg).dump());
    }
}

#endif