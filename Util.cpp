#ifndef _UTIL_CPP
#define _UTIL_CPP
#include "Util.h"
#include "Error.h"
#include <Windows.h>

namespace querier {

    //Duplicate quotes in string
    bool dup_quotes(std::wstring* wstr) {
        int count{ 0 };
        for (strIteratorIndex qt = { wstr->begin() }; qt.it < wstr->end(); qt.it++, qt.id++) {
            if (*qt.it == L'\"') {
                qt.it = wstr->insert(qt.it, L'\"');

                count++;

                if (qt.it < wstr->end() - 1)
                    qt.it++;
            }
        }
        return count > 0;
    };

    bool convert_backslashes(std::wstring* wstr) {
        int i;
        for (i = 0; i < wstr->size(); i++) {
            if((*wstr)[i] == L'\\')
                (*wstr)[i] = L'/';
        }
        return true;
    }

    std::vector<std::string> str_split(std::string input, char delim) {
        std::vector<std::string> strings;
        std::string temp;
        for (auto it = input.begin(); it < input.end(); it++) {
            if (it == input.end() - 1) {
                temp += (*it);
                strings.push_back(temp);
                break;
            }
            if (*it == delim) {
                strings.push_back(temp);
                temp.clear();
            }
            else {
                temp += (*it);
            }
        }

        return strings;
    }

    std::vector<std::wstring> wstr_split(std::wstring input, wchar_t delim) {
        std::vector<std::wstring> wstrings;
        std::wstring wtemp;
        for (auto it = input.begin(); it < input.end(); it++) {
            if (it == input.end() - 1) {
                wtemp += (*it);
                wstrings.push_back(wtemp);
                break;
            }
            if (*it == delim) {
                wstrings.push_back(wtemp);
                wtemp.clear();
            }
            else {
                wtemp += (*it);
            }
        }

        return wstrings;
    }

    //Convert ANSI string to UNICODE string
    bool str_to_wstr(const std::string& source, std::wstring* target) {
        int convertResult = MultiByteToWideChar(CP_UTF8, 0, source.c_str(), (int)source.length(), NULL, 0);
        if (convertResult <= 0) {
            PRINT_ERROR(L"MultiByteToWideChar");
            return false;
        }
        else {
            target->resize(convertResult);
            convertResult = MultiByteToWideChar(CP_UTF8, 0, source.c_str(), (int)source.length(), target->data(), (int)target->size());
            if (convertResult <= 0) {
                PRINT_ERROR(L"MultiByteToWideChar");
                return false;
            }
        }
        return true;
    }

    //Convert ANSI string to UNICODE string
    std::wstring str_to_wstr(const std::string& source) {
        std::wstring ret;
        if (!str_to_wstr(source, &ret)) {
            throw std::exception("Error converting string");
        }
        return ret;
    };

    //Convert UNICODE string to ANSI string
    bool wstr_to_str(const std::wstring& source, std::string* target) {
        int convertResult = WideCharToMultiByte(CP_ACP, 0, source.c_str(), -1, NULL, 0, NULL, NULL);
        if (convertResult <= 0) {
            PRINT_ERROR(L"WideCharToMultiByte");
            return false;
        }
        else {
            target->resize(convertResult);
            convertResult = WideCharToMultiByte(CP_ACP, 0, source.c_str(), -1, target->data(), (int)target->size(), NULL, NULL);
            if (convertResult <= 0) {
                PRINT_ERROR(L"WideCharToMultiByte");
                return false;
            }
        }
        return true;
    };

    //Convert UNICODE string to ANSI string
    std::string wstr_to_str(const std::wstring& source) {
        std::string ret;
        if (!wstr_to_str(source, &ret)) {
            throw std::exception("Error converting string");
        }
        return ret;
    };

    //Print message in a MessageBox
    void print_msg(std::wstring message) {
        MessageBox(NULL, message.c_str(), NULL, MB_OK);
    };

    //Print message and title in a MessageBox
    void print_msg(std::wstring message, std::wstring title) {
        MessageBox(NULL, message.c_str(), title.c_str(), MB_OK);
    };
}

#endif