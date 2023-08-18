#pragma once

#ifndef _PROCESS_H
#define _PROCESS_H

#define BUFSIZE 4096

#include "Pipe.h"
#include "Error.h"
#include "Stopwatch.h"
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
        bool RunInCmd = false;
    };

    class Process : public pipe::Pipe {
    public:
        ProcessStartInfo StartInfo;
        bool ProcessHasExited{ 0 };

        Process() : m_secAttr(), m_procInfo(), m_startupInfo(), StartInfo(), Pipe(m_secAttr, m_procInfo) {
            m_secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
            m_secAttr.lpSecurityDescriptor = NULL;
            m_secAttr.bInheritHandle = FALSE;
        };

        Process(std::wstring FileName) : Process() {
            StartInfo.wFileName = FileName;
        };

        Process(ProcessStartInfo startInfo) : m_secAttr(), m_procInfo(), m_startupInfo(), StartInfo(startInfo), Pipe(m_secAttr, m_procInfo) {
            m_secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
            m_secAttr.lpSecurityDescriptor = NULL;
            m_secAttr.bInheritHandle = FALSE;
        };

        ~Process() {
    
        };

        void InitProcess() {
            InitMembers();
            if (StartInfo.RedirectStdInput || StartInfo.RedirectStdOutput || StartInfo.RedirectStdError)
                InitPipe();
        }

        void SetFileName(std::wstring file) {
            StartInfo.wFileName = file;
        }

        void SetCommandLine(std::wstring s) {
            StartInfo.wCommandLine = s;
        }

        void AddCommandLineArgument(std::wstring key, std::wstring value) {
            StartInfo.wCommandLine = std::format(L"{0} {1} {2}", StartInfo.wCommandLine, key, value);
        }

        bool Start() {
            if (!m_isMembersInitialized)
                InitMembers();

            if (!m_isPipeInitialized)
                InitPipe();

            stopWatch.Start();

            if (StartInfo.RunInCmd) {
                if (CreateProcess(NULL,
                                  StartInfo.wCommandLine.data(),
                                  NULL,
                                  NULL,
                                  m_hasRedirectedIO,
                                  CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT,
                                  StartInfo.wEnvironment.length() > 0 ? (LPVOID)StartInfo.wEnvironment.c_str() : NULL,
                                  StartInfo.wWorkingDirectory.length() > 0 ? StartInfo.wWorkingDirectory.c_str() : NULL,
                                  &m_startupInfo,
                                  &m_procInfo)) {
                    m_isRunning = true;

                    CreateProcessSuccess();
                    return true;
                }
                else {
                    PRINT_ERROR(L"CreateProcess");
                    return false;
                }
            }
            else {
                if (CreateProcess(StartInfo.wFileName.c_str(),
                                  StartInfo.wCommandLine.length() > 0 ? StartInfo.wCommandLine.data() : NULL,
                                  NULL,
                                  NULL,
                                  m_hasRedirectedIO,
                                  CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT,
                                  StartInfo.wEnvironment.length() > 0 ? (LPVOID)StartInfo.wEnvironment.c_str() : NULL,
                                  StartInfo.wWorkingDirectory.length() > 0 ? StartInfo.wWorkingDirectory.c_str() : NULL,
                                  &m_startupInfo,
                                  &m_procInfo)) {
                    m_isRunning = true;

                    CreateProcessSuccess();
                    return true;
                }
                else {
                    PRINT_ERROR(L"CreateProcess");
                    return false;
                }
            }
        }

        void WaitForExit() {
            WaitForSingleObject(m_procInfo.hProcess, INFINITE);
        }

        void Write(std::string data) {
            Pipe::WriteInput(data);
        };

        bool Read(std::wstring* ret) {
            return Pipe::ReadOutput(ret);
        };

        bool Read(std::string* ret) {
            return Pipe::ReadOutput(ret);
        };

        double get_ElapsedTimeInMilliseconds() {
            return stopWatch.elapsedMilliSeconds();
        }

        double get_ElapsedTimeInSeconds() {
            return stopWatch.elapsedSeconds();
        }

        static Process* Run(const ProcessStartInfo& startInfo) {
            Process* proc = new Process(startInfo);
            proc->Start();
            return proc;
        };

        std::function<void()> OnProcessExited{ 0 };

    private:
        stopwatch::Stopwatch stopWatch;
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

        void InitMembers() {
            ZeroMemory(&m_procInfo, sizeof(PROCESS_INFORMATION));
            ZeroMemory(&m_startupInfo, sizeof(STARTUPINFO));
            m_startupInfo.cb = sizeof(STARTUPINFO);
            m_isMembersInitialized = true;
        };

        void InitPipe() {
            m_hasRedirectedIO = RedirectIO(StartInfo.RedirectStdInput,
                                           StartInfo.RedirectStdOutput,
                                           StartInfo.RedirectStdError,
                                           m_startupInfo);
            m_isPipeInitialized = true;
        }

        bool RegisterWaitCallback() {
            HANDLE hWait = NULL;
            HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

            WAIT_CONTEXT* ctx = new WAIT_CONTEXT{ hEvent, m_procInfo.hProcess, this, stopWatch };
            struct WCTX {
                HANDLE hEv;
                HANDLE hProc;
                Process* proc;
                stopwatch::Stopwatch sw;
            } *wctx = new WCTX{ hEvent, m_procInfo.hProcess, this, stopWatch };

            return RegisterWaitForSingleObject(&hWait, m_procInfo.hProcess, [](void* ctx, BOOLEAN timerOrWait) -> void {
                WAIT_CONTEXT* context = (WAIT_CONTEXT*)ctx;
                context->sw.Stop();
                CloseHandle(context->hProc);
                context->proc->ProcessHasExited = true;

                if (context->proc->OnProcessExited)
                    context->proc->OnProcessExited();

                delete ctx;

            }, wctx, INFINITE, WT_EXECUTEONLYONCE);
        }

        void CreateProcessSuccess() {
            if (!RegisterWaitCallback()) {
                PRINT_ERROR(L"RegisterWaitForSingleObject");
            }

            CloseHandle(m_procInfo.hThread);
            ProcessStarted();
        }

        struct WAIT_CONTEXT {
            HANDLE hEv;
            HANDLE hProc;
            Process* proc;
            stopwatch::Stopwatch sw;
        };

    };

    std::vector<Process*> Process::_processes;
}

#endif