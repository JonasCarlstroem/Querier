#include "Pipe.h"

ProcessPipe::ProcessPipe(SECURITY_ATTRIBUTES& sa, PROCESS_INFORMATION& pi) : saAttr(sa), piProcInfo(pi) {
}


ProcessPipe::~ProcessPipe() {
    CloseHandle(hInput.write);
    CloseHandle(hInput.read);
    CloseHandle(hOutput.write);
    CloseHandle(hOutput.read);
}


bool ProcessPipe::redirect_io(bool input, bool output, STARTUPINFO& si) {
    if (input || output) {
        si.dwFlags |= STARTF_USESTDHANDLES;
    }

    if (input) {
        saAttr.bInheritHandle = TRUE;
        if (!CreatePipe(&hInput.read, &hInput.write, &saAttr, 0)) {
            PrintError(L"Error CreatePipe Input");
            return false;
        }

        if (!SetHandleInformation(hInput.write, HANDLE_FLAG_INHERIT, 0)) {
            PrintError(L"ProcessPipe::redirect_io->SetHandleInformation");
            return false;
        }

        if (!ipc.init_input(&hInput.write))
            return false;

        si.hStdInput = hInput.read;
    }

    if (output) {
        saAttr.bInheritHandle = TRUE;
        if (!CreatePipe(&hOutput.read, &hOutput.write, &saAttr, 0)) {
            PrintError(L"Error CreatePipe Output");
            return false;
        }

        if (!SetHandleInformation(hOutput.read, HANDLE_FLAG_INHERIT, 0)) {
            PrintError(L"ProcessPipe::redirect_io->SetHandleInformation");
            return false;
        }

        if (!ipc.init_output(&hOutput.read))
            return false;

        si.hStdOutput = hOutput.write;
        si.hStdError = hOutput.write;
    }

    return input || output;
}


void ProcessPipe::write(std::string data) {
    ipc.write(data.c_str());
}


void ProcessPipe::read() {
    ipc.read();
}


void ProcessPipe::start_read() {
    evThread = std::thread(&ProcessPipe::_read_loop, this);
}


void ProcessPipe::stop_read() {
    reading = false;
    evThread.join();
}


void ProcessPipe::process_started() {
    CloseHandle(hOutput.write);
    CloseHandle(hInput.read);
}


void ProcessPipe::_read_loop() {
    reading = true;
    wprintf_s(L"Begin reading from std out ");
    wprintf_s(L"%d\n", reading);
    while (reading) {
        mutex.lock();
        read();
        mutex.unlock();
    }
}