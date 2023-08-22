#ifndef _SCRIPT_PAD_APPLICATION_CPP
#define _SCRIPT_PAD_APPLICATION_CPP

#include "Application.h"

namespace scriptpad {
    Application::Application(HINSTANCE hInst, int nCmdShow) : MainWindow(new AppWindow(hInst, nCmdShow)), ModuleHandler(MainWindow) {
        wchar_t buffer[MAX_PATH] = { 0 };
        if (GetCurrentDirectory(MAX_PATH, buffer)) {
            print_msg(buffer);
            m_cwd = buffer;
        }

        wchar_t mbuffer[MAX_PATH] = { 0 };
        if (GetModuleFileName(NULL, mbuffer, MAX_PATH)) {
            print_msg(mbuffer);
        }
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