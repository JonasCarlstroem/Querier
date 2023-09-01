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
    /*class Message;*/
    static TCHAR szWindowClass[] = _T("DesktopApp");
    static TCHAR szTitle[] = _T("Script pad desktop application");

    static HINSTANCE g_appInstance;

    class AppWindow {
    public:
        AppWindow(HINSTANCE hInstance, int nCmdShow);
        ~AppWindow();

        void AddWebMessageReceivedHandler(HRESULT(*webMessage)(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args));
        void AddWebMessageHandler(std::function<std::wstring(json)> function);
        void AddOnWebViewCreatedHandler(void (*OnWebViewCreatedHandler)(ICoreWebView2* webview));
        bool Show();

        ICoreWebView2* get_WebView() {
            return m_webView.get();
        };

        HWND get_MainWindow() {
            return m_mainWindow;
        };

        /*static std::wstring GetResponse(Message msg);*/


    private:
        HINSTANCE m_hInst{ 0 };
        HWND m_mainWindow{ 0 };
        int m_nShow{ 0 };

        HRESULT(*WebMessageReceivedHandler)(ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs*);
        std::function<HRESULT(ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs*)>   OnWebMessageReceived;
        std::function<std::wstring(json)>                                                   HandleWebMessage;

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