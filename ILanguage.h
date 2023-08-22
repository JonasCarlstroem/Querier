#pragma once

#ifndef _SCRIPT_PAD_IBASE_TYPE_H
#define _SCRIPT_PAD_IBASE_TYPE_H

namespace scriptpad {
    enum LanguageType {
        Interpreter,
        Compiler
    };

    class ILanguage {
    public:
        virtual void Invoke() = 0;
    protected:
        LanguageType m_LanguageType{ Interpreter };
    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_IBASE_TYPE_H