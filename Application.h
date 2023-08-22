#pragma once

#ifndef _SCRIPT_PAD_APPLICATION_H
#define _SCRIPT_PAD_APPLICATION_H

#include "AppWindow.h"
#include "ModuleHandler.h"
#include "Node.h"

namespace scriptpad {
    class Application {
    public:
        Application(HINSTANCE, int);

        int Start();

        AppWindow* MainWindow;
        ModuleHandler ModuleHandler;
    private:
        std::wstring m_cwd;

        int RunMessagePump();
    };
}

#endif