#pragma once

#ifndef _PROCESS_H
#define _PROCESS_H

#define BUFSIZE 4096

#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <stdio.h>
#include "Pipe.h"

namespace process {
    struct ProcessStartInfo {
        std::string FileName;
        std::wstring wFileName;
        std::string WorkingDirectory;
        std::wstring wWorkingDirectory;
        std::vector<std::string> CommandArguments;
        std::vector<std::wstring> vwCommandArguments;
        std::string CommandLine;
        std::wstring wCommandLine;
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
        void Read(std::wstring*);
        void OnOutputReceived(const wchar_t* data);

        static Process* Run(const ProcessStartInfo&);

    private:
        SECURITY_ATTRIBUTES secAttr;
        PROCESS_INFORMATION procInfo;
        STARTUPINFO startupInfo;
        pipe::ProcessPipe pipe;
        BOOL bSuccess = FALSE;
        BOOL hasRedirectedIO = FALSE;
        bool eventHooked{ 0 };

        Process();

        static std::vector<Process*> _processes;

        void init_members();

    };
}

#endif