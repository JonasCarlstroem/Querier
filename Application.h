#pragma once

#ifndef _SCRIPT_PAD_APPLICATION_H
#define _SCRIPT_PAD_APPLICATION_H

#include "AppWindow.h"
#include "ModuleManager.h"
#include "QueryManager.h"
#include <ShlObj.h>

namespace querier {
    class Application {
    public:
        Application(HINSTANCE, int);

        int Start();

        AppWindow* MainWindow;
        ModuleManager ModuleManager;
        QueryManager QueryManager;
    private:
        path m_WorkingDirectory;
        path m_ModulesDirectory;
        path m_WorkspaceDirectory;

        int RunMessagePump();
        std::wstring HandleWebMessage(json data);
    };
}

#endif