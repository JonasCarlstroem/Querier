#ifndef _SCRIPT_PAD_APPLICATION_CPP
#define _SCRIPT_PAD_APPLICATION_CPP

#include "Application.h"

namespace scriptpad {
    Application::Application() : NodeJS(), MainWindow{ 0 } {
        wchar_t buffer[MAX_PATH];
        if (GetCurrentDirectory(MAX_PATH, buffer)) {
            print_msg(buffer);
        }
    }

    void Application::Initialize(AppWindow* win) {
        MainWindow = win;
    }
}

#endif