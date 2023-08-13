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
        FileHandler(std::string fileName);
        ~FileHandler();

        void wWriteFile(std::wstring);
        void WriteFile(std::string);

    private:
        std::wstring m_wFileName;
        std::string m_fileName;
        std::wofstream m_wFile;
        std::ofstream m_file;
    };

    class ProductFinder {

    };
}

#endif