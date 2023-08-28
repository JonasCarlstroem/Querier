#pragma warning(suppress : 6387)

#ifndef _SCRIPT_PAD_FILE_CPP
    #define _SCRIPT_PAD_FILE_CPP

#include "File.h"
#include "Util.h"

#define BUFSIZE 4096

namespace querier {
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

    File File::FindFile(path file) {
        WIN32_FIND_DATA wfData;
        HANDLE hFind;

        hFind = FindFirstFile(file.c_str(), &wfData);
        if (hFind == INVALID_HANDLE_VALUE) {
            return File();
        }

        FindClose(hFind);

        return File();
    }

    bool File::SearchFile(path sFile, File* pFile) {
        LPWSTR lpFilePart;
        std::wstring ret;
        ret.resize(MAX_PATH);
        if (!SearchPath(NULL, sFile.c_str(), NULL, MAX_PATH, ret.data(), &lpFilePart)) {
            PRINT_ERROR(L"SearchPath");
            return false;
        }
        if(pFile)
            pFile = new File{ sFile, ret, lpFilePart };

        return true;
    }

    std::string File::ReadAllText(path fileName) {
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

    std::wstring File::ReadAllTextW(path fileName) {
        std::wifstream wif(fileName, std::ios_base::in);
        if (wif.good()) {
            wchar_t buffer[BUFSIZE];
            std::wstring ret;
            while (wif.read(buffer, sizeof(buffer)))
                ret.append(buffer, sizeof(buffer));
            ret.append(buffer, wif.gcount());

            return ret;
        }
        return std::wstring();
    }

    bool File::Create(path fileName) {
        std::ofstream of(fileName, std::ios_base::out);
        bool success = of.good();
        of.close();
        return success;
    }

    bool File::Exists(path file) {
        return exists(file) && is_regular_file(file);
    }

    bool File::Exists(std::wstring file) {
        return !(INVALID_FILE_ATTRIBUTES == GetFileAttributes(file.c_str()) && GetLastError() == ERROR_FILE_NOT_FOUND);
    }

    path Directory::GetCurrentWorkingDirectory() {
        TCHAR buffer[MAX_PATH] = { 0 };
        if (GetCurrentDirectory(MAX_PATH, buffer))
            return buffer;
        return path::path();
    }

    bool Directory::SetCurrentWorkingDirectory(path path) {
        return SetCurrentDirectory(path.c_str());
    }

    bool Directory::GetSpecialFolder(KNOWNFOLDERID id, path* p) {
        std::wstring fol[BUFSIZE];
        wchar_t* fol2;
        if (SUCCEEDED(SHGetKnownFolderPath(id, 0, NULL, &fol2))) {
            *p = path(fol2);
            return true;
        }
        return false;
    }

    path Directory::ApplicationDirectory() {
        return std::string();
    }

    std::vector<path> Directory::GetFiles(path _path) {
        std::vector<path> result;
        WIN32_FIND_DATA findData{ 0 };
        LARGE_INTEGER fileSize{ 0 };
        wchar_t dir[MAX_PATH];
        HANDLE hFind = INVALID_HANDLE_VALUE;

        _path.append(L"\\*");
        hFind = FindFirstFile(_path.c_str(), &findData);

        if (hFind == INVALID_HANDLE_VALUE)
            PRINT_ERROR(L"FindFirstFileW");

        BOOL tmp;
        do {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

            }
            else {
                fileSize.LowPart = findData.nFileSizeLow;
                fileSize.HighPart = findData.nFileSizeHigh;

                result.push_back(findData.cFileName);
            }
            tmp = FindNextFile(hFind, &findData);
        } while (tmp != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES)
            PRINT_ERROR(L"FindFirstFile");

        FindClose(hFind);
        return result;
    }

    std::vector<path> Directory::GetDirectories(path _path) {
        std::vector<path> result;
        WIN32_FIND_DATA findData{ 0 };
        wchar_t dir[MAX_PATH];
        HANDLE hFind = INVALID_HANDLE_VALUE;

        _path += path(L"\\*");
        hFind = FindFirstFileW(_path.c_str(), &findData);

        if (hFind == INVALID_HANDLE_VALUE)
            PRINT_ERROR(L"FindFirstFileW");

        BOOL tmp;
        do {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (findData.cFileName[0] != L'.' && findData.cFileName[1] != L'.')
                    result.push_back(findData.cFileName);
            }
            tmp = FindNextFile(hFind, &findData);
        } while (tmp != 0);

        if (GetLastError() != ERROR_NO_MORE_FILES)
            PRINT_ERROR(L"FindFirstFile");

        FindClose(hFind);
        return result;
    }

    bool Directory::Create(path _path) {
        return create_directory(_path);
    }

    bool Directory::Exists(path _path) {
        return exists(_path) && is_directory(_path);
    }
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_FILE_CPP