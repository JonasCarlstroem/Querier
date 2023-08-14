#pragma once

#ifndef _PIPE_H
#define _PIPE_H

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
        Pipe(SECURITY_ATTRIBUTES&, PROCESS_INFORMATION&);
        ~Pipe();

        bool RedirectIO(bool, bool, STARTUPINFO&);
        void wWrite(std::wstring data);
        void Write(std::string data);
        bool wRead(std::wstring*);
        bool Read(std::wstring*);
        bool Read(std::string*);
        void ProcessStarted();
        void BeginOutputRead();
        void EndOutputRead();

        void (*OnOutputReceived)(std::string, std::thread*) { 0 };

    private:
        unsigned long dataWritten = 0, dataRead = 0;
        bool outputReading = false;
        std::wstring wRetBuffer;
        std::string retBuffer;
        std::thread evThread;
        std::mutex mutex;
        SECURITY_ATTRIBUTES& saAttr;
        PROCESS_INFORMATION& piProcInfo; 

        void _wWrite(const wchar_t*, int len);
        void _Write(const char*, int len);
        bool _wRead(std::wstring*);
        bool _Read(std::string*);

        void _read_loop();

        bool _InitInput();
        bool _InitOutput();

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