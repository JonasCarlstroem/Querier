#pragma once

#ifndef _FILE_H
    #define _FILE_H
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

        File FindFile(std::wstring);

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