#ifndef _SCRIPT_PAD_LANGUAGE_MODULE_CPP
#define _SCRIPT_PAD_LANGUAGE_MODULE_CPP

#include "LanguageModule.h"

namespace scriptpad {
    LanguageFilePaths::LanguageFilePaths(std::string mainModule, std::string sourceFile = "", std::string executableFile = "") : MainModule(mainModule) {
        if(!sourceFile.empty())
            set_SourceFile(sourceFile);

        if (!executableFile.empty())
            set_ExecutableFile(executableFile);
    }

    LanguageFilePaths::LanguageFilePaths(std::wstring wMainModule, std::wstring wSourceFile = L"", std::wstring wExecutableFile = L"") : wMainModule(wMainModule) {
        if (!wSourceFile.empty())
            set_wSourceFile(wSourceFile);

        if (!wExecutableFile.empty())
            set_wExecutableFile(wExecutableFile);
    }

    LanguageModule::LanguageModule(std::wstring wMainModule, std::wstring wSourceFile, std::wstring wVersionArg, bool runAsModule = true)
        : ILanguage(),
        BaseModule(wMainModule, wVersionArg, runAsModule),
        m_LanguageFilePaths{ wMainModule },
        m_File(wSourceFile) {
        m_LanguageFilePaths.set_wSourceFile(wSourceFile);
        m_wszModuleVersion = str_to_wstr(wRun(wVersionArg));
    }

    LanguageModule::LanguageModule(std::string mainModule, std::string sourceFile, std::string versionArg, bool runAsModule = true)
        : ILanguage(),
        BaseModule(mainModule, versionArg, runAsModule),
        m_LanguageFilePaths{ mainModule },
        m_File(sourceFile) {
        m_LanguageFilePaths.set_SourceFile(sourceFile);
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
                        StartInfo.wCommandLine = wFormatCommandLine(StartInfo.wFileName,
                                                                    m_LanguageFilePaths.get_wSourceFile());
                    }
                    else {
                        StartInfo.CommandLine = FormatCommandLine(StartInfo.FileName,
                                                                  m_LanguageFilePaths.get_SourceFile());
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


    void LanguageModule::SetSourceFile(std::string fileName, std::string extension) {
        m_LanguageFilePaths.set_SourceFile(fileName + extension);
    }

    void LanguageModule::wSetSourceFile(std::wstring fileName, std::wstring extension) {
        m_LanguageFilePaths.set_wSourceFile(fileName + extension);
    }


    std::string LanguageModule::GetSourceFile() {
        return m_LanguageFilePaths.get_SourceFile();
    }

    std::wstring LanguageModule::wGetSourceFile() {
        return m_LanguageFilePaths.get_wSourceFile();
    }


    void LanguageModule::SetSourceFileName(std::string name) {
        m_LanguageFilePaths.SourceFileName = name;
    }

    void LanguageModule::wSetSourceFileName(std::wstring name) {
        m_LanguageFilePaths.wSourceFileName = name;
    }


    std::string LanguageModule::GetSourceFileName() {
        return m_LanguageFilePaths.SourceFileName;
    }

    std::wstring LanguageModule::wGetSourceFileName() {
        return m_LanguageFilePaths.wSourceFileName;
    }


    void LanguageModule::SetSourceFileExtension(std::string extension) {
        m_LanguageFilePaths.SourceFileExtension = extension;
    }

    void LanguageModule::wSetSourceFileExtension(std::wstring extension) {
        m_LanguageFilePaths.wSourceFileExtension = extension;
    }


    std::string LanguageModule::GetSourceFileExtension() {
        return m_LanguageFilePaths.SourceFileExtension;
    }
    std::wstring LanguageModule::wGetSourceFileExtension() {
        return m_LanguageFilePaths.wSourceFileExtension;
    }
}

#endif