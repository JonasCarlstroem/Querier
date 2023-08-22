#pragma once

#ifndef _SCRIPT_PAD_UTIL_H
#define _SCRIPT_PAD_UTIL_H

#include <Windows.h>
#include <string>
#include <vector>

namespace scriptpad {
    struct strIteratorIndex {
        std::wstring::const_iterator it;
        int id{ 0 };
    };

    //Duplicate quotes in string
    bool dup_quotes(std::wstring* wstr);

    std::vector<std::string> str_split(std::string, char);
    std::vector<std::wstring> wstr_split(std::wstring, wchar_t);

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
    bool str_to_wstr(const std::string& source, std::wstring* target);
    //Convert ANSI string to UNICODE string
    std::wstring str_to_wstr(const std::string& source);
    //Convert UNICODE string to ANSI string
    bool wstr_to_str(const std::wstring& source, std::string* target);
    //Convert UNICODE string to ANSI string
    std::string wstr_to_str(const std::wstring& source);

    //Print message in a MessageBox
    void print_msg(std::wstring message);
    //Print message and title in a MessageBox
    void print_msg(std::wstring message, std::wstring title);

}   //namespace scriptpad

#endif  //_SCRIPT_PAD_UTIL_H