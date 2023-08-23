#ifndef _SCRIPT_PAD_APPLICATION_CPP
#define _SCRIPT_PAD_APPLICATION_CPP

#include "Application.h"

namespace scriptpad {
    Application::Application(HINSTANCE hInst, int nCmdShow) : MainWindow(new AppWindow(hInst, nCmdShow)), ModuleHandler(MainWindow) {
        m_WorkingDirectory = Directory::wCurrentWorkingDirectory();

        auto tmp = Directory::wGetFiles(m_WorkingDirectory);
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
}

#endif