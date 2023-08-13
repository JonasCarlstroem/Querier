#ifndef _NODE_JS_CPP
    #define _NODE_JS_CPP

#include "NodeJS.h"
#include <format>
#include <iomanip>
#include <filesystem>

using namespace process;

namespace nodejs {
    NodeJS::NodeJS(std::wstring appPath) : m_appPath(appPath), m_file(m_jsFileName) {
        m_procNode.StartInfo.wEnvironment = L"NODE_OPTIONS='--import \"./langs/NodeJS/_dump_.mjs\"'";
        m_procNode.StartInfo.wCommandLine = std::format(L"{0} {1}", m_appPath, m_wjsFileName);
        m_procNode.StartInfo.RedirectStdOutput = true;
    }

    NodeJS::~NodeJS() {
        
    }


    void NodeJS::Invoke(std::wstring code, std::wstring* ret) {
        if (m_procNode.Start()) {
            m_procNode.Read(ret);
        }
    }


    void NodeJS::Invoke(std::wstring code, std::string* ret) {
        m_procNode.StartInfo.wCommandLine = std::format(L"{0} {1}", L"-e", code);

        m_procNode.Start();
        std::string result;
    }


    void NodeJS::SyncFileContent(std::wstring content) {
        m_file.wWriteFile(content);
    }


    void NodeJS::SyncFileContent(std::string content) {
        m_file.WriteFile(content);
    }
}

#endif