#ifndef _PROCESS_CPP
    #define _PROCESS_CPP

#include "Process.h"

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

        init_members();

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
            0,
            NULL,
            wdLen > 0 ? wd : NULL,
            &startupInfo,
            &procInfo);

        DWORD eCode;
        if (!GetExitCodeProcess(procInfo.hProcess, &eCode))
            error::PrintError(L"GetExitCodeProcess");


        if (!bSuccess) {
            error::PrintError(_T("Error creating process"));
            return false;
        }
        else {
            CloseHandle(procInfo.hProcess);
            CloseHandle(procInfo.hThread);

            pipe.process_started();
            return true;
        }
    }


    void Process::Write(std::string data) {
        pipe.write(data);
    }


    void Process::Read() {
        pipe.read();
    }


    void Process::Read(std::wstring* ret) {
        pipe.read();
    }


    void Process::OnOutputReceived(const wchar_t* data) {
        wprintf_s(data);
    }


    Process* Process::Run(const ProcessStartInfo& startInfo) {
        Process* proc = new Process(startInfo);
        _processes.push_back(proc);
        proc->Start();
        if (startInfo.RedirectStdOutput)
            proc->Read();

        return proc;
    }


    void Process::init_members() {
        ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&startupInfo, sizeof(STARTUPINFO));

        startupInfo.cb = sizeof(STARTUPINFO);

        if (pipe.redirect_io(StartInfo.RedirectStdInput, StartInfo.RedirectStdOutput, startupInfo)) {
            hasRedirectedIO = true;
            //pipe.start_read();
        }
    }

    std::vector<Process*> Process::_processes;
}

#endif