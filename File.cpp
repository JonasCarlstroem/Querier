#ifndef _FILE_CPP
    #define _FILE_CPP

#include "File.h"
#include "Util.h"
#include <Windows.h>

namespace file {
    File FileFinder::FindFile(std::wstring file) {
        WIN32_FIND_DATA wfData;
        HANDLE hFind;

        hFind = FindFirstFile(file.c_str(), &wfData);
        if (hFind == INVALID_HANDLE_VALUE) {
            return File();
        }

        FindClose(hFind);

        return File();
    }

    FileHandler::FileHandler(std::wstring fileName) : m_wFileName(fileName), m_wFile(fileName) {
        if (m_wFile.bad()) {
            std::string err;
            util::wstring_to_string(fileName, &err);
            throw std::exception(err.c_str());
        }
    }

    FileHandler::FileHandler(std::string fileName) : m_fileName(fileName), m_file(fileName) {
        if (m_file.bad()) {
            throw std::exception(fileName.c_str());
        }
        else {
            m_file.close();
        }
    }

    FileHandler::~FileHandler() {
        if (m_wFileName.empty()) {
            util::string_to_wstring(m_fileName, &m_wFileName);
        }
        auto res = DeleteFile(m_wFileName.c_str());
    }

    void FileHandler::wWriteFile(std::wstring content) {
        if (m_wFile.is_open()) {
            m_wFile.write(content.c_str(), content.length());
        }
    }

    void FileHandler::WriteFile(std::string content) {
        if (!m_file.is_open()) {
            m_file.open(m_fileName, std::ofstream::out | std::ofstream::trunc);
            m_file.write(content.c_str(), content.length());
            m_file.close();
        }
    }
}

#endif