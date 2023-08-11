#include "Process.h"

Process::Process() : secAttr(), procInfo(), pipe(secAttr, procInfo), startupInfo(), StartInfo() {
    secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    secAttr.lpSecurityDescriptor = NULL;
    secAttr.bInheritHandle = FALSE;
};


Process::Process(std::wstring Name) : Process() {
    StartInfo.FileName = Name;
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
    if (&StartInfo.FileName == nullptr)
        return false;

    init_members();

    const wchar_t* file = StartInfo.FileName.c_str();
    wchar_t* cmd = StartInfo.CommandLine.data();
    const wchar_t* wd = StartInfo.WorkingDirectory.c_str();

    size_t fileLen = StartInfo.FileName.length();
    size_t cmdLen = StartInfo.CommandLine.length();
    size_t wdLen = StartInfo.WorkingDirectory.length();

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
        PrintError(L"GetExitCodeProcess");


    if (!bSuccess) {
        PrintError(_T("Error creating process"));
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