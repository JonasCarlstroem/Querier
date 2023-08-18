#pragma once

#ifndef _SCRIPT_PAD_FILE_H
    #define _SCRIPT_PAD_FILE_H

#include "Error.h"
#include "Util.h"
#include <Windows.h>
#include <string>
#include <fstream>

namespace file {

    struct File {
        std::wstring Name;
        std::wstring FullName;
        std::wstring Path;
    };

    class FileFinder {
    public:

        File FindFile(std::wstring file) {
            WIN32_FIND_DATA wfData;
            HANDLE hFind;

            hFind = FindFirstFile(file.c_str(), &wfData);
            if (hFind == INVALID_HANDLE_VALUE) {
                return File();
            }

            FindClose(hFind);

            return File();
        };

        bool SearchFile(std::wstring sFile, File* pFile) {
            LPWSTR lpFilePart;
            std::wstring ret;
            ret.resize(MAX_PATH);
            if (!SearchPath(NULL, sFile.c_str(), NULL, MAX_PATH, ret.data(), &lpFilePart)) {
                PRINT_ERROR(L"SearchPath");
                return false;
            }
            pFile = new File{ sFile, ret, lpFilePart };
            return true;
        }

    private:


    };

    class FileHandler {
    public:
        FileHandler(std::wstring fileName);
        FileHandler(std::wstring fileName, std::ios_base::openmode mode);
        FileHandler(std::string fileName);
        FileHandler(std::string fileName, std::ios_base::openmode mode);
        ~FileHandler();

        void wWriteFile(std::wstring);
        void WriteFile(std::string);
        bool ReadFile(std::string*);

    private:
        std::wstring m_wFileName;
        std::string m_fileName;
        std::wofstream m_woFile;
        std::ofstream m_oFile;
        std::fstream m_ioFile;
        std::ios_base::openmode mode = std::ofstream::out;
    };

    class ProductFinder {

    };
}

#endif