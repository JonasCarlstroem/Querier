#pragma once

#ifndef _PIPE_H
#define _PIPE_H

#include "Process.h"
#include "Error.h"
#include "Util.h"
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <future>
#include <mutex>
#include <fstream>
#include <io.h>

#define BUFSIZE 4096


namespace pipe {
    class Pipe {
    public:
        Pipe(SECURITY_ATTRIBUTES& sa, PROCESS_INFORMATION& pi) : saAttr(sa), piProcInfo(pi) {};
        ~Pipe() {
            CloseHandle(stdIn.write);
            CloseHandle(stdIn.read);
            CloseHandle(stdOut.write);
            CloseHandle(stdOut.read);
        };

        bool RedirectIO(bool input, bool output, STARTUPINFO& si) {
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
        };
        void wWrite(std::wstring data) {
            _wWrite(data.c_str(), data.length());
        };
        void Write(std::string data) {
            _Write(data.c_str(), data.length());
        };
        bool wRead(std::wstring* ret) {
            return _wRead(ret);
        };
        bool Read(std::wstring* ret) {
            std::string buf;
            if (_Read(&buf)) {
                return util::string_to_wstring(buf, ret);
            }
            return false;
        };
        bool Read(std::string* ret) {
            return _Read(ret);
        };
        void ProcessStarted() {
            CloseHandle(stdOut.write);
            CloseHandle(stdIn.read);
            CloseHandle(stdIn.write);
        };
        void BeginOutputRead() {
            outputReading = true;
            evThread = std::thread(&Pipe::_read_loop, this, &retBuffer);
        };
        void EndOutputRead() {
            outputReading = false;
            evThread.join();
        };

        std::function<void(std::string)> OnOutputReceived;

    private:
        unsigned long dataWritten = 0, dataRead = 0;
        bool outputReading = false;
        std::wstring wRetBuffer;
        std::string retBuffer;
        std::thread evThread;
        std::mutex mutex;
        SECURITY_ATTRIBUTES& saAttr;
        PROCESS_INFORMATION& piProcInfo; 

        void _wWrite(const wchar_t* data, int len) {
            if (ch.win) {
                if (ch.win.is_open()) {
                    ch.win.write(data, len);
                    ch.win.close();
                }
            }
        };
        void _Write(const char* data, int len) {
            if (ch.in) {
                if (ch.in.is_open()) {
                    ch.in.write(data, len);
                    ch.in.close();
                }
            }
        };
        bool _wRead(std::wstring* ret) {
            if (ch.wout) {
                wchar_t buffer[BUFSIZE];

                while (ch.wout.read(buffer, sizeof(buffer)))
                    ret->append(buffer, sizeof(buffer));
                ret->append(buffer, ch.wout.gcount());

                return true;
            }
            return false;
        };
        bool _Read(std::string* ret) {
            if (ch.out) {
                char buffer[BUFSIZE];

                ret->clear();
                while (ch.out.read(buffer, sizeof(buffer)))
                    ret->append(buffer, sizeof(buffer));
                ret->append(buffer, ch.out.gcount());

                return ch.out.gcount() > 0;
            }
            return false;
        };

        void _read_loop(std::string* ret) {
        //std::string ret;
            while (outputReading) {
                if (_Read(ret)) {
                    OnOutputReceived(*ret);
                    //(this->*OnOutputReceived2)(*ret);
                }
            }
        };

        bool _InitInput() {
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
        };
        bool _InitOutput() {
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
        };

        struct Channel {
            int inputFileDesc = -1;
            int outputFileDesc = -1;
            FILE* inputFile = NULL;
            FILE* outputFile = NULL;
            std::ofstream in;
            std::ifstream out;
            std::wofstream win;
            std::wifstream wout;
        } ch;

        struct INPUT_HANDLES {
            HANDLE read = NULL;
            HANDLE write = NULL;
        } stdIn;

        struct OUTPUT_HANDLES {
            HANDLE read = NULL;
            HANDLE write = NULL;
        } stdOut;
    };
}


#endif