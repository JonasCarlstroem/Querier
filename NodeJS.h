#pragma once

#ifndef _NODE_JS_H
    #define _NODE_JS_H
#include "Process.h"
namespace nodejs {
    class NodeJS {
    public:
        NodeJS();
        void Invoke(std::wstring code);
        void Invoke(std::string code);

    private:
        const std::wstring m_appPath{ L"C:\\Program Files\\nodejs\\node.exe" };
        const wchar_t m_arg{ L'-e' };
        process::Process* m_procNode{ 0 };
        process::ProcessStartInfo m_procStartInfo{ 0 };
    };
}

#endif