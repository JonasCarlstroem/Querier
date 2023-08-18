#ifndef _APP_MAIN
    #define _APP_MAIN

#include "App.h"
#include "AppWindow.h"

//####Todo####
//###General###
// base module class (abstract)
// module handler
// support for multiple queries (mod handler)
// integrate http library
//
//###Node###
// make module class out of current state of this class then make this class a module
// implement npm with gui
// implement version checking (including check to see if nvm is installed)
// 
//###CSharp###
// imolement as module
// split module into type of either interpreter or compiler

using json = nlohmann::json;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
) {


    app::AppWindow App{ hInstance, nCmdShow };

    node.Initialize(&App);
    //node.Run()

    if(!App.Show())
        return 0;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

#endif