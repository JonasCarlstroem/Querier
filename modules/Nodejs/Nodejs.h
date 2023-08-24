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

    Nodejs(NodejsType, std::wstring);
    ~Nodejs();

    void Initialize();

    bool SetNodejsType(NodejsType type);
    bool AddNodeOption(std::wstring option);

private:
    NodejsType m_type;
    std::wstring m_sourceFileName;
    std::map<NodejsType, std::wstring> m_mFileExtensions{
        { ESM, L".mjs" },
        { CJS, L".cjs" },
        { CLASSIC, L".js" }
    };

    std::vector<Env> m_nodeEnv;

    bool SetEnv();
};

extern "C" __declspec(dllexport) LanguageModule * CreateModule() {
    LanguageModule* langModule = new Nodejs(ESM, L"_eval_.mjs");

    return langModule;
}