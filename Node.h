#pragma once

#ifndef _SCRIPT_PAD_NODE_JS_H
    #define _SCRIPT_PAD_NODE_JS_H
#include "interface/LanguageModule.h"
//#include "Npm.h"
#include "File.h"
#include "WebView2.h"
#include <functional>
#include <nlohmann/json.hpp>


//namespace scriptpad {
//    using json = nlohmann::json;
//
//    struct Env {
//        std::wstring key;
//        std::wstring value;
//    };
//
//    enum NodejsType {
//        CLASSIC,
//        ESM,
//        CJS
//    };
//
//    NLOHMANN_JSON_SERIALIZE_ENUM(NodejsType, {
//        {CLASSIC, "classic"},
//        {ESM, "esm"},
//        {CJS, "cjs"}
//        });
//
//    class Nodejs : public LanguageModule {
//    public:
//        NPM Npm;
//
//        Nodejs(NodejsType, std::wstring, std::wstring);
//        //Nodejs(std::wstring modulePath);
//        ~Nodejs();
//
//        void Initialize();
//        void Invoke();
//
//        void SetNodejsType(NodejsType type);
//        void AddNodeOption(std::wstring option);
//
//    private:
//        NodejsType m_type;
//        std::wstring m_sourceFileName;
//        std::map<NodejsType, std::wstring> m_mFileExtensions{
//            { ESM, L".mjs" },
//            { CJS, L".cjs" },
//            { CLASSIC, L".js" }
//        };
//
//        std::wstring get_activeSourceFile() {
//            return m_sourceFileName + m_mFileExtensions[m_type];
//        }
//
//        File m_fileFinder;
//
//        std::vector<Env> m_nodeEnv;
//
//        void SetEnv();
//
//        bool FindNodeJSInstallation(File* file);
//    };
//}   //namespace scriptpad

#endif  //_SCRIPT_PAD_NODE_JS_H