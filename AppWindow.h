#pragma once

#ifndef _APP_WINDOW_H
    #define _APP_WINDOW_H

#define JSON_DISABLE_ENUM_SERIALIZATION 0
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace Microsoft::WRL;

namespace app {

    static TCHAR szWindowClass[] = _T("DesktopApp");
    static TCHAR szTitle[] = _T("Script pad desktop application");

    enum Command {
        NONE,
        INITIALIZE,
        CODESYNC,
        INVOKE 
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(Command, {
        { INITIALIZE, "initialize" },
        { CODESYNC, "codesync" },
        { INVOKE, "invoke" }
        });

    struct Message {
        Command cmd{ NONE };
        std::string code{ 0 };
        std::string message{ 0 };
        std::string error{ 0 };
        bool respond{ 0 };
    };

    class AppWindow {
    public:
        AppWindow(HINSTANCE, int);
        ~AppWindow();

        void AddWebMessageReceivedHandler(HRESULT (*webMessage)(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args));
        bool Show();
    private:
        HINSTANCE m_hInst{ 0 };
        WNDCLASSEX m_wcEx{ 0 };
        HWND m_mainWindow{ 0 };
        int m_nShow{ 0 };

        HRESULT(*WebMessage)(ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs*);
        bool m_customWebMessageHandler{ 0 };

        wil::com_ptr<ICoreWebView2Environment> m_webViewEnv;
        wil::com_ptr<ICoreWebView2Controller> m_webViewCtrl;
        wil::com_ptr<ICoreWebView2> m_webView;

        bool InitializeWindow();
        void InitializeWebview();
        void RegisterEventHandlers();
        HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
        HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
        Message ParseMessage(json*);
        std::wstring GetResponse(Message);

        bool HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result);
        static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    };

    void to_json(json&, const Message&);
    void from_json(const json&, Message&);
} //namespace app

#endif