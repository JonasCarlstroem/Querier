#ifndef _PIPE_CPP
#define _PIPE_CPP

#include "Pipe.h"
#include "Util.h"
#include "Error.h"

namespace pipe {
    Pipe::Pipe(SECURITY_ATTRIBUTES& sa, PROCESS_INFORMATION& pi) : saAttr(sa), piProcInfo(pi) {}

    Pipe::~Pipe() {
        CloseHandle(stdIn.write);
        CloseHandle(stdIn.read);
        CloseHandle(stdOut.write);
        CloseHandle(stdOut.read);
    }


    bool Pipe::RedirectIO(bool input, bool output, STARTUPINFO& si) {
        if (input || output) {
            si.dwFlags |= STARTF_USESTDHANDLES;
            saAttr.bInheritHandle = TRUE;
        }

        if (input) {
            if (!CreatePipe(&stdIn.read, &stdIn.write, &saAttr, 0)) {
#ifdef _DEBUG
                error::PrintError(L"Error CreatePipe Input");
#endif
                return false;
            }

            if (!SetHandleInformation(stdIn.write, HANDLE_FLAG_INHERIT, 0)) {
#ifdef _DEBUG
                error::PrintError(L"ProcessPipe::redirect_io->SetHandleInformation");
#endif
                return false;
            }

            if (!_InitInput())
                return false;

            si.hStdInput = stdIn.read;
        }

        if (output) {
            if (!CreatePipe(&stdOut.read, &stdOut.write, &saAttr, 0)) {
#ifdef _DEBUG
                error::PrintError(L"Error CreatePipe Output");
#endif
                return false;
            }

            if (!SetHandleInformation(stdOut.read, HANDLE_FLAG_INHERIT, 0)) {
#ifdef _DEBUG
                error::PrintError(L"ProcessPipe::redirect_io->SetHandleInformation");
#endif
                return false;
            }

            if (!_InitOutput())
                return false;

            si.hStdOutput = stdOut.write;
            si.hStdError = stdOut.write;
        }

        return input || output;
    }


    void Pipe::wWrite(std::wstring data) {
        _wWrite(data.c_str(), data.length());
    }


    void Pipe::Write(std::string data) {
        _Write(data.c_str(), data.length());
    }


    //public
    bool Pipe::wRead(std::wstring* ret) {
        return _wRead(ret);
    }


    bool Pipe::Read(std::wstring* ret) {
        std::string buf;
        if (_Read(&buf)) {
            return util::string_to_wstring(buf, ret);
        }
        return false;
    }

    //public
    bool Pipe::Read(std::string* ret) {
        return _Read(ret);
    }

    void Pipe::BeginOutputRead() {
        outputReading = true;
        evThread = std::thread(&Pipe::_read_loop, this);
    }

    void Pipe::EndOutputRead() {
        outputReading = false;
        evThread.join();
    }


    void Pipe::_wWrite(const wchar_t* data, int len) {
        if (ch.win) {
            if (ch.win.is_open()) {
                ch.win.write(data, len);
                ch.win.close();
            }
        }
    }


    void Pipe::_Write(const char* data, int len) {
        if (ch.in) {
            if (ch.in.is_open()) {
                ch.in.write(data, len);
                ch.in.close();
            }
        }
    }


    //private
    bool Pipe::_wRead(std::wstring* ret) {
        if (ch.wout) {
            wchar_t buffer[BUFSIZE];

            while (ch.wout.read(buffer, sizeof(buffer)))
                ret->append(buffer, sizeof(buffer));
            ret->append(buffer, ch.wout.gcount());

            return true;
        }
        return false;
    }


    //private
    bool Pipe::_Read(std::string* ret) {
        if (ch.out) {
            char buffer[BUFSIZE];
            
            ret->clear();
            while (ch.out.read(buffer, sizeof(buffer)))
                ret->append(buffer, sizeof(buffer));
            ret->append(buffer, ch.out.gcount());

            return ch.out.gcount() > 0;
        }
        return false;
    }


    void Pipe::ProcessStarted() {
        CloseHandle(stdOut.write);
        CloseHandle(stdIn.read);
        CloseHandle(stdIn.write);
    }

    void Pipe::_read_loop() {
        std::string ret;
        int count = 0;
        while (outputReading) {
            if (_Read(&ret)) {
                OnOutputReceived(ret, &evThread);
            }
            count++;
        }
        util::print_message(std::to_wstring(count));
    }

    bool Pipe::_InitInput() {
        if ((ch.inputFileDesc = _open_osfhandle((intptr_t)stdIn.write, 0)) == -1) {
#ifdef _DEBUG
            error::PrintError(L"IPC::init_input->_open_osfhandle");
#endif
            return false;
        }

        if ((ch.inputFile = _fdopen(ch.inputFileDesc, "w")) == NULL) {
#ifdef _DEBUG
            error::PrintError(L"IPC::init_input->_fdopen");
#endif
            return false;
        }

        ch.win = std::wofstream(ch.inputFile);
        ch.in = std::ofstream(ch.inputFile);
        return true;
    }

    bool Pipe::_InitOutput() {
        if ((ch.outputFileDesc = _open_osfhandle((intptr_t)stdOut.read, 0)) == -1) {
#ifdef _DEBUG
            error::PrintError(L"IPC::init_output->_open_osfhandle");
#endif
            return false;
        }


        if ((ch.outputFile = _fdopen(ch.outputFileDesc, "r")) == NULL) {
#ifdef _DEBUG
            error::PrintError(L"IPC::init_output->_fdopen");
#endif
            return false;
        }

        ch.wout = std::wifstream(ch.outputFile);
        ch.out = std::ifstream(ch.outputFile);
        return true;
    }
}

#endif