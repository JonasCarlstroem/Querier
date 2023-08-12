#ifndef _PROCESS_CPP
    #define _PROCESS_CPP

#include "Process.h"
#include "Error.h"

namespace process {
    Process::Process() : secAttr(), procInfo(), pipe(secAttr, procInfo), startupInfo(), StartInfo() {
        secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        secAttr.lpSecurityDescriptor = NULL;
        secAttr.bInheritHandle = FALSE;
    };


    Process::Process(std::wstring Name) : Process() {
        StartInfo.wFileName = Name;
    }


    Process::Process(ProcessStartInfo startInfo) : secAttr(), procInfo(), pipe(secAttr, procInfo), startupInfo(), StartInfo(startInfo) {
        secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        secAttr.lpSecurityDescriptor = NULL;
        secAttr.bInheritHandle = FALSE;
    }


    Process::~Process() {
        for (auto proc : _processes) {
            delete proc;
        }
    };


    bool Process::Start() {
        if (&StartInfo.wFileName == nullptr)
            return false;

        InitMembers();

        const wchar_t* file = StartInfo.wFileName.c_str();
        wchar_t* cmd = StartInfo.wCommandLine.data();
        const wchar_t* wd = StartInfo.wWorkingDirectory.c_str();

        size_t fileLen = StartInfo.wFileName.length();
        size_t cmdLen = StartInfo.wCommandLine.length();
        size_t wdLen = StartInfo.wWorkingDirectory.length();
        
        bSuccess = CreateProcess(fileLen > 0 ? file : NULL,
            cmdLen > 0 ? cmd : NULL,
            NULL,
            NULL,
            hasRedirectedIO,
            CREATE_NO_WINDOW,
            NULL,
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
            CloseHandle(procInfo.hProcess);
            CloseHandle(procInfo.hThread);

            pipe.ProcessStarted();
            return true;
        }
    }


    void Process::Write(std::string data) {
        pipe.Write(data);
    }


    bool Process::wRead(std::wstring* ret) {
        return pipe.wRead(ret);
    }


    bool Process::Read(std::string* ret) {
        return pipe.Read(ret);
    }


    Process* Process::Run(const ProcessStartInfo& startInfo) {
        Process* proc = new Process(startInfo);
        _processes.push_back(proc);
        proc->Start();
        return proc;
    }


    void Process::InitMembers() {
        ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&startupInfo, sizeof(STARTUPINFO));

        startupInfo.cb = sizeof(STARTUPINFO);

        if (pipe.RedirectIO(StartInfo.RedirectStdInput, StartInfo.RedirectStdOutput, startupInfo)) {
            hasRedirectedIO = true;
        }
    }

    std::vector<Process*> Process::_processes;
}

#endif