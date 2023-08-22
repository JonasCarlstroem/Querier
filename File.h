#pragma once

#ifndef _SCRIPT_PAD_FILE_H
    #define _SCRIPT_PAD_FILE_H

#include "Error.h"
#include "Util.h"
#include <Windows.h>
#include <string>
#include <fstream>

namespace scriptpad {
    class File {
    public:
        File();
        File(std::wstring);
        File(std::wstring, std::ios_base::openmode);
        File(std::string);
        File(std::string, std::ios_base::openmode);
        File(std::wstring, std::wstring, std::wstring);
        ~File();

        std::wstring Name;
        std::wstring FullName;
        std::wstring Path;

        void wWriteFile(std::wstring);
        void WriteFile(std::string);
        bool Read(std::string*);

        static File FindFile(std::wstring file);
        static bool SearchFile(std::wstring sFile, File* pFile);
        static std::string ReadAllText(std::string);
        static bool Create(std::string fileName);

    private:
        std::wstring m_wFileName{ 0 };
        std::string m_fileName{ 0 };
        std::wofstream m_woFile;
        std::ofstream m_oFile;
        std::fstream m_ioFile;
        std::ios_base::openmode mode = std::ofstream::out;
    };

    class Directory {
        static std::string CurrentWorkingDirectory();
        static std::wstring wCurrentWorkingDirectory();
        static std::string ApplicationDirectory();
        static std::wstring wApplicationDirectory();
    };

    class ProductFinder {

    };
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_FILE_H