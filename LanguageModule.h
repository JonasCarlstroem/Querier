#pragma once

#ifndef _SCRIPT_PAD_LANGUAGE_MODULE_H
#define _SCRIPT_PAD_LANGUAGE_MODULE_H

#include "../resource.h"
#include "ILanguage.h"
#include "BaseModule.h"

namespace querier {
    enum LanguageType {
        Interpreter,
        Compiler
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(LanguageType, {
        { Interpreter, "interpreter" },
        { Compiler, "compiler" }
    });

    struct LanguageFilePaths {
        LanguageFilePaths(std::string, std::string, std::string);
        LanguageFilePaths(std::wstring, std::wstring, std::wstring);

        std::string MainModule;
        std::wstring wMainModule;

        std::string SourceFile;
        std::wstring wSourceFile;

        std::string ExecutableFile;
        std::wstring wExecutableFile;
    };

    class __declspec(dllexport) LanguageModule : public ILanguage, public BaseModule {
    public:
        LanguageModule(std::wstring, std::wstring, bool);
        LanguageModule(std::string, std::string, bool);

        virtual ~LanguageModule();

        virtual void Initialize() = 0;
        virtual void Invoke(std::string);

        bool GetFileContent(std::string*);
        bool wGetFileContent(std::wstring*);

        void SetFileContent(std::string);
        void wSetFileContent(std::wstring);

        std::string GetSourceFile();
        std::wstring wGetSourceFile();

        void SetSourceFile(std::string);
        void wSetSourceFile(std::wstring);

        std::string GetModuleVersion();
        std::wstring wGetModuleVersion();

        bool IsUnicode() {
            return m_isUnicode;
        }

    protected:
        LanguageType m_LanguageType{ Interpreter };

    private:
        LanguageFilePaths m_LanguageFilePaths;
        File m_File;

    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_LANGUAGE_MODULE_H