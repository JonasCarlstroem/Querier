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

static constexpr size_t s_maxLoadString = 100;

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

    void to_json(json& j, const Message& m) {
        j = json{
            { "cmd", m.cmd },
            { "resultType", m.resultType },
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

    HINSTANCE g_appInstance;

    class AppWindow {
    public:
        AppWindow(HINSTANCE hInstance, int nCmdShow) : m_hInst(hInstance), m_nShow(nCmdShow) {
            g_appInstance = hInstance;
            InitializeWindow();
        };

        ~AppWindow() {
            m_webViewEnv->Release();
            m_webViewCtrl->Close();
            m_webViewCtrl = nullptr;
            m_webView = nullptr;

            m_webViewEnv = nullptr;
        };

        void AddWebMessageReceivedHandler(HRESULT(*webMessage)(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args)) {
            OnWebMessageReceived = webMessage;
            m_customWebMessageHandler = true;
        };

        void AddWebMessageHandler(std::function<std::wstring(Message*)> function) {
            HandleWebMessage = function;
        };

        void AddOnWebViewCreatedHandler(void (*OnWebViewCreatedHandler)(ICoreWebView2* webview)) {
            OnWebViewCreated = OnWebViewCreatedHandler;
        };

        bool Show() {
            ShowWindow(m_mainWindow,
                       m_nShow);
            UpdateWindow(m_mainWindow);

            EnableWindow(m_mainWindow, TRUE);

            InitializeWebview();
            return true;
        };

        ICoreWebView2* get_WebView() {
            return m_webView.get();
        };

        HWND get_MainWindow() {
            return m_mainWindow;
        };

        static std::wstring GetResponse(app::Message msg) {
            json cont = json(msg);
            std::string src = cont.dump();
            std::wstring ret;
            if (util::string_to_wstring(src, &ret))
                return ret;

            return std::wstring();
        };


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

        bool InitializeWindow() {
            m_mainWindow = CreateWindowEx(
                WS_EX_OVERLAPPEDWINDOW,
                GetWindowClass(),
                szTitle,
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT,
                1200, 900,
                NULL,
                NULL,
                g_appInstance,
                NULL
            );

            if (!m_mainWindow) {
                MessageBox(NULL,
                           _T("Call to CreateWindow failed!"),
                           _T("ScriptPad Desktop Application"),
                           NULL);

                throw std::exception("Error when creating window");
            }

            SetWindowLongPtr(m_mainWindow, GWLP_USERDATA, (LONG_PTR)this);

            return true;
        };

        PCWSTR GetWindowClass() {
            static PCWSTR winClass = [] {
                WNDCLASSEXW wcex;
                wcex.cbSize = sizeof(WNDCLASSEX);

                wcex.style = CS_HREDRAW | CS_VREDRAW;
                wcex.lpfnWndProc = WndProc;
                wcex.cbClsExtra = 0;
                wcex.cbWndExtra = 0;
                wcex.hInstance = g_appInstance;
                wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
                wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
                wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
                wcex.lpszMenuName = NULL;
                wcex.lpszClassName = szWindowClass;
                wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

                RegisterClassExW(&wcex);
                return szWindowClass;
            }();
            return winClass;
        }

        void InitializeWebview() {
            HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
                                                                  Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                                                                      this,
                                                                      &AppWindow::OnCreateEnvironmentCompleted).Get());

            if (!SUCCEEDED(hr)) {
                if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
                    MessageBox(
                        m_mainWindow,
                        L"Couldn't find Edge intallation. "
                        "Do you have a version installed that's compatible with this "
                        "WebView2 SDK version?",
                        nullptr, MB_OK);
                }
            }
        };

        void RegisterEventHandlers() {
            EventRegistrationToken token;
            
            if (m_customWebMessageHandler) {
                m_webView->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(WebMessageReceivedHandler).Get(), &token);
            }
            else {
                m_webView->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(this, 
                                                                                                        &AppWindow::WebMessageReceived).Get(), &token);
            }
        };

        HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment) {
            m_webViewEnv = environment;

            m_webViewEnv->CreateCoreWebView2Controller(
                m_mainWindow, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    this, &AppWindow::OnCreateCoreWebView2ControllerCompleted)
                .Get());

            return S_OK;
        };

        HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller) {
            if (result == S_OK) {
                m_webViewCtrl = controller;

                m_webViewCtrl->get_CoreWebView2(&m_webView);

                wil::com_ptr<ICoreWebView2Settings> settings;
                m_webView->get_Settings(&settings);
                
                auto settings3 = settings.try_query<ICoreWebView2Settings3>();
                
                settings->put_IsStatusBarEnabled(FALSE);
                settings->put_IsScriptEnabled(TRUE);
                settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                settings->put_IsWebMessageEnabled(TRUE);

                settings3->put_AreBrowserAcceleratorKeysEnabled(FALSE);

#ifndef _DEBUG
                settings->put_AreDefaultContextMenusEnabled(FALSE);
                settings->put_AreDevToolsEnabled(FALSE);
#endif

                RECT bounds;
                GetClientRect(m_mainWindow, &bounds);
                m_webViewCtrl->put_Bounds(bounds);

                // Schedule an async task to navigate to Bing
                m_webView->Navigate(L"http://localhost:5173/");

                RegisterEventHandlers();
                if (OnWebViewCreated)
                    OnWebViewCreated(m_webView.get());
            }
            return S_OK;
        };

        bool HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result) {

            //std::wstring response;
            switch (message) {
                case WM_PAINT:
                {
                    PAINTSTRUCT ps;
                    HDC hdc;
                    hdc = BeginPaint(hWnd, &ps);

                    return EndPaint(hWnd, &ps);
                }
                case WM_DESTROY:
                {
                    PostQuitMessage(0);
                    break;
                }
                case WM_SIZE:
                case WM_SIZING:
                {
                    if (m_webViewCtrl != nullptr) {
                        RECT bounds;
                        GetClientRect(hWnd, &bounds);
                        m_webViewCtrl->put_Bounds(bounds);

                        return true;
                    }
                    return false;
                }
                case WM_WEBVIEW:
                {
                    Message* data = (Message*)wParam;
                    std::wstring response = GetResponse(*data);
                    m_webView->PostWebMessageAsJson(response.c_str());
                    return true;
                }
                case WM_KEYDOWN:
                {
                    switch (wParam) {
                        case VK_F5:
                        {
                            std::string* key = (std::string*)wParam;
                            util::print_message(util::string_to_wstring(*key));
                        }
                    }
                }
                case WM_KEYUP:
                {
                    switch (wParam) {
                        case VK_F5:
                        {
                            std::string* key = (std::string*)wParam;
                            util::print_message(util::string_to_wstring(*key));
                        }
                    }
                    return true;
                }
                case WM_CHAR:
                {
                    switch (wParam) {
                        case VK_F5:
                        {
                            std::string* key = (std::string*)wParam;
                            util::print_message(util::string_to_wstring(*key));
                        }
                    }
                }
                //case WM_KEYUP:
                    //break;

            }
            return false;
        };

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
            if (auto app = (AppWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA)) {
                LRESULT result = 0;
                if (app->HandleWindowMessage(hWnd, message, wParam, lParam, &result)) {
                    return result;
                }
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        };

        HRESULT WebMessageReceived(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args) {
            wil::unique_cotaskmem_string message;
            args->TryGetWebMessageAsString(&message);

            json data = json::parse((std::wstring)message.get());
            app::Message cmd = data.template get<app::Message>();
            std::wstring response = HandleWebMessage(&cmd);

            if (cmd.respond) {
                webView->PostWebMessageAsJson(response.c_str());
            }

            return S_OK;
        };
    };


} //namespace app

#endif