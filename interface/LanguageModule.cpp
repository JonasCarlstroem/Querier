#ifndef _SCRIPT_PAD_LANGUAGE_MODULE_CPP
#define _SCRIPT_PAD_LANGUAGE_MODULE_CPP

#include "LanguageModule.h"

namespace querier {
    LanguageFilePaths::LanguageFilePaths(std::string mainModule, std::string sourceFile = "", std::string executableFile = "") : MainModule(mainModule) {
        if (!sourceFile.empty())
            SourceFile = sourceFile;
        else
            SourceFile = "";

        if (!executableFile.empty())
            ExecutableFile = executableFile;
        else
            ExecutableFile = "";
    }

    LanguageFilePaths::LanguageFilePaths(std::wstring _wMainModule, std::wstring _wSourceFile = L"", std::wstring _wExecutableFile = L"") : wMainModule(_wMainModule) {
        if (!_wSourceFile.empty())
            wSourceFile = _wSourceFile;
        else
            wSourceFile = L"";

        if (!_wExecutableFile.empty())
            wExecutableFile = _wExecutableFile;
        else
            wExecutableFile = L"";
    }

    LanguageModule::LanguageModule(std::wstring wMainModule, std::wstring wVersionArg, bool runAsModule = true)
        : ILanguage(),
        BaseModule(wMainModule, wVersionArg, runAsModule),
        m_LanguageFilePaths{ wMainModule } {
        m_LanguageFilePaths.wSourceFile = L"";
        m_wszModuleVersion = str_to_wstr(wRun(wVersionArg));
    }

    LanguageModule::LanguageModule(std::string mainModule, std::string versionArg, bool runAsModule = true)
        : ILanguage(),
        BaseModule(mainModule, versionArg, runAsModule),
        m_LanguageFilePaths{ mainModule } {
        m_LanguageFilePaths.SourceFile = "";
        m_szModuleVersion = Run(versionArg);
    }

    LanguageModule::~LanguageModule() {

    }

    void LanguageModule::Invoke(std::string queryName) {
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
                    BeginOutputRead(queryName);
                    BeginErrorRead(queryName);
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

    bool LanguageModule::wGetFileContent(std::wstring* ret) {
        std::string read;
        if (m_File.Read(&read)) {
            *ret = str_to_wstr(read);
            return true;
        }
        return false;
    }

    void LanguageModule::SetFileContent(std::string content) {
        m_File.Write(content);
    };

    void LanguageModule::wSetFileContent(std::wstring content) {
        m_File.wWrite(content);
    }

    std::string LanguageModule::GetSourceFile() {
        return m_LanguageFilePaths.SourceFile;
    }

    std::wstring LanguageModule::wGetSourceFile() {
        return m_LanguageFilePaths.wSourceFile;
    }

    void LanguageModule::SetSourceFile(std::string fileName) {
        m_LanguageFilePaths.SourceFile = fileName;
        m_File.SetFileName(fileName);
    }

    void LanguageModule::wSetSourceFile(std::wstring fileName) {
        m_LanguageFilePaths.wSourceFile = fileName;
        m_File.SetFileName(wstr_to_str(fileName));
    }

    std::string LanguageModule::GetModuleVersion() {
        if (m_szModuleVersion.size() > 0)
            return m_szModuleVersion;
        else if(m_wszModuleVersion.size() > 0)
            return wstr_to_str(m_wszModuleVersion);
    }

    std::wstring LanguageModule::wGetModuleVersion() {
        if (m_wszMainModulePath.size() > 0)
            return m_wszMainModulePath;
        else if (m_szModuleVersion.size() > 0)
            return str_to_wstr(m_szModuleVersion);
    }
}

#endif