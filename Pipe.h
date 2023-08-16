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
        Pipe(SECURITY_ATTRIBUTES& sa, PROCESS_INFORMATION& pi) : m_saAttr(sa), m_piProcInfo(pi) {};
        ~Pipe() {
            if(m_redirectedInput)
                CloseHandle(m_stdIn.write);

            if(m_redirectedOutput)
                CloseHandle(m_stdOut.write);

            if(m_redirectedError)
                CloseHandle(m_stdErr.write);
        };

        bool RedirectIO(bool input, bool output, bool error, STARTUPINFO& si) {
            m_redirectedInput = input;
            m_redirectedOutput = output;
            m_redirectedError = error;

            if (input || output || error) {
                si.dwFlags |= STARTF_USESTDHANDLES;
                m_saAttr.bInheritHandle = TRUE;
            }

            if (input) {
                if (!CreatePipe(&m_stdIn.read, &m_stdIn.write, &m_saAttr, 0)) {
#ifdef _DEBUG
                    error::PrintError(L"Error CreatePipe Input");
#endif
                    return false;
                }

                if (!SetHandleInformation(m_stdIn.write, HANDLE_FLAG_INHERIT, 0)) {
#ifdef _DEBUG
                    error::PrintError(L"ProcessPipe::redirect_io->SetHandleInformation");
#endif
                    return false;
                }

                if (!_InitInput())
                    return false;

                si.hStdInput = m_stdIn.read;
            }

            if (output) {
                if (!CreatePipe(&m_stdOut.read, &m_stdOut.write, &m_saAttr, 0)) {
#ifdef _DEBUG
                    error::PrintError(L"Error CreatePipe Output");
#endif
                    return false;
                }

                if (!SetHandleInformation(m_stdOut.read, HANDLE_FLAG_INHERIT, 0)) {
#ifdef _DEBUG
                    error::PrintError(L"ProcessPipe::redirect_io->SetHandleInformation");
#endif
                    return false;
                }

                if (!_InitOutput())
                    return false;

                si.hStdOutput = m_stdOut.write;
            }

            if (error) {
                if (!CreatePipe(&m_stdErr.read, &m_stdErr.write, &m_saAttr, 0)) {
#ifdef _DEBUG
                    error::PrintError(L"Pipe::RedirectIO->CreatePipe");
#endif
                    return false;
                }

                if (!SetHandleInformation(m_stdErr.read, HANDLE_FLAG_INHERIT, 0)) {
#ifdef _DEBUG
                    error::PrintError(L"Pipe::RedirectIO->SetHandleInformation");
#endif
                    return false;
                }

                if (!_InitError())
                    return false;

                si.hStdError = m_stdErr.write;
            }

            return input || output || error;
        };


        void WriteInput(std::string data) {
            if (m_ch.in) {
                if (m_ch.in.is_open()) {
                    m_ch.in.write(data.c_str(), data.length());
                    m_ch.in.close();
                }
            }
        };

        bool ReadOutput(std::wstring* ret) {
            std::string buf;
            if (ReadOutput(&buf)) {
                return util::string_to_wstring(buf, ret);
            }
            return false;
        };

        bool ReadOutput(std::string* ret) {
            if (m_ch.out) {
                return Read(&m_ch.out, ret);
            }
            return false;
        };

        bool ReadError(std::wstring* ret) {
            std::string buf;
            if (ReadError(&buf)) {
                return util::string_to_wstring(buf, ret);
            }
            return false;
        }

        bool ReadError(std::string* ret) {
            if (m_ch.err) {
                return Read(&m_ch.err, ret);
            }
            return false;
        }

        bool Read(std::ifstream* std, std::string* ret) {
            char buffer[BUFSIZE];

            ret->clear();
            while (std->read(buffer, sizeof(buffer)))
                ret->append(buffer, sizeof(buffer));
            ret->append(buffer, std->gcount());

            return std->gcount() > 0;
        }

        void ProcessStarted() {
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


        void BeginOutputRead() {
            readingOutput = true;
            m_threads.insert({ "output", std::thread(&Pipe::_read_output_loop, this, &m_retBuffer) });
        };


        void EndOutputRead() {
            readingOutput = false;
            m_threads["output"].join();
            m_threads.erase("output");
        };

    
        void BeginErrorRead() {
            readingError = true;
            m_threads.insert({ "error", std::thread(&Pipe::_read_error_loop, this, &m_errBuffer) });
        }


        void EndErrorRead() {
            readingError = false;
            m_threads["error"].join();
            m_threads.erase("error");
        }
        std::function<void(std::string)> OnOutputReceived;
        std::function<void(std::string)> OnErrorReceived;

    private:
        unsigned long dataWritten = 0, dataRead = 0;
        bool m_redirectedInput = false, 
             m_redirectedOutput = false, 
             m_redirectedError = false, 
             readingOutput = false,
             readingError = false;

        std::string m_retBuffer;
        std::string m_errBuffer;
        std::map<std::string, std::thread> m_threads;
        std::vector<std::thread> m_evThreads;
        std::mutex m_mutex;
        SECURITY_ATTRIBUTES& m_saAttr;
        PROCESS_INFORMATION& m_piProcInfo; 

        void _read_output_loop(std::string* ret) {
            while (readingOutput) {
                if (ReadOutput(ret)) {
                    OnOutputReceived(*ret);
                }
            }
        };

        void _read_error_loop(std::string* ret) {
            while (readingError) {
                if (ReadError(ret)) {
                    OnErrorReceived(*ret);
                }
            }
        }

        bool _InitInput() {
            if ((m_ch.inputFileDesc = _open_osfhandle((intptr_t)m_stdIn.write, 0)) == -1) {
#ifdef _DEBUG
                error::PrintError(L"IPC::init_input->_open_osfhandle");
#endif
                return false;
            }

            if ((m_ch.inputFile = _fdopen(m_ch.inputFileDesc, "w")) == NULL) {
#ifdef _DEBUG
                error::PrintError(L"IPC::init_input->_fdopen");
#endif
                return false;
            }

            m_ch.win = std::wofstream(m_ch.inputFile);
            m_ch.in = std::ofstream(m_ch.inputFile);
            return true;
        };

        bool _InitOutput() {
            if ((m_ch.outputFileDesc = _open_osfhandle((intptr_t)m_stdOut.read, 0)) == -1) {
#ifdef _DEBUG
                error::PrintError(L"IPC::init_output->_open_osfhandle");
#endif
                return false;
            }


            if ((m_ch.outputFile = _fdopen(m_ch.outputFileDesc, "r")) == NULL) {
#ifdef _DEBUG
                error::PrintError(L"IPC::init_output->_fdopen");
#endif
                return false;
            }

            m_ch.wout = std::wifstream(m_ch.outputFile);
            m_ch.out = std::ifstream(m_ch.outputFile);
            return true;
        };

        bool _InitError() {
            if ((m_ch.errorFileDesc = _open_osfhandle((intptr_t)m_stdErr.read, 0)) == -1) {
#ifdef _DEBUG
                error::PrintError(L"PIPE::_InitError->_open_osfhandle");
#endif
                return false;
            }

            if ((m_ch.errorFile = _fdopen(m_ch.errorFileDesc, "r")) == NULL) {
#ifdef _DEBUG
                error::PrintError(L"Pipe::_InitError->_fdopen");
#endif
                return false;
            }

            m_ch.werr = std::wifstream(m_ch.errorFile);
            m_ch.err = std::ifstream(m_ch.errorFile);
            return true;
        }

        struct Channel {
            int inputFileDesc = -1;
            int outputFileDesc = -1;
            int errorFileDesc = -1;
            FILE* inputFile = NULL;
            FILE* outputFile = NULL;
            FILE* errorFile = NULL;
            std::ofstream in;
            std::wofstream win;
            std::ifstream out;
            std::wifstream wout;
            std::ifstream err;
            std::wifstream werr;

        } m_ch;

        struct INPUT_HANDLES {
            HANDLE read = NULL;
            HANDLE write = NULL;
        } m_stdIn;

        struct OUTPUT_HANDLES {
            HANDLE read = NULL;
            HANDLE write = NULL;
        } m_stdOut;

        struct ERROR_HANDLES {
            HANDLE read = NULL;
            HANDLE write = NULL;
        } m_stdErr;
    };
}


#endif