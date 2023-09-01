#ifndef _SCRIPT_PAD_APP_WINDOW_CPP
    #define _SCRIPT_PAD_APP_WINDOW_CPP

#include "AppWindow.h"

namespace querier {

    AppWindow::AppWindow(HINSTANCE hInstance, int nCmdShow) : m_hInst(hInstance), m_nShow(nCmdShow) {
        g_appInstance = hInstance;
        InitializeWindow();
    };

    AppWindow::~AppWindow() {
        m_webViewEnv->Release();
        m_webViewCtrl->Close();
        m_webViewCtrl = nullptr;
        m_webView = nullptr;

        m_webViewEnv = nullptr;
    };

    void AppWindow::AddWebMessageReceivedHandler(HRESULT(*webMessage)(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args)) {
        OnWebMessageReceived = webMessage;
        m_customWebMessageHandler = true;
    };

    void AppWindow::AddWebMessageHandler(std::function<std::wstring(json)> function) {
        HandleWebMessage = function;
    };

    void AppWindow::AddOnWebViewCreatedHandler(void (*OnWebViewCreatedHandler)(ICoreWebView2* webview)) {
        OnWebViewCreated = OnWebViewCreatedHandler;
    };

    bool AppWindow::Show() {
        ShowWindow(m_mainWindow,
                   m_nShow);
        UpdateWindow(m_mainWindow);

        EnableWindow(m_mainWindow, TRUE);

        InitializeWebview();
        return true;
    };

    //std::wstring AppWindow::GetResponse(Message msg) {
    //    json cont = json(msg);
    //    std::string src = cont.dump();
    //    std::wstring ret;
    //    if (str_to_wstr(src, &ret))
    //        return ret;

    //    return std::wstring();
    //};

    bool AppWindow::InitializeWindow() {
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

    PCWSTR AppWindow::GetWindowClass() {
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

    void AppWindow::InitializeWebview() {
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

    void AppWindow::RegisterEventHandlers() {
        EventRegistrationToken token;

        if (m_customWebMessageHandler) {
            m_webView->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(WebMessageReceivedHandler).Get(), &token);
        }
        else {
            m_webView->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(this,
                                                                                                    &AppWindow::WebMessageReceived).Get(), &token);
        }
    };

    HRESULT AppWindow::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment) {
        m_webViewEnv = environment;

        m_webViewEnv->CreateCoreWebView2Controller(
            m_mainWindow, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                this, &AppWindow::OnCreateCoreWebView2ControllerCompleted)
            .Get());

        return S_OK;
    };

    HRESULT AppWindow::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller) {
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

    bool AppWindow::HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result) {

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
                json* data = (json*)wParam;
                std::wstring response = str_to_wstr(data->dump());
                m_webView->PostWebMessageAsJson(response.c_str());
                delete data;
                return true;
            }
            case WM_KEYDOWN:
            {
                switch (wParam) {
                    case VK_F5:
                    {
                        std::string* key = (std::string*)wParam;
                        print_msg(str_to_wstr(*key));
                    }
                }
            }
            case WM_KEYUP:
            {
                switch (wParam) {
                    case VK_F5:
                    {
                        std::string* key = (std::string*)wParam;
                        print_msg(str_to_wstr(*key));
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
                        print_msg(str_to_wstr(*key));
                    }
                }
            }
            //case WM_KEYUP:
                //break;

        }
        return false;
    };

    LRESULT CALLBACK AppWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (auto app = (AppWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA)) {
            LRESULT result = 0;
            if (app->HandleWindowMessage(hWnd, message, wParam, lParam, &result)) {
                return result;
            }
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    };

    HRESULT AppWindow::WebMessageReceived(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args) {
        wil::unique_cotaskmem_string message;
        args->TryGetWebMessageAsString(&message);

        json data = json::parse((std::wstring)message.get());
        std::wstring response = HandleWebMessage(data);

        webView->PostWebMessageAsJson(response.c_str());

        return S_OK;
    };

}   //namespace scriptpad

#endif  //_SCRIPT_PAD_APP_WINDOW_CPP