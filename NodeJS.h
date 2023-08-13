#pragma once

#ifndef _NODE_JS_H
    #define _NODE_JS_H
#include "Process.h"
#include "File.h"
namespace nodejs {
    class NodeJS {
    public:
        NodeJS(std::wstring appPath);
        ~NodeJS();
        void Invoke(std::wstring code, std::wstring* ret);
        void Invoke(std::wstring code, std::string* ret);

        void SyncFileContent(std::wstring);
        void SyncFileContent(std::string);

    private:
        std::string m_jsFileName{ "langs\\NodeJS\\_eval_.js" };
        std::wstring m_wjsFileName{ L"langs\\NodeJS\\_eval_.js" };
        file::FileHandler m_file;
        const std::wstring m_appPath{ L"C:\\Program Files\\nodejs\\node.exe" };
        const wchar_t* m_arg{ L"- e" };
        process::Process m_procNode;

        bool FindNodeJSInstallation();
    };
}

#endif