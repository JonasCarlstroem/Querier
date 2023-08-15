#pragma once

#ifndef _APP_WINDOW_H
#define _APP_WINDOW_H

#define WM_WEBVIEW (WM_USER + 100)

#include "Util.h"
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"
#include <nlohmann/json.hpp>

using namespace Microsoft::WRL;

namespace app {
    using json = nlohmann::json;

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

    NLOHMANN_JSON_SERIALIZE_ENUM(AppCommand, {
        { INITIALIZE, "initialize" },
        { CONFIG, "config" },
        { CODESYNC, "codesync" },
        { INVOKE, "invoke" },
        { RESULT, "result" }
    });

    struct Message {
        AppCommand cmd{ NONE };
        std::string message{ 0 };
        std::string error{ 0 };
        bool respond{ 0 };
    };

    void to_json(json&, const Message&);
    void from_json(const json&, Message&);

    class AppWindow {
    public:
        AppWindow(HINSTANCE, int);
        ~AppWindow();

        void AddWebMessageReceivedHandler(HRESULT(*webMessage)(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args));
        void AddOnWebViewCreatedHandler(void (*OnWebViewCreatedHandler)(ICoreWebView2* webview));
        bool Show();
        ICoreWebView2* get_WebView() {
            return m_webView.get();
        }
        HWND get_MainWindow() {
            return m_mainWindow;
        }

        static std::wstring GetResponse(app::Message msg) {
            json cont = json(msg);
            std::string src = cont.dump();
            std::wstring ret;
            if (util::string_to_wstring(src, &ret))
                return ret;

            return std::wstring();
        }
    private:
        HINSTANCE m_hInst{ 0 };
        WNDCLASSEX m_wcEx{ 0 };
        HWND m_mainWindow{ 0 };
        int m_nShow{ 0 };

        HRESULT(*WebMessage)(ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs*);
        void (*OnWebViewCreated)(ICoreWebView2* webview);
        bool m_customWebMessageHandler{ 0 };

        wil::com_ptr<ICoreWebView2Environment> m_webViewEnv;
        wil::com_ptr<ICoreWebView2Controller> m_webViewCtrl;
        wil::com_ptr<ICoreWebView2> m_webView;

        bool InitializeWindow();
        void InitializeWebview();
        void RegisterEventHandlers();
        HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
        HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
        //Message ParseMessage(json*);
        //std::wstring GetResponse(Message);

        bool HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result);
        static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    };

    
} //namespace app

#endif