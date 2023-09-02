#include "pch.h"
#include "Nodejs.h"

Nodejs::Nodejs(NodejsType type, std::string libFilePath)
    : LanguageModule("C:\\Program Files\\nodejs\\node.exe", "-v", false), m_type(type) {
    m_SourceFileName = GetSourceFile();
    if (m_bIsModuleInstalled) {
        m_bRunAsModule = false;
        m_isUnicode = false;

        SetNodejsType(ESM);

        //m_nodeEnv.push_back(Env{ L"NODE_OPTIONS"})
    }
}

Nodejs::Nodejs(NodejsType type, std::wstring libFilePath)
    : LanguageModule(L"C:\\Program Files\\nodejs\\node.exe", L"-v", false), m_type(type) {
    m_wSourceFileName = wGetSourceFile();
    if (m_bIsModuleInstalled) {
        m_bRunAsModule = false;
        m_isUnicode = true;

        SetNodejsType(ESM);
        wAddImportLibrary(libFilePath);
    }
}

Nodejs::~Nodejs() {};

void Nodejs::Initialize() {
    m_nodeEnv.push_back(Env{ L"NODE_OPTIONS", L"--import \"_lib_.mjs\"" });
    SetEnv();
};

bool Nodejs::SetNodejsType(NodejsType type) {
    m_type = type;
    return true;
};

bool Nodejs::wSetNodejsType(NodejsType type) {
    wSetSourceFile(m_wSourceFileName + m_wmFileExtensions[type]);
    m_type = type;
    return true;
}

bool Nodejs::AddNodeOption(std::string option) {
    if (!SetEnvironmentVariableA("NODE_OPTIONS", option.c_str()))
        return false;
    return true;
}

bool Nodejs::wAddNodeOption(std::wstring option) {
    m_nodeEnv.push_back(Env{ L"NODE_OPTIONS", option });
    if (!SetEnvironmentVariableW(L"NODE_OPTIONS", option.c_str())) 
        return false;
    return true;
};

bool Nodejs::wAddImportLibrary(std::wstring library) {
    convert_backslashes(&library);
    Env opt{ L"NODE_OPTIONS", std::format(L"--import \"file:///{0}\"", library) };
    if (!SetEnvironmentVariableW(opt.key.c_str(), opt.value.c_str())) {
        return false;
    }
    m_nodeEnv.push_back(opt);
    return true;
}

bool Nodejs::SetEnv() {
    for (auto it = m_nodeEnv.begin(); it < m_nodeEnv.end(); it++) {
        if (!SetEnvironmentVariable(it->key.c_str(), it->value.c_str())) {
            return false;
        }
    }
    return true;
};