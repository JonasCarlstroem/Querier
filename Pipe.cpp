#ifndef _PIPE_CPP
    #define _PIPE_CPP

#include "Pipe.h"
#include "Util.h"
#include "Error.h"

namespace pipe {
    Pipe::Pipe(SECURITY_ATTRIBUTES& sa, PROCESS_INFORMATION& pi) : saAttr(sa), piProcInfo(pi) {}

    Pipe::~Pipe() {
        CloseHandle(hInput.write);
        CloseHandle(hInput.read);
        CloseHandle(hOutput.write);
        CloseHandle(hOutput.read);
    }


    bool Pipe::RedirectIO(bool input, bool output, STARTUPINFO& si) {
        if (input || output) {
            si.dwFlags |= STARTF_USESTDHANDLES;
        }

        if (input) {
            saAttr.bInheritHandle = TRUE;
            if (!CreatePipe(&hInput.read, &hInput.write, &saAttr, 0)) {
#ifdef _DEBUG
                error::PrintError(L"Error CreatePipe Input");
#endif
                return false;
            }

            if (!SetHandleInformation(hInput.write, HANDLE_FLAG_INHERIT, 0)) {
#ifdef _DEBUG
                error::PrintError(L"ProcessPipe::redirect_io->SetHandleInformation");
#endif
                return false;
            }

            if (!_InitInput())
                return false;

            si.hStdInput = hInput.read;
        }

        if (output) {
            saAttr.bInheritHandle = TRUE;
            if (!CreatePipe(&hOutput.read, &hOutput.write, &saAttr, 0)) {
#ifdef _DEBUG
                error::PrintError(L"Error CreatePipe Output");
#endif
                return false;
            }

            if (!SetHandleInformation(hOutput.read, HANDLE_FLAG_INHERIT, 0)) {
#ifdef _DEBUG
                error::PrintError(L"ProcessPipe::redirect_io->SetHandleInformation");
#endif
                return false;
            }

            if (!_InitOutput())
                return false;

            si.hStdOutput = hOutput.write;
            si.hStdError = hOutput.write;
        }

        return input || output;
    }


    void Pipe::Write(std::string data) {
        _Write(data.c_str());
    }


    void Pipe::_Write(const char* data) {
        if (ch.inputFileDesc > -1) {
            if (ch.inputFile != NULL) {
                const int cbData = strlen(data);
                ch.in = std::ofstream(ch.inputFile);

                if (ch.in.is_open()) {
                    ch.in.write(data, cbData);
                    ch.in.close();
                }
            }
        }
    }

    //public
    bool Pipe::wRead(std::wstring* ret) {
        _Read(&retBuffer);
        return util::string_to_wstring(retBuffer, ret);
    }

    //public
    bool Pipe::Read(std::string* ret) {
        _Read(ret);
        return true;
    }

    //private
    bool Pipe::_Read(std::string* ret) {
        if (ch.outputFileDesc > -1) {
            if (ch.outputFile != NULL) {
                std::string re2;

                char buffer[BUFSIZE];
                while (ch.out.read(buffer, sizeof(buffer)))
                    ret->append(buffer, sizeof(buffer));
                ret->append(buffer, ch.out.gcount());
            }
        }
    }


    void Pipe::ProcessStarted() {
        CloseHandle(hOutput.write);
        CloseHandle(hInput.read);
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

        ch.out = std::ifstream(ch.outputFile);
        return true;
    }
}

#endif