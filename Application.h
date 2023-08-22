#pragma once

#ifndef _SCRIPT_PAD_APPLICATION_H
#define _SCRIPT_PAD_APPLICATION_H

#include "AppWindow.h"
#include "ModuleHandler.h"
#include "Node.h"

namespace scriptpad {
    class Application {
    public:
        Application();
        void Initialize(AppWindow*);
        ModuleHandler ModuleHandler;
        Nodejs NodeJS;
        AppWindow* MainWindow;
    private:
        std::wstring m_cwd;
    };
}

#endif