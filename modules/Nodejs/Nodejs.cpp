#include "pch.h"
#include "Nodejs.h"

Nodejs::Nodejs(NodejsType type, std::wstring sourceFile)
    : LanguageModule(L"C:\\Program Files\\nodejs\\node.exe", sourceFile, false), m_type(type) {
    m_sourceFileName = GetSourceFileName();
    if (m_bIsModuleInstalled) {
        m_bRunAsModule = false;

        SetNodejsType(ESM);
    }
}

Nodejs::~Nodejs() {};

void Nodejs::Initialize() {
    m_nodeEnv.push_back(Env{ L"NODE_OPTIONS", L"--import \"_lib_.mjs\"" });
    SetEnv();
};

bool Nodejs::SetNodejsType(NodejsType type) {
    SetSourceFileExtension(m_mFileExtensions[type]);
    m_type = type;
    return true;
};

bool Nodejs::AddNodeOption(std::wstring option) {
    if (!SetEnvironmentVariable(L"NODE_OPTIONS", option.c_str())) {
        return false;
    }
    return true;
};

bool Nodejs::SetEnv() {
    for (auto it = m_nodeEnv.begin(); it < m_nodeEnv.end(); it++) {
        if (!SetEnvironmentVariable(it->key.c_str(), it->value.c_str())) {
            return false;
        }
    }
    return true;
};