#pragma once

#ifndef _SCRIPT_PAD_NODE_JS_H
    #define _SCRIPT_PAD_NODE_JS_H
#include "LanguageModule.h"
#include "Npm.h"
#include "File.h"
#include "WebView2.h"
#include <functional>
#include <nlohmann/json.hpp>


namespace scriptpad {
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
        });

    class Nodejs : public LanguageModule<Interpreter> {
    public:
        NPM Npm;

        Nodejs();
        Nodejs(std::wstring modulePath);
        ~Nodejs();

        void Initialize(AppWindow* mainWin);
        void Invoke();
        void SyncFileContent(std::wstring content);
        void SyncFileContent(std::string content);
        void SetNodeType(NodeJSType type);
        void AddNodeOption(std::wstring option);
        bool GetInitialFileContent(std::string* ret);
        std::wstring HandleWebMessage(Message* msg);
        void HandleOutputReceived(std::string ret);
        void HandleErrorReceived(std::string ret);

    private:
        NodeJSType m_type{ ESM };
        std::wstring m_esmFileName{ L"langs\\NodeJS\\_eval_.mjs" };
        std::wstring m_cjsFileName{ L"langs\\NodeJS\\_eval_.cjs" };
        std::wstring m_clFileName{ L"langs\\NodeJS\\_eval_.js" };
        std::wstring* m_activeFileName = &m_esmFileName;
        FileHandler m_file;
        FileFinder m_fileFinder;

        std::vector<Env> m_nodeEnv;

        void SetEnv();

        bool FindNodeJSInstallation(File* file);
    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_NODE_JS_H