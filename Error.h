#pragma once

#ifndef _ERROR_H
    #define _ERROR_H

//#define PRINT_ERROR(errorStr) if _DEBUG error::PrintError(#errorStr);
#ifdef _DEBUG
#define PRINT_ERROR_IF_DEBUG(str)                   scriptpad::PrintError(str);
#define PRINT_ERROR_EXIT_IF_DEBUG(hProc, sMsg)     scriptpad::PrintExitCode(hProc, sMsg);
#endif
#define PRINT_ERROR(str)                            PRINT_ERROR_IF_DEBUG(str)
#define PRINT_ERROR_EXIT(hProc, sMsg)              PRINT_ERROR_EXIT_IF_DEBUG(hProc, sMsg)

#include <Windows.h>
#include <string>
#include <strsafe.h>

namespace scriptpad {
    void PrintError(LPCTSTR lpszFunction);
    void PrintExitCode(HANDLE hProcess, LPCTSTR lpszStr);
}   //namespace scriptpad

#endif  //_SCRIPT_PAD_ERROR_H