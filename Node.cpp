#ifndef _SCRIPT_PAD_NODE_CPP
#define _SCRIPT_PAD_NODE_CPP

#include "Node.h"

namespace scriptpad {

    //Nodejs::Nodejs(NodejsType type, std::wstring sourceFileName, std::wstring fileExtension) 
    //    : LanguageModule(LanguageFilePaths{ L"C:\\Program Files\\nodejs\\node.exe", sourceFileName, fileExtension }), m_type(type), m_sourceFileName(sourceFileName) {
    //    if (m_bIsModuleInstalled) {
    //        StartInfo.wFileName = m_LanguageFilePaths.MainModule;
    //        StartInfo.RedirectStdOutput = true;
    //        StartInfo.RedirectStdError = true;
    //        m_bRunAsModule = false;

    //        m_wszModuleVersion = str_to_wstr(Run(L"-v"));
    //        SetNodejsType(ESM);
    //    }
    //}

    //Nodejs::~Nodejs() {};

    //void Nodejs::Initialize() {
    //    //m_AppWindow = mainWin;
    //    m_nodeEnv.push_back(Env{ L"NODE_OPTIONS", L"--import \"./langs/NodeJS/_dump_.mjs\"" });
    //    SetEnv();
    //};

    //void Nodejs::Invoke() {
    //    StartInfo.wCommandLine = FormatCommandLine(StartInfo.wFileName, 
    //                                               get_activeSourceFile());

    //    OnProcessExited = [this]() -> void {
    //        EndOutputRead();
    //        EndErrorRead();
    //    };
    //    BeginOutputRead();
    //    BeginErrorRead();
    //    Start();
    //};

    //void Nodejs::SetNodejsType(NodejsType type) {
    //    m_LanguageFilePaths.SourceFileExtension = m_mFileExtensions[type];
    //    m_type = type;
    //};

    //void Nodejs::AddNodeOption(std::wstring option) {
    //    if (!SetEnvironmentVariable(L"NODE_OPTIONS", option.c_str())) {
    //        scriptpad::PrintError(L"SetEnvironmentVariable");
    //    }
    //};

    //void Nodejs::SetEnv() {
    //    for (auto it = m_nodeEnv.begin(); it < m_nodeEnv.end(); it++) {
    //        if (!SetEnvironmentVariable(it->key.c_str(), it->value.c_str())) {
    //            scriptpad::PrintError(L"SetEnvironmentVariable");
    //        }
    //    }
    //};

    //bool Nodejs::FindNodeJSInstallation(scriptpad::File* file) {
    //    return m_fileFinder.SearchFile(L"node.exe", file);
    //}

}   //namespace scriptpad

#endif  //_SCRIPT_PAD_NODE_JS_CPP