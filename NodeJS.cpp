#ifndef _NODE_JS_CPP
    #define _NODE_JS_CPP

#include "NodeJS.h"
#include "Error.h"
#include <format>
#include <iomanip>
#include <filesystem>

using namespace process;

namespace nodejs {
    NodeJS::NodeJS(std::wstring appPath) : m_appPath(appPath), m_file(m_esmFileName, std::fstream::in | std::fstream::out) {
        
    }

    NodeJS::~NodeJS() {
        
    }

    void NodeJS::Initialize(app::AppWindow* mainWin) {
        mainWindow = mainWin;
        m_nodeEnv.push_back(Env{ L"NODE_OPTIONS", L"--import \"./langs/NodeJS/_dump_.mjs\"" });
        SetEnv();
        m_procNode.StartInfo.RedirectStdOutput = true;
    }

    void NodeJS::Invoke(std::wstring* ret) {
        m_procNode.StartInfo.wCommandLine = std::format(L"{0} {1}", m_appPath, *m_activeFileName);
        if (m_procNode.Start()) {
            m_procNode.Read(ret);
        }
    }


    void NodeJS::Invoke(std::string* ret) {
        m_procNode.StartInfo.wCommandLine = std::format(L"{0} {1}", m_appPath, *m_activeFileName);
        m_procNode.Start();
    }


    void NodeJS::SyncFileContent(std::wstring content) {
        m_file.wWriteFile(content);
    }


    void NodeJS::SyncFileContent(std::string content) {
        m_file.WriteFile(content);
    }

    void NodeJS::SetNodeType(NodeJSType type) {

    }

    void NodeJS::AddNodeOption(std::wstring option) {
        if (!SetEnvironmentVariable(L"NODE_OPTIONS", option.c_str())) {
            error::PrintError(L"SetEnvironmentVariable");
        }
    }

    bool NodeJS::GetInitialFileContent(std::string* ret) {
        return m_file.ReadFile(ret);
    }

    void NodeJS::SetOutputReceivedEvent(void (*OnOutputReceived)(std::string ret, std::thread*)) {
        m_procNode.OnOutputReceived = OnOutputReceived;
    }

    void NodeJS::SetEnv() {
        for (auto it = m_nodeEnv.begin(); it < m_nodeEnv.end(); it++) {
            if (!SetEnvironmentVariable(it->key.c_str(), it->value.c_str())) {
                error::PrintError(L"SetEnvironmentVariable");
            }
        }
    }
}

#endif