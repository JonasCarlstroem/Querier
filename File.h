#pragma once

#ifndef _SCRIPT_PAD_FILE_H
    #define _SCRIPT_PAD_FILE_H

#include "Error.h"
#include "Util.h"
#include <Windows.h>
#include <utility>
#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <ShlObj.h>

using namespace std::filesystem;
namespace querier {
    class __declspec(dllexport) File {
    public:
        File();
        File(std::wstring);
        File(std::wstring, std::ios_base::openmode);
        File(std::string);
        File(std::string, std::ios_base::openmode);
        File(std::wstring, std::wstring, std::wstring);
        ~File();

        File& operator=(const File&& file) noexcept {
            m_fileName = file.m_fileName;
            m_ioFile = std::fstream(m_fileName, std::ios::in | std::ios::out);
            return *this;
        };

        std::wstring Name;
        std::wstring FullName;
        std::wstring Path;

        void wWriteFile(std::wstring);
        void Write(std::string);
        //bool Read(std::string*);
        bool Read(std::string*);

        void SetFileName(std::string);

        static File FindFile(path file);
        static bool SearchFile(path sFile, File* pFile);
        static std::string ReadAllText(path);
        static std::wstring ReadAllTextW(path);
        static bool Create(path fileName);

        static bool Exists(path);
        static bool Exists(std::wstring);

    private:
        std::wstring m_wFileName{ 0 };
        std::string m_fileName{ 0 };
        std::wofstream m_woFile;
        std::ofstream m_oFile;
        std::fstream m_ioFile;
        std::ios_base::openmode mode = std::ofstream::out;
    };

    class __declspec(dllexport) Directory {
    public:
        static path GetCurrentWorkingDirectory();
        static bool SetCurrentWorkingDirectory(path);
        static path ApplicationDirectory();

        static bool GetSpecialFolder(KNOWNFOLDERID, path*);

        static std::vector<path> GetFiles(path);
        static std::vector<path> GetDirectories(path);

        static bool Create(path);

        static bool Exists(path);
    };

    class ProductFinder {

    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_FILE_H