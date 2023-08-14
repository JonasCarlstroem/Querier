#pragma once

#ifndef _NODE_JS_H
    #define _NODE_JS_H
#include "Process.h"
#include "File.h"
#include "WebView2.h"
#include <nlohmann/json.hpp>


namespace nodejs {
    using json = nlohmann::json;

    struct Env {
        std::wstring key;
        std::wstring value;
    };

    enum NodeJSType {
        CLASSIC,
        ESM,
        CJS
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(NodeJSType, {
        {CLASSIC, "classic"},
        {ESM, "esm"},
        {CJS, "cjs"}
        })

    class NodeJS {
    public:
        NodeJS(std::wstring appPath);
        ~NodeJS();
        void Invoke(std::wstring* ret);
        void Invoke(std::string* ret);

        void SyncFileContent(std::wstring);
        void SyncFileContent(std::string);

        void SetNodeType(NodeJSType);

        void AddNodeOption(std::wstring);

        bool GetInitialFileContent(std::string*);

        void SetOutputReceivedEvent(void (*OnOutputReceived)(std::string ret, std::thread*));

    private:
        NodeJSType m_type{ ESM };
        std::wstring m_esmFileName{ L"langs\\NodeJS\\_eval_.mjs" };
        std::wstring m_cjsFileName{ L"langs\\NodeJS\\_eval_.cjs" };
        std::wstring m_clFileName{ L"langs\\NodeJS\\_eval_.js" };
        std::wstring* m_activeFileName = &m_esmFileName;
        file::FileHandler m_file;
        std::wstring m_appPath{ L"C:\\Program Files\\nodejs\\node.exe" };
        process::Process m_procNode;

        std::vector<Env> m_nodeEnv;

        void SetEnv();
        bool FindNodeJSInstallation();
    };
}

#endif