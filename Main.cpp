#ifndef _SCRIPT_PAD_APP_MAIN
    #define _SCRIPT_PAD_APP_MAIN

#include "Application.h"
#include "Node.h"

//####Todo####
//###General###
// support for multiple queries (mod handler)
// integrate http library
//
//###Node###
// implement npm with gui
// implement version checking (including check to see if nvm is installed)
// 
//###CSharp###
// imolement as module

using namespace scriptpad;

//static Nodejs nodejs(L"C:\\Program Files\\nodejs\\node.exe");

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