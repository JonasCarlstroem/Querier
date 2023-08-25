#pragma once

#ifndef _SCRIPT_PAD_BASE_MODULE_H
#define _SCRIPT_PAD_BASE_MODULE_H

#include "IModule.h"
#include "../Process.h"
#include "../File.h"

namespace scriptpad {
    class BaseModule : public IModule, public Process {
    public:
        BaseModule();
        BaseModule(std::wstring, std::wstring, bool);
        BaseModule(std::string, std::string, bool);

        template<class...U>
        std::string Run(U&&...cmds) {
            if (m_bIsModuleInstalled) {
                if (m_bRunAsModule) {
                    StartInfo.RunInCmd = false;
                    StartInfo.FileName = m_szMainModulePath;
                }
                else {
                    StartInfo.RunInCmd = true;
                }
                StartInfo.CommandLine = FormatCommandLine(m_szMainModulePath, std::forward<U>(cmds)...);

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

        template<class...T>
        std::string wRun(T&&...cmds) {
            if (m_bIsModuleInstalled) {
                if (m_bRunAsModule) {
                    StartInfo.RunInCmd = false;
                    StartInfo.wFileName = m_wszMainModulePath;
                }
                else {
                    StartInfo.RunInCmd = true;
                }
                StartInfo.wCommandLine = wFormatCommandLine(m_wszMainModulePath, std::forward<T>(cmds)...);

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
        bool m_isUnicode;
        std::string m_szVersionArg = "--version";
        std::string m_szVersionArgAlias = "-v";
        std::wstring m_wszVersionArg = L"--version";
        std::wstring m_wszVersionArgAlias = L"-v";

        template<class... T>
        std::wstring wFormatCommandLine(T&&... cmds) {
            return _wFormatCommandLine_({ std::forward<T>(cmds)... });
        }

        template<class... U>
        std::string FormatCommandLine(U&&...cmds) {
            return _FormatCommandLine_({ std::forward<U>(cmds)... });
        }

    private:
        File m_FFinder;

        bool _FindInstallation(std::wstring, File*);
        bool _FindInstallation(path, File*);

        std::wstring _wFormatCommandLine_(std::initializer_list<std::wstring>&& cmds) {
            std::wstring ret;
            for (auto it = cmds.begin(); it < cmds.end(); it++) {
                ret.append(*it);
                if (it < cmds.end() - 1) {
                    ret.append(L" ");
                }
            }

            return ret;
        }

        std::string _FormatCommandLine_(std::initializer_list<std::string>&& cmds) {
            std::string ret;
            for (auto it = cmds.begin(); it < cmds.end(); it++) {
                ret.append(*it);
                if (it < cmds.end() - 1) {
                    ret.append(" ");
                }
            }

            return ret;
        }

    };
}

#endif