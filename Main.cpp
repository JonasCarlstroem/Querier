#ifndef _SCRIPT_PAD_APP_MAIN
    #define _SCRIPT_PAD_APP_MAIN

#include "Application.h"
#include "Node.h"

//####Todo####
//###General###
// module handler
// support for multiple queries (mod handler)
// integrate http library
//
//###Node###
// make module class out of current state of this class 
// implement npm with gui
// implement version checking (including check to see if nvm is installed)
// 
//###CSharp###
// imolement as module

using json = nlohmann::json;
using namespace scriptpad;

static Application App;
//static Nodejs nodejs(L"C:\\Program Files\\nodejs\\node.exe");

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
) {


    AppWindow MainWindow{ hInstance, nCmdShow };

    App.Initialize(&MainWindow);
    //nodejs.Initialize(&MainWindow);
    //node.Run()

    if(!MainWindow.Show())
        return 0;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

#endif  //_SCRIPT_PAD_APP_MAIN