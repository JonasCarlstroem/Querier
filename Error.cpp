#ifndef _SCRIPT_PAD_ERROR_CPP
#define _SCRIPT_PAD_ERROR_CPP
#include "Error.h"

namespace querier {
    void PrintError(LPCTSTR lpszFunction) {
        LPVOID lpMsgBuf;
        LPVOID lpDisplayBuf;
        DWORD dw = GetLastError();

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);

        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
            (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
        StringCchPrintf((LPTSTR)lpDisplayBuf,
            LocalSize(lpDisplayBuf) / sizeof(TCHAR),
            TEXT("%s failed with error %d: %s"),
            lpszFunction, dw, lpMsgBuf);
        MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
        //wprintf_s((LPCTSTR)lpDisplayBuf);

        LocalFree(lpMsgBuf);
        LocalFree(lpDisplayBuf);
    }

    void PrintExitCode(HANDLE hProcess, LPCTSTR sMsg) {
        DWORD dCode;
        if (!GetExitCodeProcess(hProcess, &dCode))
            PrintError(L"GetExitCodeProcess");
        else
            PrintError(sMsg);
    }
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_ERROR_CPP