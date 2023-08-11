#pragma once

#ifndef _ERROR_H
    #define _ERROR_H

#include <Windows.h>
#include <strsafe.h>

namespace error {
    void PrintError(LPCTSTR lpszFunction);
}

#endif