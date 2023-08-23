#pragma once

#ifndef _SCRIPT_PAD_CMD_MODULE_H
#define _SCRIPT_PAD_CMD_MODULE_H

#include "../resources.h"
#include "BaseModule.h"

namespace scriptpad {
    class CmdModule : public BaseModule {
    public:
        CmdModule(std::wstring);
        CmdModule(std::wstring, std::wstring);
        virtual ~CmdModule();

        template<class...T>
        std::string Run(T&&...cmds) {
            if (m_bIsModuleInstalled) {
                if (m_bRunAsModule) {
                    StartInfo.RunInCmd = false;
                    StartInfo.wCommandLine = FormatCommandLine(L"/C", m_wszAppName, std::forward<T>(cmds)...);
                }
                else {
                    StartInfo.RunInCmd = true;
                    StartInfo.wCommandLine = FormatCommandLine(m_wszMainModulePath, L"/C", m_wszAppName, std::forward<T>(cmds)...);
                }

                if (Start()) {
                    WaitForExit();

                    std::string ret;
                    ReadOutput(&ret);

                    return ret;
                }
                else {
                    throw std::exception("Error creating process");
                }
            }
            throw std::exception("Module is not installed");
        }

    private:
        std::wstring m_wszAppName;
    };
}

#endif