#pragma once
#define BUFSIZE 4096

#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <stdio.h>
#include "Pipe.h"

//Specify start info
struct ProcessStartInfo {
    std::wstring FileName;
    std::wstring WorkingDirectory;
    std::vector<std::wstring> CommandArguments;
    std::wstring CommandLine;
    bool RedirectStdInput = false;
    bool RedirectStdOutput = false;
    bool RedirectStdError = false;
};

class Process {
public:
    ProcessStartInfo StartInfo;

    Process(std::wstring FileName);
    Process(ProcessStartInfo startInfo);
    ~Process();

    bool Start();
    void Write(std::string data);
    void Read();
    void OnOutputReceived(const wchar_t* data);

    static Process* Run(const ProcessStartInfo&);

private:
    SECURITY_ATTRIBUTES secAttr;
    PROCESS_INFORMATION procInfo;
    STARTUPINFO startupInfo;
    ProcessPipe pipe;
    BOOL bSuccess = FALSE;
    BOOL hasRedirectedIO = FALSE;
    bool eventHooked{ 0 };

    Process();

    static std::vector<Process*> _processes;

    void init_members();

};