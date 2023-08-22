#ifndef _SCRIPT_PAD_FILE_CPP
    #define _SCRIPT_PAD_FILE_CPP

#include "File.h"
#include "Util.h"
#include <Windows.h>

#define BUFSIZE 4096

namespace scriptpad {
    File::File() {

    }

    File::File(std::wstring fileName) : m_fileName(wstr_to_str(fileName)), m_ioFile(m_fileName, std::ios_base::in | std::ios_base::out) {
        if (m_ioFile.bad()) {
            throw std::exception(m_fileName.c_str());
        }
        else {
            m_ioFile.close();
        }
    }

    File::File(std::wstring fileName, std::ios_base::openmode mode) : m_fileName(wstr_to_str(fileName)), m_ioFile(m_fileName, mode) {
        if (m_ioFile.bad()) {
            throw std::exception(m_fileName.c_str());
        }
        else {
            m_ioFile.close();
        }
    }

    File::File(std::string fileName) : m_fileName(fileName), m_ioFile(fileName, std::ios_base::in | std::ios_base::out) {
        if (m_ioFile.bad()) {
            throw std::exception(fileName.c_str());
        }
        else {
            m_ioFile.close();
        }
    }

    File::File(std::string fileName, std::ios_base::openmode mode) : m_fileName(fileName), m_ioFile(fileName, mode) {
        if (m_ioFile.bad()) {
            throw std::exception(m_fileName.c_str());
        }
        else {
            m_ioFile.close();
        }
    }

    File::File(std::wstring fileName, std::wstring fullName, std::wstring path) : Name(fileName), FullName(fullName), Path(path) {
        m_fileName = wstr_to_str(fileName);
        m_ioFile = std::fstream(m_fileName, std::ios_base::in | std::ios_base::out);
        if (m_ioFile.good())
            m_ioFile.close();
    }

    File::~File() {
        
    }

    void File::wWriteFile(std::wstring content) {
        if (m_woFile.is_open()) {
            m_woFile.write(content.c_str(), content.length());
        }
    }

    void File::WriteFile(std::string content) {
        if (!m_ioFile.is_open()) {
            m_ioFile.open(m_fileName, std::ofstream::out | std::ofstream::trunc);
            m_ioFile.write(content.c_str(), content.length());
            m_ioFile.close();
        }
    }

    bool File::Read(std::string* ret) {
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

    File File::FindFile(std::wstring file) {
        WIN32_FIND_DATA wfData;
        HANDLE hFind;

        hFind = FindFirstFile(file.c_str(), &wfData);
        if (hFind == INVALID_HANDLE_VALUE) {
            return File();
        }

        FindClose(hFind);

        return File();
    }

    bool File::SearchFile(std::wstring sFile, File* pFile) {
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

    std::string File::ReadAllText(std::string fileName) {
        std::ifstream is(fileName, std::ios_base::in);
        if (is.good()) {
            char buffer[BUFSIZE];
            std::string ret;
            while (is.read(buffer, sizeof(buffer)))
                ret.append(buffer, sizeof(buffer));
            ret.append(buffer, is.gcount());

            return ret;
        }
        else return nullptr;
    }

    bool File::Create(std::string fileName) {

    }

    std::string Directory::CurrentWorkingDirectory() {

    }

    std::wstring Directory::wCurrentWorkingDirectory() {

    }

    std::string Directory::ApplicationDirectory() {

    }

    std::wstring Directory::wApplicationDirectory() {

    }
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_FILE_CPP