#ifndef _SCRIPT_PAD_PROCESS_CPP
#define _SCRIPT_PAD_PROCESS_CPP

#include "Process.h"

namespace querier {

    /********************************************************** PUBLIC ***********************************************************************/

    Process::Process() : m_secAttr(), m_procInfo(), m_startupInfo(), StartInfo(), Pipe(m_secAttr, m_procInfo) {
        m_secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        m_secAttr.lpSecurityDescriptor = NULL;
        m_secAttr.bInheritHandle = FALSE;
    };

    Process::Process(std::wstring FileName) : Process() {
        StartInfo.wFileName = FileName;
    };

    Process::Process(ProcessStartInfo startInfo) : m_secAttr(), m_procInfo(), m_startupInfo(), StartInfo(startInfo), Pipe(m_secAttr, m_procInfo) {
        m_secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        m_secAttr.lpSecurityDescriptor = NULL;
        m_secAttr.bInheritHandle = FALSE;
    };

    Process::~Process() {

    };

    void Process::InitProcess() {
        InitMembers();
        if (StartInfo.RedirectStdInput || StartInfo.RedirectStdOutput || StartInfo.RedirectStdError)
            InitPipe();
    }

    void Process::SetFileName(std::wstring file) {
        StartInfo.wFileName = file;
    }

    void Process::SetCommandLine(std::wstring s) {
        StartInfo.wCommandLine = s;
    }

    void Process::AddCommandLineArgument(std::wstring key, std::wstring value) {
        StartInfo.wCommandLine = std::format(L"{0} {1} {2}", StartInfo.wCommandLine, key, value);
    }

    bool Process::Start() {
        stopWatch.Start();

        if (StartInfo.wCommandLine.size() > 0) {
            if (_CreateProcess(StartInfo.RunInCmd ? nullptr : StartInfo.wFileName.c_str(),
                               StartInfo.wCommandLine.data(),
                               StartInfo.wEnvironment.c_str(),
                               StartInfo.wWorkingDirectory.c_str())) {
                m_isRunning = true;

                CreateProcessSuccess();
                return true;
            }
            else {
                PRINT_ERROR(L"CreateProcess");
                return false;
            }
        }
        else if (StartInfo.CommandLine.size() > 0) {
            if (_CreateProcess(StartInfo.RunInCmd ? nullptr : StartInfo.FileName.c_str(),
                               StartInfo.CommandLine.data(),
                               StartInfo.Environment.c_str(),
                               StartInfo.WorkingDirectory.c_str())) {
                m_isRunning = true;

                CreateProcessSuccess();
                return true;
            }
            else {
                PRINT_ERROR(L"CreateProcess");
                return false;
            }
        }
        else
            return false;

    }

    void Process::WaitForExit() {
        WaitForSingleObject(m_procInfo.hProcess, INFINITE);
    }

    void Process::Write(std::string data) {
        Pipe::WriteInput(data);
    };

    bool Process::Read(std::wstring* ret) {
        return Pipe::ReadOutput(ret);
    };

    bool Process::Read(std::string* ret) {
        return Pipe::ReadOutput(ret);
    };

    double Process::get_ElapsedTimeInMilliseconds() {
        return stopWatch.elapsedMilliSeconds();
    }

    double Process::get_ElapsedTimeInSeconds() {
        return stopWatch.elapsedSeconds();
    }

    Process* Process::Run(const ProcessStartInfo& startInfo) {
        Process* proc = new Process(startInfo);
        proc->Start();
        return proc;
    };

    /********************************************************** PRIVATE ***********************************************************************/

    void Process::InitMembers() {
        ZeroMemory(&m_procInfo, sizeof(PROCESS_INFORMATION));
        m_isMembersInitialized = true;
    };

    void Process::InitPipe() {
        m_hasRedirectedIO = RedirectIO(StartInfo.RedirectStdInput, 
                                       StartInfo.RedirectStdOutput, 
                                       StartInfo.RedirectStdError,
                                       m_startupInfo);
        m_isPipeInitialized = true;
    }

    void CALLBACK Process::OnWaitCallback(void* ctx, BOOLEAN timerOrWait) {
        WAIT_CONTEXT* context = (WAIT_CONTEXT*)ctx;
        context->sw.Stop();
        CloseHandle(context->hProc);
        context->proc->ProcessHasExited = true;

        if (context->proc->OnProcessExited)
            context->proc->OnProcessExited();

        delete ctx;
    }

    bool Process::RegisterWaitCallback() {
        HANDLE hWait = NULL;
        HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

        WAIT_CONTEXT* ctx = new WAIT_CONTEXT{ hEvent, m_procInfo.hProcess, this, stopWatch };
        struct WCTX {
            HANDLE hEv;
            HANDLE hProc;
            Process* proc;
            Stopwatch sw;
        } *wctx = new WCTX{ hEvent, m_procInfo.hProcess, this, stopWatch };

        return RegisterWaitForSingleObject(&hWait, m_procInfo.hProcess, OnWaitCallback, wctx, INFINITE, WT_EXECUTEONLYONCE);
    }

    bool Process::_BeforeCreateProcess(bool unicode) {
        ZeroMemory(&m_procInfo, sizeof(PROCESS_INFORMATION));
        if (unicode) {
            ZeroMemory(&m_startupInfo, sizeof(STARTUPINFO));
        }
        else {
            ZeroMemory(&m_startupInfoA, sizeof(STARTUPINFOA));
        }

        m_startupInfo.cb = sizeof(STARTUPINFO);
        if (unicode) {
            m_hasRedirectedIO = RedirectIO(StartInfo.RedirectStdInput,
                                           StartInfo.RedirectStdOutput,
                                           StartInfo.RedirectStdError,
                                           m_startupInfo);
        }
        else {
            m_hasRedirectedIO = RedirectIO(StartInfo.RedirectStdInput,
                                           StartInfo.RedirectStdOutput,
                                           StartInfo.RedirectStdError,
                                           m_startupInfoA);
        }

        return true;
    }

    bool Process::_CreateProcess(const char* file, char* cmd, const char* env, const char* cwd) {
        if (_BeforeCreateProcess(false))
            return CreateProcessA(lstrlenA(file) > 0 ? file : NULL,
                                  lstrlenA(cmd) > 0 ? cmd : NULL,
                                  NULL,
                                  NULL,
                                  m_hasRedirectedIO,
                                  CREATE_NO_WINDOW,
                                  lstrlenA(env) > 0 ? (LPVOID)env : NULL,
                                  lstrlenA(cwd) > 0 ? cwd : NULL,
                                  &m_startupInfoA,
                                  &m_procInfo);
        return false;
    }

    bool Process::_CreateProcess(const wchar_t* file, wchar_t* cmd, const wchar_t* env, const wchar_t* cwd) {
        if(_BeforeCreateProcess(true))
            return CreateProcessW(lstrlenW(file) > 0 ? file : NULL,
                                 lstrlenW(cmd) > 0 ? cmd : NULL,
                                 NULL,
                                 NULL,
                                 m_hasRedirectedIO,
                                 CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT,
                                 lstrlenW(env) > 0 ? (LPVOID)env : NULL,
                                 lstrlenW(cwd) > 0 ? cwd : NULL,
                                 &m_startupInfo,
                                 &m_procInfo);
        return false;
    }

    void Process::CreateProcessSuccess() {
        if (!RegisterWaitCallback()) {
            PRINT_ERROR(L"RegisterWaitForSingleObject");
        }

        CloseHandle(m_procInfo.hThread);
        ProcessStarted();
    }

    std::vector<Process*> Process::_processes;

}   //namespace scriptpad

#endif  //_SCRIPT_PAD_PROCESS_CPP