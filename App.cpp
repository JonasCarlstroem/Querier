#ifndef _APP_MAIN
    #define _APP_MAIN

#include "App.h"

using json = nlohmann::json;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
) {
    app::AppWindow App{ hInstance, nCmdShow };

    node.Initialize(&App);
    //App.AddWebMessageReceivedHandler(WebMessageReceived);

    if(!App.Show())
        return 0;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//HRESULT WebMessageReceived(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args) {
//    wil::unique_cotaskmem_string message;
//    args->TryGetWebMessageAsString(&message);
//
//    json data = json::parse((std::wstring)message.get());
//
//    app::Message msg = HandleWebMessage(&data);
//
//    if (msg.respond) {
//        std::wstring response = app::AppWindow::GetResponse(msg);
//        webView->PostWebMessageAsJson(response.c_str());
//    }
//
//    return S_OK;
//}

std::wstring HandleWebMessage(app::Message* msg) {
    std::wstring wret;
    std::string ret;
    switch (msg->cmd) {
        case app::AppCommand::INITIALIZE:
            msg->respond = true;
            node.GetInitialFileContent(&msg->message);
            break;
        case app::AppCommand::CONFIG:
            break;
        case app::AppCommand::CODESYNC:
            node.SyncFileContent(msg->message);
            break;
        case app::AppCommand::INVOKE:
            node.Invoke();
            break;
    }
    return json(*msg);
}

#endif