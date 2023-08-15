#pragma once

#ifndef _PROCESS_H
#define _PROCESS_H

#define BUFSIZE 4096

#include "Pipe.h"
#include "Error.h"
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <stdio.h>

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
        std::string Environment;
        std::wstring wEnvironment;
        bool RedirectStdInput = false;
        bool RedirectStdOutput = false;
        bool RedirectStdError = false;
    };

    class Process {
    public:
        ProcessStartInfo StartInfo;

        Process() : secAttr(), procInfo(), pipe(secAttr, procInfo), startupInfo(), StartInfo() {
            secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
            secAttr.lpSecurityDescriptor = NULL;
            secAttr.bInheritHandle = FALSE;
        };

        Process(std::wstring FileName) : Process() {
            StartInfo.wFileName = FileName;
        };

        Process(ProcessStartInfo startInfo) : secAttr(), procInfo(), pipe(secAttr, procInfo), startupInfo(), StartInfo(startInfo) {
            secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
            secAttr.lpSecurityDescriptor = NULL;
            secAttr.bInheritHandle = FALSE;
        };

        ~Process() {
            CloseHandle(procInfo.hProcess);
            for (auto proc : _processes) {
                delete proc;
            }
        };

        bool Start() {
            if (&StartInfo.wFileName == nullptr)
                return false;

            InitMembers();

            const wchar_t* file = StartInfo.wFileName.c_str();
            wchar_t* cmd = StartInfo.wCommandLine.data();
            const wchar_t* wd = StartInfo.wWorkingDirectory.c_str();
            const wchar_t* env = StartInfo.wEnvironment.c_str();

            size_t fileLen = StartInfo.wFileName.length();
            size_t cmdLen = StartInfo.wCommandLine.length();
            size_t wdLen = StartInfo.wWorkingDirectory.length();
            size_t envLen = StartInfo.wEnvironment.length();

            bSuccess = CreateProcess(fileLen > 0 ? file : NULL,
                cmdLen > 0 ? cmd : NULL,
                NULL,
                NULL,
                hasRedirectedIO,
                CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT,
                /*envLen > 0 ? (LPVOID)env : */NULL,
                wdLen > 0 ? wd : NULL,
                &startupInfo,
                &procInfo);

#ifdef _DEBUG
            DWORD eCode;
            if (!GetExitCodeProcess(procInfo.hProcess, &eCode))
                error::PrintError(L"GetExitCodeProcess");
#endif


            if (!bSuccess) {
#ifdef _DEBUG
                error::PrintError(_T("Error creating process"));
#endif
                return false;
            }
            else {
                //CloseHandle(procInfo.hProcess);
                CloseHandle(procInfo.hThread);

                pipe.ProcessStarted();

                WaitForSingleObject(procInfo.hProcess, INFINITE);
                CloseHandle(procInfo.hProcess);
                pipe.EndOutputRead();
                return true;
            }
        };

        void Write(std::string data) {
            pipe.Write(data);
        };
        bool wRead(std::wstring* ret) {
            return pipe.wRead(ret);
        };
        bool Read(std::wstring* ret) {
            return pipe.Read(ret);
        };
        bool Read(std::string* ret) {
            return pipe.Read(ret);
        };

        static Process* Run(const ProcessStartInfo& startInfo) {
            Process* proc = new Process(startInfo);
            _processes.push_back(proc);
            proc->Start();
            return proc;
        };

        std::function<void(std::string)> OnOutputReceived;

    private:
        SECURITY_ATTRIBUTES secAttr;
        PROCESS_INFORMATION procInfo;
        STARTUPINFO startupInfo;
        pipe::Pipe pipe;
        BOOL bSuccess = FALSE;
        BOOL hasRedirectedIO = FALSE;
        bool eventHooked{ 0 };

        static std::vector<Process*> _processes;

        void InitMembers() {
            ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));
            ZeroMemory(&startupInfo, sizeof(STARTUPINFO));

            startupInfo.cb = sizeof(STARTUPINFO);
            if (pipe.RedirectIO(StartInfo.RedirectStdInput, StartInfo.RedirectStdOutput, startupInfo)) {
                pipe.OnOutputReceived = OnOutputReceived;
                hasRedirectedIO = true;
                pipe.BeginOutputRead();
            }
        };

    };

    std::vector<Process*> Process::_processes;
}

#endif