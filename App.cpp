#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"
#include "Process.h"
#include "Util.h"
#include <nlohmann/json.hpp>


using json = nlohmann::json;
using namespace Microsoft::WRL;

namespace ns {
    struct Message {
        enum Command { INVOKE };
        Command _cmd;
        std::string cmd;
        std::string value;
        bool respond{ 0 };
    };

    void to_json(json& j, const Message& c) {
        j = json{ {"cmd", c.cmd}, {"value", c.value}, {"cmd", c._cmd}};
    }

    void from_json(const json& j, Message& c) {
        j.at("cmd").get_to(c.cmd);
        j.at("value").get_to(c.value);
    }

}

static TCHAR szWindowClass[] = _T("DesktopApp");

static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ns::Message ParseMessage(json);
std::wstring GetResponse(std::wstring);

static wil::com_ptr<ICoreWebView2Controller> webviewController;

static wil::com_ptr<ICoreWebView2> webview;

class Command {
    std::string cmd;
    std::string value;
};

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
) {
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL,
                   _T("Call to RegisterClassEx failed!"),
                   _T("Windows Desktop Guided Tour"),
                   NULL);

        return 1;
    }

    hInst = hInstance;

    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1200, 900,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd) {
        MessageBox(NULL,
                   _T("Call to CreateWindow failed!"),
                   _T("Windows Desktop Guided Tour"),
                   NULL);

        return 1;
    }

    ShowWindow(hWnd,
               nCmdShow);
    UpdateWindow(hWnd);

    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
                                             Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                                             [hWnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

                                                 // Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
                                                 env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                                     [hWnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                                                         if (controller != nullptr) {
                                                             webviewController = controller;
                                                             webviewController->get_CoreWebView2(&webview);
                                                         }

                                                         // Add a few settings for the webview
                                                         // The demo step is redundant since the values are the default settings
                                                         wil::com_ptr<ICoreWebView2Settings> settings;
                                                         webview->get_Settings(&settings);
                                                         settings->put_IsScriptEnabled(TRUE);
                                                         settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                                                         settings->put_IsWebMessageEnabled(TRUE);

                                                         // Resize WebView to fit the bounds of the parent window
                                                         RECT bounds;
                                                         GetClientRect(hWnd, &bounds);
                                                         webviewController->put_Bounds(bounds);

                                                         // Schedule an async task to navigate to Bing
                                                         webview->Navigate(L"http://localhost:5173/");

                                                         // Step 4 - Navigation events

                                                         // Step 5 - Scripting

                                                         // Step 6 - Communication between host and web content
                                                         EventRegistrationToken token;
                                                         webview->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                                             [](ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                                                 wil::unique_cotaskmem_string message;
                                                                 args->TryGetWebMessageAsString(&message);
                                                                 std::wstring data = message.get();
                                                                 ns::Message cmd = ParseMessage(json::parse(data));

                                                                 if (cmd.respond) {
                                                                    std::wstring response = GetResponse(message.get());
                                                                    webview->PostWebMessageAsJson(response.c_str());
                                                                 }
                                                                 //std::wstring resp = L"{ \"command\": \"Hello!\" }";
                                                                 //json data = json::parse(resp);
                                                                 //webview->PostWebMessageAsString(message.get());
                                                                 return S_OK;
                                                             }
                                                         ).Get(), &token);

                                                         return S_OK;
                                                     }).Get());
                                                 return S_OK;
                                             }).Get());


    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, Windows desktop!");

    switch (message) {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            /*TextOut(hdc,
                    5, 5,
                    greeting, _tcslen(greeting));*/

            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
        case WM_SIZING:
            if (webviewController != nullptr) {
                RECT bounds;
                GetClientRect(hWnd, &bounds);
                webviewController->put_Bounds(bounds);
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;
    }

    return 0;
}

ns::Message ParseMessage(json data) {
    ns::Message cmd = data.template get<ns::Message>();
    return data.template get<ns::Message>();
}


std::wstring GetResponse(ns::Message cmd) {
    return std::wstring(L"Hello");
}