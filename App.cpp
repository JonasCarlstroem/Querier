#ifndef _APP_MAIN
    #define _APP_MAIN

#include "AppWindow.h"
#include "NodeJS.h"
#include <locale>
#include <codecvt>
#include "Util.h"

HRESULT WebMessageReceived(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args);
app::Message ParseMessage(json*);
std::wstring GetResponse(app::Message);
static nodejs::NodeJS node(L"C:\\Program Files\\nodejs\\node.exe");

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

    app::Message msg = ParseMessage(&data);

    if (msg.respond) {
        std::wstring response = GetResponse(msg);
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
            msg.respond = true;
            std::wstring code;
            if (util::string_to_wstring(msg.code, &code)) {
                std::wstring ret;
                node.Invoke(code, &ret);
                if (!util::wstring_to_string(ret, &msg.message)) {
                    msg.message = "There was an error";
                }
            }
            break;
    }
    return msg;
}

std::wstring GetResponse(app::Message msg) {
    json cont = json(msg);
    std::string src = cont.dump();
    std::wstring ret;
    if (util::string_to_wstring(src, &ret))
        return ret;

    return std::wstring();
}

#endif