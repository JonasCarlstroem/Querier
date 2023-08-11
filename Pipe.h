#pragma once

#ifndef _PIPE_H
#define _PIPE_H

#include "IPC.h"
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

    class ProcessPipe {
    public:
        ProcessPipe(SECURITY_ATTRIBUTES&, PROCESS_INFORMATION&);
        ~ProcessPipe();

        bool redirect_io(bool, bool, STARTUPINFO&);

        void write(std::string data);
        void read();
        void start_read();
        void stop_read();
        void process_started();

    private:
        unsigned long dataWritten = 0, dataRead = 0;
        bool reading = false;
        std::wstring buffer;
        std::thread evThread;
        std::mutex mutex;
        SECURITY_ATTRIBUTES& saAttr;
        PROCESS_INFORMATION& piProcInfo;
        INPUT_HANDLES hInput;
        OUTPUT_HANDLES hOutput;
        ipc::IPC ipc;

        void _redirect_input();
        void _redirect_output();
        void _write(const char* data);
        void _read_loop();
    };
}


#endif