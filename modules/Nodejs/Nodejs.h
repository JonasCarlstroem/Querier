#pragma once

//#include "interface/LanguageModule.h"
//#include "Npm.h"

#include "../../interface/LanguageModule.h"
#include "Npm.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace scriptpad;

struct Env {
    std::wstring key;
    std::wstring value;
};

enum NodejsType {
    CLASSIC,
    ESM,
    CJS
};

NLOHMANN_JSON_SERIALIZE_ENUM(NodejsType, {
    {CLASSIC, "classic"},
    {ESM, "esm"},
    {CJS, "cjs"}
                             });

class Nodejs : public LanguageModule {
public:
    NPM Npm;

    Nodejs(NodejsType, std::string);
    Nodejs(NodejsType, std::wstring);
    ~Nodejs();

    void Initialize();

    bool SetNodejsType(NodejsType);
    bool wSetNodejsType(NodejsType);
    bool AddNodeOption(std::string);
    bool wAddNodeOption(std::wstring);

private:
    NodejsType m_type;
    std::wstring m_wSourceFileName;
    std::string m_SourceFileName;
    std::map<NodejsType, std::wstring> m_wmFileExtensions{
        { ESM, L".mjs" },
        { CJS, L".cjs" },
        { CLASSIC, L".js" }
    };

    std::map<NodejsType, std::string> m_mFileExtensions{
        { ESM, ".mjs" },
        { CJS, ".cjs" },
        { CLASSIC, ".js" }
    };

    std::vector<Env> m_nodeEnv;

    bool SetEnv();
};

extern "C" __declspec(dllexport) LanguageModule* CreateModule(std::string sourceFilePath = "") {
    LanguageModule* langModule = new Nodejs(ESM, sourceFilePath);

    return langModule;
}