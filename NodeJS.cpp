#ifndef _NODE_JS_CPP
    #define _NODE_JS_CPP

#include "NodeJS.h"
#include <format>
#include <iomanip>
#include <filesystem>

using namespace process;

namespace nodejs {
    NodeJS::NodeJS(std::wstring appPath) : m_appPath(appPath), m_procStartInfo() {
        m_procStartInfo.wFileName = m_appPath;
        m_procStartInfo.RedirectStdOutput = true;
    }


    void NodeJS::Invoke(std::wstring code, std::wstring* ret) {
        m_procStartInfo.wCommandLine  = std::format(L"{0} '{1}'", L"-e", code);
         
        m_procNode = Process::Run(m_procStartInfo);
        m_procNode->wRead(ret);
        
    }


    void NodeJS::Invoke(std::wstring code, std::string* ret) {
        std::wstring finalCode = std::format(L"{0} {1}", L"-e", code);

        m_procStartInfo.wCommandLine = finalCode;
        m_procNode = Process::Run(m_procStartInfo);
        std::string result;
    }
}

#endif