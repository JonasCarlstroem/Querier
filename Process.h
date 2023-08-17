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
            /*for (auto proc : _processes) {
                delete proc;
            }*/
        };

        bool Start() {
            _processes.push_back(this);

            InitMembers();

            const wchar_t* file = StartInfo.wFileName.c_str();
            wchar_t* cmd = StartInfo.wCommandLine.data();
            const wchar_t* wd = StartInfo.wWorkingDirectory.c_str();
            const wchar_t* env = StartInfo.wEnvironment.c_str();

            size_t fileLen = StartInfo.wFileName.length();
            size_t cmdLen = StartInfo.wCommandLine.length();
            size_t wdLen = StartInfo.wWorkingDirectory.length();
            size_t envLen = StartInfo.wEnvironment.length();

            stopWatch.Start();

            if (CreateProcess(fileLen > 0 ? file : NULL,
                              cmdLen > 0 ? cmd : NULL,
                              NULL,
                              NULL,
                              m_hasRedirectedIO,
                              CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT,
                              /*envLen > 0 ? (LPVOID)env : */NULL,
                              wdLen > 0 ? wd : NULL,
                              &m_startupInfo,
                              &m_procInfo)) {
                m_isRunning = true;
                HANDLE hWait = NULL;
                HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

                struct CTX {
                    HANDLE hEv;
                    HANDLE hProc;
                    Process* proc;
                    stopwatch::Stopwatch sw;
                } *ctx = new CTX{ hEvent, m_procInfo.hProcess, this, stopWatch };

                if (!RegisterWaitForSingleObject(&hWait, m_procInfo.hProcess, [](void* ctx, BOOLEAN timerOrWait) -> void {
                    CTX* context = (CTX*)ctx;
                    context->sw.Stop();
                    CloseHandle(context->hProc);
                    context->proc->ProcessHasExited = true;
                    context->proc->OnProcessExited();

                    delete ctx;

                }, ctx, INFINITE, WT_EXECUTEONLYONCE)) {
                    stopWatch.Stop();
#ifdef _DEBUG
                    error::PrintError(L"RegisterWaitForSingleObject");
#endif
                    return false;
                }

                CloseHandle(m_procInfo.hThread);
                ProcessStarted();
                return true;
            }
            else {
#ifdef _DEBUG
                DWORD eCode;
                if (!GetExitCodeProcess(m_procInfo.hProcess, &eCode))
                    error::PrintError(L"GetExitCodeProcess");
#endif
                return false;
            }
        };

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
        bool m_bSuccess = FALSE,
             m_hasRedirectedIO = FALSE,
             m_isRunning = FALSE;

        static std::vector<Process*> _processes;

        void InitMembers() {
            ZeroMemory(&m_procInfo, sizeof(PROCESS_INFORMATION));
            ZeroMemory(&m_startupInfo, sizeof(STARTUPINFO));

            m_startupInfo.cb = sizeof(STARTUPINFO);
            m_hasRedirectedIO = RedirectIO(StartInfo.RedirectStdInput,
                                         StartInfo.RedirectStdOutput,
                                         StartInfo.RedirectStdError,
                                         m_startupInfo);
        };

    };

    std::vector<Process*> Process::_processes;
}

#endif