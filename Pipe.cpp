#ifndef _SCRIPT_PAD_PIPE_CPP
#define _SCRIPT_PAD_PIPE_CPP

#include "Pipe.h"

namespace scriptpad {

    /********************************************************** PUBLIC ***********************************************************************/

    Pipe::Pipe(SECURITY_ATTRIBUTES& sa, PROCESS_INFORMATION& pi) : m_saAttr(sa), m_piProcInfo(pi) {};
    Pipe::~Pipe() {
        if (m_redirectedInput)
            CloseHandle(m_stdIn.write);

        if (m_redirectedOutput)
            CloseHandle(m_stdOut.read);

        if (m_redirectedError)
            CloseHandle(m_stdErr.read);
    };

    bool Pipe::RedirectIO(bool input, bool output, bool error, STARTUPINFO& si) {
        m_redirectedInput = input;
        m_redirectedOutput = output;
        m_redirectedError = error;

        if (input || output || error) {
            si.dwFlags |= STARTF_USESTDHANDLES;
            m_saAttr.bInheritHandle = TRUE;
        }

        if (input) {

            if (!CreatePipe(&m_stdIn.read, &m_stdIn.write, &m_saAttr, 0)) {
                PRINT_ERROR(L"Pipe::RedirectIO->CreatePipe");
                return false;
            }

            if (!SetHandleInformation(m_stdIn.write, HANDLE_FLAG_INHERIT, 0)) {
                PRINT_ERROR(L"Pipe::RedirectIO->SetHandleInformation");
                return false;
            }

            if (!_InitInput())
                return false;

            si.hStdInput = m_stdIn.read;
        }

        if (output) {
            if (!CreatePipe(&m_stdOut.read, &m_stdOut.write, &m_saAttr, 0)) {
                PRINT_ERROR(L"Pipe::RedirectIO->CreatePipe");
                return false;
            }

            if (!SetHandleInformation(m_stdOut.read, HANDLE_FLAG_INHERIT, 0)) {
                PRINT_ERROR(L"ProcessPipe::redirect_io->SetHandleInformation");
                return false;
            }

            if (!_InitOutput())
                return false;

            si.hStdOutput = m_stdOut.write;
        }

        if (error) {
            if (!CreatePipe(&m_stdErr.read, &m_stdErr.write, &m_saAttr, 0)) {
                PRINT_ERROR(L"Pipe::RedirectIO->CreatePipe");
                return false;
            }

            if (!SetHandleInformation(m_stdErr.read, HANDLE_FLAG_INHERIT, 0)) {
                PRINT_ERROR(L"Pipe::RedirectIO->SetHandleInformation");
                return false;
            }

            if (!_InitError())
                return false;

            si.hStdError = m_stdErr.write;
        }

        return input || output|| error;
    };


    void Pipe::WriteInput(std::string data) {
        if (m_stdIn.in) {
            if (m_stdIn.in.is_open()) {
                m_stdIn.in.write(data.c_str(), data.length());
                m_stdIn.in.close();
            }
        }
    };

    bool Pipe::ReadOutput(std::wstring* ret) {
        std::string buf;
        if (ReadOutput(&buf)) {
            return str_to_wstr(buf, ret);
        }
        return false;
    };

    bool Pipe::ReadOutput(std::string* ret) {
        if (m_stdOut.out) {
            return Read(&m_stdOut.out, ret);
        }
        return false;
    };

    bool Pipe::ReadError(std::wstring* ret) {
        std::string buf;
        if (ReadError(&buf)) {
            return str_to_wstr(buf, ret);
        }
        return false;
    }

    bool Pipe::ReadError(std::string* ret) {
        if (m_stdErr.err) {
            return Read(&m_stdErr.err, ret);
        }
        return false;
    }

    bool Pipe::Read(std::ifstream* std, std::string* ret) {
        char buffer[BUFSIZE];

        ret->clear();
        while (std->read(buffer, sizeof(buffer)))
            ret->append(buffer, sizeof(buffer));
        ret->append(buffer, std->gcount());

        return std->gcount() > 0;
    }

    void Pipe::ProcessStarted() {
        if (m_redirectedInput) {
            CloseHandle(m_stdIn.read);
        }
        if (m_redirectedOutput) {
            CloseHandle(m_stdOut.write);
        }
        if (m_redirectedError) {
            CloseHandle(m_stdErr.write);
        }
    };


    void Pipe::BeginOutputRead() {
        readingOutput = true;
        m_threads.insert({ "output", std::thread(&Pipe::_read_output_loop, this, &m_retBuffer) });
    };


    void Pipe::EndOutputRead() {
        readingOutput = false;
        m_threads["output"].join();
        m_threads.erase("output");
    };


    void Pipe::BeginErrorRead() {
        readingError = true;
        m_threads.insert({ "error", std::thread(&Pipe::_read_error_loop, this, &m_errBuffer) });
    }


    void Pipe::EndErrorRead() {
        readingError = false;
        m_threads["error"].join();
        m_threads.erase("error");
    }

    /********************************************************** PRIVATE ***********************************************************************/

    void Pipe::_read_output_loop(std::string* ret) {
        while (readingOutput) {
            if (ReadOutput(ret)) {
                OnOutputReceived(*ret);
            }
        }
    };

    void Pipe::_read_error_loop(std::string* ret) {
        while (readingError) {
            if (ReadError(ret)) {
                OnErrorReceived(*ret);
            }
        }
    }

    bool Pipe::_InitInput() {
        if ((m_stdIn.inputFileDesc = _open_osfhandle((intptr_t)m_stdIn.write, 0)) == -1) {
            PRINT_ERROR(L"Pipe::_InitInput->_open_osfhandle");
            return false;
        }

        if ((m_stdIn.inputFile = _fdopen(m_stdIn.inputFileDesc, "w")) == NULL) {
            PRINT_ERROR(L"Pipe::_InitInput->_fdopen");
            return false;
        }

        m_stdIn.in = std::ofstream(m_stdIn.inputFile);
        return true;
    };

    bool Pipe::_InitOutput() {
        if ((m_stdOut.outputFileDesc = _open_osfhandle((intptr_t)m_stdOut.read, 0)) == -1) {
            PRINT_ERROR(L"Pipe::_InitOutput->_open_osfhandle");
            return false;
        }


        if ((m_stdOut.outputFile = _fdopen(m_stdOut.outputFileDesc, "r")) == NULL) {
            PRINT_ERROR(L"Pipe::_InitOutput->_fdopen");
            return false;
        }

        m_stdOut.out = std::ifstream(m_stdOut.outputFile);
        return true;
    };

    bool Pipe::_InitError() {
        if ((m_stdErr.errorFileDesc = _open_osfhandle((intptr_t)m_stdErr.read, 0)) == -1) {
            PRINT_ERROR(L"Pipe::_InitError->_open_osfhandle");
            return false;
        }

        if ((m_stdErr.errorFile = _fdopen(m_stdErr.errorFileDesc, "r")) == NULL) {
            PRINT_ERROR(L"Pipe::_InitError->_fdopen");
            return false;
        }

        m_stdErr.err = std::ifstream(m_stdErr.errorFile);
        return true;
    }
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_PIPE_CPP