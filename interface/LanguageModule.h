#pragma once

#ifndef _SCRIPT_PAD_LANGUAGE_MODULE_H
#define _SCRIPT_PAD_LANGUAGE_MODULE_H

#include "../resources.h"
#include "ILanguage.h"
#include "BaseModule.h"

namespace scriptpad {
    enum LanguageType {
        Interpreter,
        Compiler
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(LanguageType, {
        { Interpreter, "interpreter" },
        { Compiler, "compiler" }
    });

    struct LanguageFilePaths {
        std::wstring MainModule;

        std::wstring SourceFileName;
        std::wstring SourceFileExtension;

        std::wstring get_SourceFile() {
            return SourceFileName + SourceFileExtension;
        }

        void set_SourceFile(std::wstring fileName) {
            auto file0Ext1 = wstr_split(fileName, L'.');
            if (file0Ext1.size() == 2) {
                SourceFileName = file0Ext1[0];
                SourceFileExtension = L'.' + file0Ext1[1];
            }
        }

        std::wstring ExecutableFileName;
        std::wstring ExecutableFileExtension;

        std::wstring get_ExecutableFile() {
            return ExecutableFileName + ExecutableFileExtension;
        }
    };

    class __declspec(dllexport) LanguageModule : public ILanguage, public BaseModule {
    public:
        LanguageModule(std::wstring mainModule, std::wstring sourceFile, std::wstring versionArg, bool runAsModule);
        //LanguageModule(LanguageFilePaths filePaths);
        //LanguageModule(const LanguageModule& mod);

        virtual ~LanguageModule();

        bool GetFileContent(std::string*);
        void SetFileContent(std::string);
        virtual void Initialize() = 0;
        virtual void Invoke();

        void SetSourceFile(std::wstring fileName, std::wstring extension);
        std::wstring GetSourceFile();
        void SetSourceFileName(std::wstring name);
        std::wstring GetSourceFileName();
        void SetSourceFileExtension(std::wstring extension);
        std::wstring GetSourceFileExtension();

    protected:
        LanguageType m_LanguageType{ Interpreter };

    private:
        LanguageFilePaths m_LanguageFilePaths;
        File m_File;

    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_LANGUAGE_MODULE_H