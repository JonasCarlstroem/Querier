#pragma once

#ifndef _SCRIPT_PAD_CMD_MODULE_H
#define _SCRIPT_PAD_CMD_MODULE_H

#include "../resources.h"
#include "BaseModule.h"

namespace scriptpad {
    class __declspec(dllexport) CmdModule : public BaseModule {
    public:
        CmdModule(std::wstring, std::wstring, bool);
        CmdModule(std::string, std::string, bool);
        virtual ~CmdModule();

        template<class...T>
        std::string Run(T&&...cmds) {
            if (m_bIsModuleInstalled) {
                if (m_bRunAsModule) {
                    StartInfo.RunInCmd = false;
                    StartInfo.CommandLine = FormatCommandLine("/C", m_szAppName, std::forward<T>(cmds)...);
                }
                else {
                    StartInfo.RunInCmd = true;
                    StartInfo.CommandLine = FormatCommandLine(m_szMainModulePath, "/C", m_szAppName, std::forward<T>(cmds)...);
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

        template<class...U>
        std::string wRun(U&&...cmds) {
            if (m_bIsModuleInstalled) {
                if (m_bRunAsModule) {
                    StartInfo.RunInCmd = false;
                    StartInfo.wCommandLine = wFormatCommandLine(L"/C", m_wszAppName, std::forward<U>(cmds)...);
                }
                else {
                    StartInfo.RunInCmd = true;
                    StartInfo.wCommandLine = wFormatCommandLine(m_wszMainModulePath, L"/C", m_wszAppName, std::forward<U>(cmds)...);
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
        std::string m_szAppName;
    };
}

#endif