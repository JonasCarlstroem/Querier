#ifndef _SCRIPT_PAD_LANGUAGE_MODULE_CPP
#define _SCRIPT_PAD_LANGUAGE_MODULE_CPP

#include "LanguageModule.h"

namespace scriptpad {
    LanguageModule::LanguageModule(std::wstring mainModule, std::wstring sourceFile, std::wstring versionArg, bool runAsModule)
        : ILanguage(),
        BaseModule(mainModule, versionArg, runAsModule),
        m_LanguageFilePaths{ mainModule },
        m_File(sourceFile) {

        m_LanguageFilePaths.set_SourceFile(sourceFile);
        m_wszModuleVersion = str_to_wstr(Run(versionArg));
    }

    LanguageModule::~LanguageModule() {

    }

    void LanguageModule::Invoke() {
        if (m_bIsModuleInstalled) {
            switch (m_LanguageType) {
                case Interpreter:
                {
                    StartInfo.wCommandLine = FormatCommandLine(StartInfo.wFileName,
                                                               m_LanguageFilePaths.get_SourceFile());

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

    void LanguageModule::SetSourceFile(std::wstring fileName, std::wstring extension) {
        m_LanguageFilePaths.set_SourceFile(fileName + extension);
    }

    std::wstring LanguageModule::GetSourceFile() {
        return m_LanguageFilePaths.get_SourceFile();
    }

    void LanguageModule::SetSourceFileName(std::wstring name) {
        m_LanguageFilePaths.SourceFileName = name;
    }

    std::wstring LanguageModule::GetSourceFileName() {
        return m_LanguageFilePaths.SourceFileName;
    }

    void LanguageModule::SetSourceFileExtension(std::wstring extension) {
        m_LanguageFilePaths.SourceFileExtension = extension;
    }

    std::wstring LanguageModule::GetSourceFileExtension() {
        return m_LanguageFilePaths.SourceFileExtension;
    }
}

#endif