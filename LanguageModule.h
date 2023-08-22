#pragma once

#ifndef _SCRIPT_PAD_LANGUAGE_MODULE_H
#define _SCRIPT_PAD_LANGUAGE_MODULE_H

#include "ILanguage.h"
#include "BaseModule.h"
#include "AppWindow.h"

namespace scriptpad {

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
                SourceFileExtension = file0Ext1[1];
            }
        }

        std::wstring ExecutableFileName;
        std::wstring ExecutableFileExtension;

        std::wstring get_ExecutableFile() {
            return ExecutableFileName + ExecutableFileExtension;
        }
    };

    class LanguageModule : public ILanguage, public BaseModule {
    public:
        LanguageModule(LanguageFilePaths filePaths) 
            : ILanguage(),
            m_LanguageFilePaths(filePaths), 
            m_FileHandler(filePaths.get_SourceFile()),
            BaseModule(filePaths.MainModule), 
            m_AppWindow{0} {};

        LanguageModule(const LanguageModule& mod)
            : ILanguage(),
            m_LanguageFilePaths(mod.m_LanguageFilePaths),
            m_FileHandler(m_LanguageFilePaths.get_SourceFile()),
            BaseModule(m_LanguageFilePaths.MainModule),
            m_AppWindow{ 0 } {};

        virtual bool GetFileContent(std::string*);
        virtual void SetFileContent(std::string);
        virtual void Initialize(AppWindow*);
        virtual void Invoke();

    protected:
        AppWindow* m_AppWindow;

        LanguageFilePaths m_LanguageFilePaths;
        /*std::wstring* m_wszSourceFileName{ 0 };
        std::wstring* m_wszSourceFileExtension{ 0 };

        std::wstring* m_wszExecutableFileName{ 0 };
        std::wstring* m_wszExecutableFileExtension{ 0 };*/

    private:
        File m_File;

    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_LANGUAGE_MODULE_H