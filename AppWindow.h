#pragma once

#ifndef _SCRIPT_PAD_APP_WINDOW_H
#define _SCRIPT_PAD_APP_WINDOW_H

#define WM_WEBVIEW (WM_USER + 100)

#include "Util.h"
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"
#include "resource.h"

using namespace Microsoft::WRL;

static constexpr size_t s_maxLoadString = 100;

namespace querier {
    static TCHAR szWindowClass[] = _T("DesktopApp");
    static TCHAR szTitle[] = _T("Script pad desktop application");

    enum AppCommand {
        NONE,
        INITIALIZE,
        CONFIG,
        CODESYNC,
        INVOKE,
        RESULT
    };

    enum ResultType {
        NORESULT,
        ISSUCCESS,
        ISERROR
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(AppCommand, {
        { INITIALIZE, "initialize" },
        { CONFIG, "config" },
        { CODESYNC, "codesync" },
        { INVOKE, "invoke" },
        { RESULT, "result" }
    });

    NLOHMANN_JSON_SERIALIZE_ENUM(ResultType, {
        { NORESULT, "noresult" },
        { ISSUCCESS, "success" },
        { ISERROR, "error" }
    });

    struct Message {
        AppCommand cmd{ NONE };
        ResultType resultType{ NORESULT };
        std::string message{ 0 };
        std::string error{ 0 };
        bool respond{ 0 };
        std::string response{0};
    };

    void to_json(json& j, const Message& m);
    void from_json(const json& j, Message& m);

    static HINSTANCE g_appInstance;

    class AppWindow {
    public:
        AppWindow(HINSTANCE hInstance, int nCmdShow);
        ~AppWindow();

        void AddWebMessageReceivedHandler(HRESULT(*webMessage)(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args));
        void AddWebMessageHandler(std::function<std::wstring(Message*)> function);
        void AddOnWebViewCreatedHandler(void (*OnWebViewCreatedHandler)(ICoreWebView2* webview));
        bool Show();

        ICoreWebView2* get_WebView() {
            return m_webView.get();
        };

        HWND get_MainWindow() {
            return m_mainWindow;
        };

        static std::wstring GetResponse(Message msg);


    private:
        HINSTANCE m_hInst{ 0 };
        HWND m_mainWindow{ 0 };
        int m_nShow{ 0 };

        HRESULT(*WebMessageReceivedHandler)(ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs*);
        std::function<HRESULT(ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs*)>   OnWebMessageReceived;
        std::function<std::wstring(Message*)>                                               HandleWebMessage;

        void (*OnWebViewCreated)(ICoreWebView2* webview);
        bool m_customWebMessageHandler{ 0 };

        wil::com_ptr<ICoreWebView2Environment> m_webViewEnv;
        wil::com_ptr<ICoreWebView2Controller> m_webViewCtrl;
        wil::com_ptr<ICoreWebView2> m_webView;

        bool InitializeWindow();
        PCWSTR GetWindowClass();
        void InitializeWebview();
        void RegisterEventHandlers();
        HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
        HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
        bool HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result);
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        HRESULT WebMessageReceived(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args);
    };


} //namespace scriptpad

#endif  //_SCRIPT_PAD_APP_WINDOW_H