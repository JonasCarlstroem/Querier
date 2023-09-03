#ifndef _SCRIPT_PAD_APP_MAIN
    #define _SCRIPT_PAD_APP_MAIN

#include "Application.h"

//--->Todo<---
//-->General<--
// - integrate http library (maybe)
// - integrat zip library (when saving queries, 
// zip files together, use custom file
// extension which will be registered with
// the operating system when instslled
// - implement shared configuration class
// for language modules with key value 
// semantics which can be loaded and set
// in the gui
// - implement query configuration which
// can be set inte the gui
//
//-->Node<--
// - implement npm with gui
// 
//-->C#<--
// - implement as module
//
//-->C++<--
// - implement as module

using namespace querier;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
) {
    Application App(hInstance, nCmdShow);

    return App.Start();
}

#endif  //_SCRIPT_PAD_APP_MAIN