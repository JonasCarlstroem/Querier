#ifndef _NODE_JS_CPP
    #define _NODE_JS_CPP

#include "NodeJS.h"
#include <format>

using namespace process;

namespace nodejs {
    NodeJS::NodeJS() : m_procStartInfo() {
        m_procStartInfo.wFileName = m_appPath;
        m_procStartInfo.RedirectStdOutput = true;
    }

    void NodeJS::Invoke(std::wstring code) {
        std::wstring finalCode = std::format(L"{0} {1}", m_arg, code);

        m_procNode = Process::Run(m_procStartInfo);
        std::wstring result;
        
    }

    void NodeJS::Invoke(std::string code) {
        std::string finalCode = std::format("{0} {1}", '-e', code);
        m_procStartInfo.CommandLine = finalCode;
        m_procNode = Process::Run(m_procStartInfo);
        std::string result;
    }
}

#endif