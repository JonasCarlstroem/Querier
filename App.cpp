#ifndef _APP_MAIN
    #define _APP_MAIN

#include "AppWindow.h"
#include "NodeJS.h"
#include <locale>
#include <codecvt>
#include "Util.h"

enum Command;
struct Message;

HRESULT WebMessageReceived(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args);
void OnOutputReceived(std::string, std::thread*);
void OnWebViewCreated(ICoreWebView2* webview);

Message ParseMessage(json*);
std::wstring GetResponse(Message);

static nodejs::NodeJS node(L"C:\\Program Files\\nodejs\\node.exe");
static ICoreWebView2* webview;

void to_json(json&, const Message&);
void from_json(const json&, Message&);

enum Command {
    NONE,
    INITIALIZE,
    CONFIG,
    CODESYNC,
    INVOKE
};

NLOHMANN_JSON_SERIALIZE_ENUM(Command, {
    { INITIALIZE, "initialize" },
    { CONFIG, "config" },
    { CODESYNC, "codesync" },
    { INVOKE, "invoke" },
});

struct Message {
    Command cmd{ NONE };
    std::string message{ 0 };
    std::string error{ 0 };
    bool respond{ 0 };
};

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
) {
    app::AppWindow App{ hInstance, nCmdShow };

    App.AddWebMessageReceivedHandler(WebMessageReceived);
    App.AddOnWebViewCreatedHandler(OnWebViewCreated);
    node.SetOutputReceivedEvent(OnOutputReceived);
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

    Message msg = ParseMessage(&data);

    if (msg.respond) {
        std::wstring response = GetResponse(msg);
        webView->PostWebMessageAsJson(response.c_str());
    }

    return S_OK;
}

void OnOutputReceived(std::string ret, std::thread* th) {
    th->join();
    Message msg = Message{ INVOKE, ret };
    std::wstring response = GetResponse(msg);
    util::print_message(response);
    webview->PostWebMessageAsJson(response.c_str());
    util::print_message(util::string_to_wstring(ret));
    th->detach();
}

void OnWebViewCreated(ICoreWebView2* wv) {
    webview = wv;
}

Message ParseMessage(json* data) {
    Message msg = data->template get<Message>();
    std::wstring wret;
    std::string ret;
    switch (msg.cmd) {
        case Command::INITIALIZE:
            msg.respond = true;
            node.GetInitialFileContent(&msg.message);
            break;
        case Command::CONFIG:
            break;
        case Command::CODESYNC:
            node.SyncFileContent(msg.message);
            break;
        case Command::INVOKE:
            //msg.respond = true;
            node.Invoke(&msg.message);
            break;
    }
    return msg;
}

std::wstring GetResponse(Message msg) {
    json cont = json(msg);
    std::string src = cont.dump();
    std::wstring ret;
    if (util::string_to_wstring(src, &ret))
        return ret;

    return std::wstring();
}

void to_json(json& j, const Message& m) {
    j = json{
            { "cmd", m.cmd },
            { "message", m.message },
            { "error", m.error }
    };
}

void from_json(const json& j, Message& m) {
    j.at("cmd").get_to(m.cmd);
    if (m.cmd != INITIALIZE) {
        j.at("message").get_to(m.message);
    }
}

#endif