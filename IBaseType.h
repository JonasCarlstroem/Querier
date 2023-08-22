#pragma once

#ifndef _SCRIPT_PAD_IBASE_TYPE_H
#define _SCRIPT_PAD_IBASE_TYPE_H

#include <concepts>

namespace scriptpad {
    class IBaseType {
    public:
        virtual void Invoke() = 0;
    };

    class Interpreter : public IBaseType {

    };

    class Compiler : public IBaseType {
    public:
        virtual void Compile() = 0;
    };

    template<class T>
    concept IType = requires(T Sub) {
        std::derived_from<T, IBaseType>;
    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_IBASE_TYPE_H