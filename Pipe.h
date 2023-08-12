#pragma once

#ifndef _PIPE_H
#define _PIPE_H

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <io.h>

#define BUFSIZE 4096

namespace pipe {
    struct INPUT_HANDLES {
        HANDLE read = NULL;
        HANDLE write = NULL;
    };

    struct OUTPUT_HANDLES {
        HANDLE read = NULL;
        HANDLE write = NULL;
    };

    class Pipe {
    public:
        Pipe(SECURITY_ATTRIBUTES&, PROCESS_INFORMATION&);
        ~Pipe();

        bool RedirectIO(bool, bool, STARTUPINFO&);

        void Write(std::string data);
        bool wRead(std::wstring*);
        bool Read(std::string*);
        void ProcessStarted();

    private:
        unsigned long dataWritten = 0, dataRead = 0;
        bool reading = false;
        std::wstring wRetBuffer;
        std::string retBuffer;
        std::thread evThread;
        std::mutex mutex;
        SECURITY_ATTRIBUTES& saAttr;
        PROCESS_INFORMATION& piProcInfo;
        INPUT_HANDLES hInput;
        OUTPUT_HANDLES hOutput;

        void _Write(const char*);
        bool _Read(std::string*);

        bool _InitInput();
        bool _InitOutput();

        struct Channel {
            int inputFileDesc = -1;
            int outputFileDesc = -1;
            FILE* inputFile = NULL;
            FILE* outputFile = NULL;
            std::ofstream in;
            std::ifstream out;
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