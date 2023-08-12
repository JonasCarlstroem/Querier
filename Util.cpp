#ifndef _UTIL_CPP
#define _UTIL_CPP
#include "Util.h"
#include "Error.h"
#include <Windows.h>

namespace util {
    bool dup_quotes(std::wstring* str) {
        int count{ 0 };
        for (strIteratorIndex qt = { str->begin() }; qt.it < str->end(); qt.it++, qt.id++) {
            if (*qt.it == L'\"') {
                qt.it = str->insert(qt.it, L'\"');

                count++;

                if (qt.it < str->end() - 1)
                    qt.it++;
            }
        }

        return count > 0;
    }

    bool string_to_wstring(const std::string& source, std::wstring* target) {
        int convertResult = MultiByteToWideChar(CP_UTF8, 0, source.c_str(), (int)source.length(), NULL, 0);
        if (convertResult <= 0) {
#ifdef _DEBUG
            error::PrintError(L"MultiByteToWideChar");
#endif
            return false;
        }
        else {
            target->resize(convertResult);
            convertResult = MultiByteToWideChar(CP_UTF8, 0, source.c_str(), (int)source.length(), target->data(), (int)target->size());
            if (convertResult <= 0) {
#ifdef _DEBUG
                error::PrintError(L"MultiByteToWideChar");
#endif
                return false;
            }
        }
        return true;
    }

    bool wstring_to_string(const std::wstring& source, std::string* target) {
        int convertResult = WideCharToMultiByte(CP_ACP, 0, source.c_str(), -1, NULL, 0, NULL, NULL);
        if (convertResult <= 0) {
#ifdef _DEBUG
            error::PrintError(L"WideCharToMultiByte");
#endif
            return false;
        }
        else {
            target->resize(convertResult);
            convertResult = WideCharToMultiByte(CP_ACP, 0, source.c_str(), -1, target->data(), (int)target->size(), NULL, NULL);
            if (convertResult <= 0) {
#ifdef _DEBUG
                error::PrintError(L"WideCharToMultiByte");
#endif
                return false;
            }
        }
        return true;
    }
}

#endif