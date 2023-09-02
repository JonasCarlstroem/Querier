#pragma once

#ifndef _SCRIPT_PAD_PIPE_H
#define _SCRIPT_PAD_PIPE_H

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
#include <map>

#define BUFSIZE 4096

namespace querier {
    class Pipe {
    public:
        Pipe(SECURITY_ATTRIBUTES& sa, PROCESS_INFORMATION& pi);
        ~Pipe();

        bool RedirectIO(bool input, bool output, bool error, STARTUPINFOA& sia);
        bool RedirectIO(bool input, bool output, bool error, STARTUPINFO& si);
        void WriteInput(std::string data);
        bool ReadOutput(std::wstring* ret);
        bool ReadOutput(std::string* ret);
        bool ReadError(std::wstring* ret);
        bool ReadError(std::string* ret);
        bool Read(std::ifstream* std, std::string* ret);
        void ProcessStarted();
        void BeginOutputRead();
        void BeginOutputRead(std::string);
        void EndOutputRead();

        void BeginErrorRead();
        void BeginErrorRead(std::string);
        void EndErrorRead();

        std::function<void(std::string)> OnOutputReceived;
        std::function<void(std::string, std::string)> OnOutputReceivedQ;
        std::function<void(std::string)> OnErrorReceived;
        std::function<void(std::string, std::string)> OnErrorReceivedQ;
        
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

        void _read_output_loop(std::string* ret);
        void _read_output_loop_p(std::string* ret, std::string param);

        void _read_error_loop(std::string* ret);
        void _read_error_loop_p(std::string* ret, std::string param);

        bool _InitInput();
        bool _InitOutput();
        bool _InitError();

        struct STD_INPUT {
            HANDLE read;
            HANDLE write;
            int inputFileDesc = -1;
            FILE* inputFile = NULL;
            std::ofstream in;
        } m_stdIn;

        struct STD_OUTPUT {
            HANDLE read;
            HANDLE write;
            int outputFileDesc = -1;
            FILE* outputFile = NULL;
            std::ifstream out;
        } m_stdOut;

        struct STD_ERROR {
            HANDLE read;
            HANDLE write;
            int errorFileDesc = -1;
            FILE* errorFile = NULL;
            std::ifstream err;
        } m_stdErr;
    };

}   //namespace scriptpad

#endif  //_SCRIPT_PAD_PIPE_H