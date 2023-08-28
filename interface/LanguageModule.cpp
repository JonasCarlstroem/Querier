#ifndef _SCRIPT_PAD_LANGUAGE_MODULE_CPP
#define _SCRIPT_PAD_LANGUAGE_MODULE_CPP

#include "LanguageModule.h"

namespace querier {
    LanguageFilePaths::LanguageFilePaths(std::string mainModule, std::string sourceFile = "", std::string executableFile = "") : MainModule(mainModule) {
        if(!sourceFile.empty())
            SourceFile = sourceFile;

        if (!executableFile.empty())
            ExecutableFile = executableFile;
    }

    LanguageFilePaths::LanguageFilePaths(std::wstring _wMainModule, std::wstring _wSourceFile = L"", std::wstring _wExecutableFile = L"") : wMainModule(_wMainModule) {
        if (!_wSourceFile.empty())
            wSourceFile = _wSourceFile;

        if (!_wExecutableFile.empty())
            wExecutableFile = _wExecutableFile;
    }

    LanguageModule::LanguageModule(std::wstring wMainModule, std::wstring wSourceFile, std::wstring wVersionArg, bool runAsModule = true)
        : ILanguage(),
        BaseModule(wMainModule, wVersionArg, runAsModule),
        m_LanguageFilePaths{ wMainModule },
        m_File(wSourceFile) {
        m_LanguageFilePaths.wSourceFile = wSourceFile;
        m_wszModuleVersion = str_to_wstr(wRun(wVersionArg));
    }

    LanguageModule::LanguageModule(std::string mainModule, std::string sourceFile, std::string versionArg, bool runAsModule = true)
        : ILanguage(),
        BaseModule(mainModule, versionArg, runAsModule),
        m_LanguageFilePaths{ mainModule },
        m_File(sourceFile) {
        m_LanguageFilePaths.SourceFile = sourceFile;
        m_szModuleVersion = Run(versionArg);
    }

    LanguageModule::~LanguageModule() {

    }

    void LanguageModule::Invoke() {
        if (m_bIsModuleInstalled) {
            switch (m_LanguageType) {
                case Interpreter:
                {
                    if (m_isUnicode) {
                        StartInfo.wCommandLine = wFormatCommandLine(m_LanguageFilePaths.wMainModule,
                                                                    m_LanguageFilePaths.wSourceFile);
                    }
                    else {
                        StartInfo.CommandLine = FormatCommandLine(m_LanguageFilePaths.MainModule,
                                                                  m_LanguageFilePaths.SourceFile);
                    }

                    OnProcessExited = [this]() -> void {
                        EndOutputRead();
                        EndErrorRead();
                    };
                    BeginOutputRead();
                    BeginErrorRead();
                    Start();
                    break;
                }
                case Compiler:
                    //Do compiler stuff
                    break;
                default:
                    break;
            }
        }
    }

    bool LanguageModule::GetFileContent(std::string* ret) {
        return m_File.Read(ret);
    };

    void LanguageModule::SetFileContent(std::string content) {
        m_File.WriteFile(content);
    };


    void LanguageModule::SetSourceFile(std::string fileName) {
        m_LanguageFilePaths.SourceFile = fileName;
    }

    void LanguageModule::wSetSourceFile(std::wstring fileName) {
        m_LanguageFilePaths.wSourceFile = fileName;
    }


    std::string LanguageModule::GetSourceFile() {
        return m_LanguageFilePaths.SourceFile;
    }

    std::wstring LanguageModule::wGetSourceFile() {
        return m_LanguageFilePaths.wSourceFile;
    }
}

#endif