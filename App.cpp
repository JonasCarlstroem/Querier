#ifndef _APP_MAIN
    #define _APP_MAIN

#include "AppWindow.h"
#include "NodeJS.h"
#include <locale>
#include <codecvt>

HRESULT WebMessageReceived(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args);
app::Message ParseMessage(json*);
std::wstring GetResponse(app::Message);
static nodejs::NodeJS node;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
) {
    app::AppWindow App{ hInstance, nCmdShow };

    App.AddWebMessageReceivedHandler(WebMessageReceived);

    if(!App.Show())
        return 0;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

HRESULT WebMessageReceived(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args) {
    wil::unique_cotaskmem_string message;
    args->TryGetWebMessageAsString(&message);

    json data = json::parse((std::wstring)message.get());

    app::Message cmd = ParseMessage(&data);

    if (cmd.respond) {
        std::wstring response = GetResponse(cmd);
        webView->PostWebMessageAsJson(response.c_str());
    }

    return S_OK;
}

app::Message ParseMessage(json* data) {
    app::Message msg = data->template get<app::Message>();
    switch (msg.cmd) {
        case app::Command::INITIALIZE:
            break;
        case app::Command::INVOKE:
            std::string code_ = msg.value;
            std::wstring code = L"";
            //int result = MultiByteToWideChar(CP_UTF8, 0, code_.c_str(), (int)code_.length(), code.c_str(), 0);
            //std::string code = msg.value;

            node.Invoke(code);

            msg.respond = true;
            break;
    }
    return msg;
}

std::wstring GetResponse(app::Message msg) {
    return std::wstring();
}

#endif