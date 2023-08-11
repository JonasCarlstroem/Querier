#pragma once

#ifndef _IPC_H
#define _IPC_H

#define BUFSIZE 4096

#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include "Error.h"
#include <io.h>

namespace ipc {
    class IPC {
    public:
        void write(const char*);
        void read();

        bool init_input(HANDLE*);
        bool init_output(HANDLE*);
    private:
        int inputFileDesc = -1;
        int outputFileDesc = -1;
        FILE* inputFile = NULL;
        FILE* outputFile = NULL;
        std::ofstream in;
        std::ifstream out;
    };
}

#endif