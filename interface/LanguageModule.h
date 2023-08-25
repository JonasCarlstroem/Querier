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
        LanguageFilePaths(std::string, std::string, std::string);
        LanguageFilePaths(std::wstring, std::wstring, std::wstring);

        std::string MainModule;
        std::wstring wMainModule;

        std::string SourceFileName;
        std::string SourceFileExtension;

        std::string get_SourceFile() {
            return SourceFileName + SourceFileExtension;
        }

        void set_SourceFile(std::string fileName) {
            auto file0Ext1 = str_split(fileName, '.');
            if (file0Ext1.size() == 2) {
                SourceFileName = file0Ext1[0];
                SourceFileExtension = file0Ext1[1];
            }
        }

        std::wstring wSourceFileName;
        std::wstring wSourceFileExtension;

        std::wstring get_wSourceFile() {
            return wSourceFileName + wSourceFileExtension;
        }

        void set_wSourceFile(std::wstring fileName) {
            auto file0Ext1 = wstr_split(fileName, L'.');
            if (file0Ext1.size() == 2) {
                wSourceFileName = file0Ext1[0];
                wSourceFileExtension = L'.' + file0Ext1[1];
            }
        }

        std::string ExecutableFileName;
        std::string ExecutableFileExtension;

        std::string get_ExecutableFile() {
            return ExecutableFileName + ExecutableFileExtension;
        }

        void set_ExecutableFile(std::string fileName) {
            auto file0Ext1 = str_split(fileName, '.');
            if (file0Ext1.size() == 2) {
                ExecutableFileName = file0Ext1[0];
                ExecutableFileExtension = file0Ext1[1];
            }
        }

        std::wstring wExecutableFileName;
        std::wstring wExecutableFileExtension;

        std::wstring get_wExecutableFile() {
            return wExecutableFileName + wExecutableFileExtension;
        }

        void set_wExecutableFile(std::wstring fileName) {
            auto file0Ext1 = wstr_split(fileName, L'.');
            if (file0Ext1.size() == 2) {
                wExecutableFileName = file0Ext1[0];
                wExecutableFileExtension = file0Ext1[1];
            }
        }
    };

    class __declspec(dllexport) LanguageModule : public ILanguage, public BaseModule {
    public:
        LanguageModule(std::wstring, std::wstring, std::wstring, bool);
        LanguageModule(std::string, std::string, std::string, bool);

        virtual ~LanguageModule();

        bool GetFileContent(std::string*);
        void SetFileContent(std::string);
        virtual void Initialize() = 0;
        virtual void Invoke();

        void SetSourceFile(std::string, std::string);
        void wSetSourceFile(std::wstring, std::wstring);

        std::string GetSourceFile();
        std::wstring wGetSourceFile();

        void SetSourceFileName(std::string);
        void wSetSourceFileName(std::wstring);

        std::string GetSourceFileName();
        std::wstring wGetSourceFileName();

        void SetSourceFileExtension(std::string);
        void wSetSourceFileExtension(std::wstring);

        std::string GetSourceFileExtension();
        std::wstring wGetSourceFileExtension();

    protected:
        LanguageType m_LanguageType{ Interpreter };

    private:
        LanguageFilePaths m_LanguageFilePaths;
        File m_File;

    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_LANGUAGE_MODULE_H