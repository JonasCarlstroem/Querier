#pragma once

#ifndef _SCRIPT_PAD_NPM_H
#define _SCRIPT_PAD_NPM_H

#include "Module.h"
#include "File.h"
#include <utility>

namespace npm {
    class NPM : public mod::SubModule {
    public:
        NPM() : mod::SubModule(L"C:\\windows\\system32\\cmd.exe"), m_FileFinder() {
            StartInfo.wFileName = m_ModulePath;
            StartInfo.RedirectStdOutput = true;
            StartInfo.RedirectStdError = true;
            
            std::wstring tmp = FormatCommandLine(L"-v", L"-e");

            file::File fileInfo;
            if (FindNpmInstallation(&fileInfo)) {
                m_isNpmInstalled = true;
                m_npmVersion = util::string_to_wstring(Run(L"-v"));
            }

        }

        NPM(std::wstring modulePath) : mod::SubModule(modulePath) {
            StartInfo.wFileName = modulePath;
        }

        bool Install(std::wstring package) {
            if (m_isNpmInstalled) {
                //StartInfo.wCommandLine = std::format(L"{0} {1}", m_baseArg, )
            }
        }

        std::string Run(std::wstring arg) {
            if (m_isNpmInstalled) {
                StartInfo.wCommandLine = std::format(L"{0} {1}", m_baseArg, arg);
                Start();

                WaitForExit();

                std::string ret;
                ReadOutput(&ret);

                return ret;
            }
            throw std::exception("NPM is not installed on this computer");
        }

    private:
        bool m_isNpmInstalled = false;
        std::wstring m_npmVersion;
        const std::wstring m_baseArg = L"/C npm";
        const std::wstring m_localInstallArg = L"install";
        const std::wstring m_globalInstallArg = L"install -g";
        file::FileFinder m_FileFinder;

        template<class T>
        std::wstring _FormatCommandLine_(std::initializer_list<T>&& cmds) {
            std::wstring ret;
            for (auto it = cmds.begin(); it < cmds.end(); it++) {
                ret.append(*it);
                if (it < cmds.end() - 1) {
                    ret.append(L" ");
                }
            }

            return ret;
        }

        template<class... T>
        std::wstring FormatCommandLine(T&&... cmd) {
            return _FormatCommandLine_({ std::forward<T>(cmd)... });
        }

        bool FindNpmInstallation(file::File* pFile) {
            return m_FileFinder.SearchFile(L"npm", pFile);
        }
    };
}

#endif