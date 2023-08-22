#ifndef _SCRIPT_PAD_LANGUAGE_MODULE_CPP
#define _SCRIPT_PAD_LANGUAGE_MODULE_CPP

#include "LanguageModule.h"

namespace scriptpad {

    void LanguageModule::Initialize(AppWindow* appWin) {
        m_AppWindow = appWin;
    }

    void LanguageModule::Invoke() {
        if (m_bIsModuleInstalled) {
            switch (m_LanguageType) {
                case Interpreter:
                {
                    Start();
                    WaitForExit();

                    std::string ret;
                    ReadOutput(&ret);
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
}

#endif