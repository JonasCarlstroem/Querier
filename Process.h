#pragma once

#ifndef _SCRIPT_PAD_PROCESS_H
#define _SCRIPT_PAD_PROCESS_H

#define BUFSIZE 4096

#include "Pipe.h"
#include "Error.h"
#include "Stopwatch.h"
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <functional>


namespace scriptpad {

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
        bool RunInCmd = false;
    };

    class Process : public Pipe {
    public:
        ProcessStartInfo StartInfo;
        bool ProcessHasExited{ 0 };

        Process();
        Process(std::wstring FileName);
        Process(ProcessStartInfo startInfo);
        ~Process();

        void InitProcess();
        void SetFileName(std::wstring file);
        void SetCommandLine(std::wstring s);
        void AddCommandLineArgument(std::wstring key, std::wstring value);
        bool Start();
        void WaitForExit();
        void Write(std::string data);
        bool Read(std::wstring* ret);
        bool Read(std::string* ret);
        double get_ElapsedTimeInMilliseconds();
        double get_ElapsedTimeInSeconds();
        static Process* Run(const ProcessStartInfo& startInfo);

        std::function<void()> OnProcessExited{ 0 };

    private:
        Stopwatch stopWatch;
        SECURITY_ATTRIBUTES m_secAttr;
        PROCESS_INFORMATION m_procInfo;
        STARTUPINFO m_startupInfo;
        bool m_bSuccess = false,
             m_hasRedirectedIO = false,
             m_isRunning = false,
             m_isMembersInitialized = false,
             m_isPipeInitialized = false,
             m_isExecInfoInitialized = false;

        static std::vector<Process*> _processes;

        static void CALLBACK OnWaitCallback(void* ctx, BOOLEAN timerOrWait);

        void InitMembers();
        void InitPipe();
        bool RegisterWaitCallback();
        bool _CreateProcess(const wchar_t*, wchar_t*, const wchar_t*, const wchar_t*);
        void CreateProcessSuccess();

        struct WAIT_CONTEXT {
            HANDLE hEv;
            HANDLE hProc;
            Process* proc;
            Stopwatch sw;
        };

    };

}   //namespace scriptpad

#endif  //_SCRIPT_PAD_PROCESS_H