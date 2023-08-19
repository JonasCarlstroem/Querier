#pragma once

#ifndef _UTIL_H
#define _UTIL_H

#include <Windows.h>
#include <string>
namespace util {
    struct strIteratorIndex {
        std::wstring::const_iterator it;
        int id{ 0 };
    };

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

    //Trim whitespaces from both ends
    bool str_trim(std::string*);
    //Trim whitespaces from both ends
    bool wstr_trim(std::wstring*);
    //Trim whitespaces from both ends
    std::string str_trim(std::string);
    //Trim whitespaces from both ends
    std::wstring wstr_trim(std::wstring);
    
    //Trim whitespaces from beginning of string
    bool str_trim_start(std::string*);
    //Trim whitespaces from beginning of string
    bool wstr_trim_start(std::wstring*);
    //Trim whitespaces from beginning of string
    std::string str_trim_start(std::string);
    //Trim whitespaces from beginning of string
    std::wstring wstr_trim_start(std::wstring);

    //Trim whitespaces from end of string
    bool str_trim_end(std::string*);
    //Trim whitespaces from end of string
    bool wstr_trim_end(std::wstring*);
    //Trim whitespaces from end of string
    std::string str_trim_end(std::string);
    //Trim whitespaces from end of string
    std::wstring wstr_trim_end(std::wstring);

    //Trim all specified char from string
    bool str_trim_char(std::string* str, char toTrim);
    //Trim all specified wchar_t from string
    bool wstr_trim_char(std::wstring* wstr, wchar_t toTrim);
    //Trim all specified char from string
    std::string str_trim_char(std::string str, char toTrim);
    //Trim all specified wchar_t from string
    std::wstring wstr_trim_char(std::wstring wstr, wchar_t toTrim);
    
    //Convert ANSI string to UNICODE string
    bool str_to_wstr(const std::string& source, std::wstring* target) {
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

    //Convert ANSI string to UNICODE string
    std::wstring str_to_wstr(const std::string& source) {
        std::wstring ret;
        if (!util::str_to_wstr(source, &ret)) {
            throw std::exception("Error converting string");
        }
        return ret;
    };

    //Convert UNICODE string to ANSI string
    bool wstr_to_str(const std::wstring& source, std::string* target) {
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
    };

    //Convert UNICODE string to ANSI string
    std::string wstr_to_str(const std::wstring& source) {
        std::string ret;
        if (!util::wstr_to_str(source, &ret)) {
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