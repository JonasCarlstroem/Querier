#include "AppWindow.h"

HRESULT WebMessageReceived(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args);
app::Message ParseMessage(json*);
std::wstring GetResponse(app::Message);

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

    app::Message cmd = ParseMessage(&json::parse((std::wstring)message.get()));

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
            std::string code = msg.value;
            msg.respond = true;
            break;
    }
    return msg;
}

std::wstring GetResponse(app::Message msg) {
    return std::wstring();
}