#pragma once

#ifndef _SCRIPT_PAD_MODULE_H
#define _SCRIPT_PAD_MODULE_H

#include "IModule.h"
#include "AppWindow.h"
#include "Process.h"
#include "File.h"
#include <utility>

namespace scriptpad {
    
    class BaseModule : public IModule, public scriptpad::Process {
    public:
        BaseModule(std::wstring);

        template<class...T>
        std::string Run(T&&...cmds) {
            if (m_bIsModuleInstalled) {
                if (m_bRunAsModule) {
                    StartInfo.RunInCmd = false;
                    StartInfo.wFileName = m_wszModulePath;
                    //StartInfo.wCommandLine = FormatCommandLine(std::forward<T>(cmds)...);
                }
                else {
                    StartInfo.RunInCmd = true;
                }
                StartInfo.wCommandLine = FormatCommandLine(m_wszModulePath, std::forward<T>(cmds)...);

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

    protected:
        bool m_bRunAsModule = true;
        std::wstring m_wszVersionArg = L"--version";
        std::wstring m_wszVersionArgAlias = L"-v";

        template<class... T>
        std::wstring FormatCommandLine(T&&... cmds) {
            return _FormatCommandLine_({ std::forward<std::wstring>(cmds)... });
        }

    private:
        scriptpad::FileFinder m_FFinder;

        bool _FindInstallation(std::wstring, scriptpad::File*);

        std::wstring _FormatCommandLine_(std::initializer_list<std::wstring>&& cmds) {
            std::wstring ret;
            for (auto it = cmds.begin(); it < cmds.end(); it++) {
                ret.append(*it);
                if (it < cmds.end() - 1) {
                    ret.append(L" ");
                }
            }

            return ret;
        }

    };

    template<IType U>
    class LanguageModule : public U, public BaseModule {
    public:
        LanguageModule(std::wstring path) : U(), BaseModule(path), m_AppWindow{ 0 } {};

        virtual void Initialize(scriptpad::AppWindow* appWin) = 0;

    protected:
        scriptpad::AppWindow* m_AppWindow;
    };


    class CmdModule : public BaseModule {
    public:
        CmdModule(std::wstring);
        CmdModule(std::wstring, std::wstring);

        template<class...T>
        std::string Run(T&&...cmds) {
            if (m_bIsModuleInstalled) {
                if (m_bRunAsModule) {
                    StartInfo.RunInCmd = false;
                    StartInfo.wCommandLine = FormatCommandLine(L"/C", m_wszAppName, std::forward<T>(cmds)...);
                }
                else {
                    StartInfo.RunInCmd = true;
                    StartInfo.wCommandLine = FormatCommandLine(m_wszModulePath, L"/C", m_wszAppName, std::forward<T>(cmds)...);
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

}   //namespace scriptpad

#endif  //_SCRIPT_PAD_MODULE_H