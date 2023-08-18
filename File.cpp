#ifndef _SCRIPT_PAD_FILE_CPP
    #define _SCRIPT_PAD_FILE_CPP

#include "File.h"
#include "Util.h"
#include <Windows.h>

#define BUFSIZE 4096

namespace file {

    FileHandler::FileHandler(std::wstring fileName) : m_fileName(util::wstring_to_string(fileName)), m_ioFile(m_fileName, std::ios_base::in | std::ios_base::out) {
        if (m_ioFile.bad()) {
            throw std::exception(m_fileName.c_str());
        }
        else {
            m_ioFile.close();
        }
    }

    FileHandler::FileHandler(std::wstring fileName, std::ios_base::openmode mode) : m_fileName(util::wstring_to_string(fileName)), m_ioFile(m_fileName, mode) {
        if (m_ioFile.bad()) {
            throw std::exception(m_fileName.c_str());
        }
        else {
            m_ioFile.close();
        }
    }

    FileHandler::FileHandler(std::string fileName) : m_fileName(fileName), m_ioFile(fileName, std::ios_base::in | std::ios_base::out) {
        if (m_ioFile.bad()) {
            throw std::exception(fileName.c_str());
        }
        else {
            m_ioFile.close();
        }
    }

    FileHandler::FileHandler(std::string fileName, std::ios_base::openmode mode) : m_fileName(fileName), m_ioFile(fileName, mode) {
        if (m_ioFile.bad()) {
            throw std::exception(m_fileName.c_str());
        }
        else {
            m_ioFile.close();
        }
    }

    FileHandler::~FileHandler() {
        
    }

    void FileHandler::wWriteFile(std::wstring content) {
        if (m_woFile.is_open()) {
            m_woFile.write(content.c_str(), content.length());
        }
    }

    void FileHandler::WriteFile(std::string content) {
        if (!m_ioFile.is_open()) {
            m_ioFile.open(m_fileName, std::ofstream::out | std::ofstream::trunc);
            m_ioFile.write(content.c_str(), content.length());
            m_ioFile.close();
        }
    }

    bool FileHandler::ReadFile(std::string* ret) {
        if (!m_ioFile.is_open()) {
            char buffer[BUFSIZE];
            ret->clear();
            m_ioFile.open(m_fileName, std::fstream::in);
            while (m_ioFile.read(buffer, sizeof(buffer)))
                ret->append(buffer, sizeof(buffer));
            ret->append(buffer, m_ioFile.gcount());

            m_ioFile.close();
            return true;
        }
        return false;
    }
}

#endif