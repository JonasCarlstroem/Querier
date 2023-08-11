#pragma once

#define BUFSIZE 4096

#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include "Error.h"
#include <io.h>


//Inter process communication structure
//  Contains handles to stdin and stdout
//  and file descriptors to direct handles to stream
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