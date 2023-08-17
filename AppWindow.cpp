#ifndef _APP_WINDOW_CPP
    #define _APP_WINDOW_CPP

//#include "AppWindow.h"
//#include <memory>

namespace app {
    //using json = nlohmann::json;
    //AppWindow::AppWindow(HINSTANCE hInstance, int nCmdShow) : m_hInst(hInstance), m_nShow(nCmdShow) {
    //    m_wcEx = WNDCLASSEX();

    //    m_wcEx.cbSize = sizeof(WNDCLASSEX);
    //    m_wcEx.style = CS_HREDRAW | CS_VREDRAW;
    //    m_wcEx.lpfnWndProc = WndProc;
    //    m_wcEx.cbClsExtra = 0;
    //    m_wcEx.cbWndExtra = 0;
    //    m_wcEx.hInstance = hInstance;
    //    m_wcEx.hIcon = LoadIcon(m_wcEx.hInstance, IDI_APPLICATION);
    //    m_wcEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    //    m_wcEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    //    m_wcEx.lpszMenuName = NULL;
    //    m_wcEx.lpszClassName = szWindowClass;
    //    m_wcEx.hIconSm = LoadIcon(m_wcEx.hInstance, IDI_APPLICATION);

    //    if (!RegisterClassExW(&m_wcEx)) {
    //        MessageBox(NULL,
    //                   _T("Call to RegisterClassEx failed!"),
    //                   _T("ScriptPad Desktop Application"),
    //                   NULL);

    //        throw std::exception("Error registering window class");
    //    }

    //    InitializeWindow();
    //}

    //AppWindow::~AppWindow() {
    //    m_webViewEnv->Release();
    //    m_webViewCtrl->Close();
    //    m_webViewCtrl = nullptr;
    //    m_webView = nullptr;

    //    m_webViewEnv = nullptr;
    //}

    //void AppWindow::AddWebMessageReceivedHandler(HRESULT (*webMessage)(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args)) {
    //    WebMessage = webMessage;
    //    m_customWebMessageHandler = true;
    //}

    //void AppWindow::AddOnWebViewCreatedHandler(void (*OnWebViewCreatedHandler)(ICoreWebView2* webview)) {
    //    OnWebViewCreated = OnWebViewCreatedHandler;
    //}

    //bool AppWindow::Show() {
    //    ShowWindow(m_mainWindow,
    //        m_nShow);
    //    UpdateWindow(m_mainWindow);

    //    InitializeWebview();
    //    return true;
    //}

    //bool AppWindow::InitializeWindow() {
    //    m_mainWindow = CreateWindowEx(
    //        WS_EX_OVERLAPPEDWINDOW,
    //        szWindowClass,
    //        szTitle,
    //        WS_OVERLAPPEDWINDOW,
    //        CW_USEDEFAULT, CW_USEDEFAULT,
    //        1200, 900,
    //        NULL,
    //        NULL,
    //        m_hInst,
    //        NULL
    //    );

    //    if (!m_mainWindow) {
    //        MessageBox(NULL,
    //            _T("Call to CreateWindow failed!"),
    //            _T("ScriptPad Desktop Application"),
    //            NULL);

    //        throw std::exception("Error when creating window");
    //    }

    //    SetWindowLongPtr(m_mainWindow, GWLP_USERDATA, (LONG_PTR)this);

    //    return true;
    //}

    //void AppWindow::InitializeWebview() {
    //    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr, 
    //        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
    //        this, 
    //        &AppWindow::OnCreateEnvironmentCompleted).Get());

    //    if (!SUCCEEDED(hr)) {
    //        if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
    //            MessageBox(
    //                m_mainWindow,
    //                L"Couldn't find Edge intallation. "
    //                "Do you have a version installed that's compatible with this "
    //                "WebView2 SDK version?",
    //                nullptr, MB_OK);
    //        }
    //    }
    //}

    //void AppWindow::RegisterEventHandlers() {
    //    EventRegistrationToken token;

    //    if (m_customWebMessageHandler) {
    //        m_webView->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(WebMessage).Get(), &token);
    //    }
    //    else {
    //        m_webView->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(&WebMessageReceived).Get(), &token);
    //    }
    //}

    //HRESULT AppWindow::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment) {
    //    m_webViewEnv = environment;

    //    m_webViewEnv->CreateCoreWebView2Controller(
    //        m_mainWindow, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
    //        this, &AppWindow::OnCreateCoreWebView2ControllerCompleted)
    //        .Get());

    //    return S_OK;
    //}

    //HRESULT AppWindow::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller) {
    //    if (result == S_OK) {
    //        m_webViewCtrl = controller;

    //        m_webViewCtrl->get_CoreWebView2(&m_webView);

    //        wil::com_ptr<ICoreWebView2Settings> settings;
    //        m_webView->get_Settings(&settings);
    //        settings->put_IsScriptEnabled(TRUE);
    //        settings->put_AreDefaultScriptDialogsEnabled(TRUE);
    //        settings->put_IsWebMessageEnabled(TRUE);

    //        RECT bounds;
    //        GetClientRect(m_mainWindow, &bounds);
    //        m_webViewCtrl->put_Bounds(bounds);

    //        // Schedule an async task to navigate to Bing
    //        m_webView->Navigate(L"http://localhost:5173/");

    //        RegisterEventHandlers();
    //        if (OnWebViewCreated)
    //            OnWebViewCreated(m_webView.get());
    //    }
    //    return S_OK;
    //}

    //bool AppWindow::HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* result) {
    //    switch (message) {
    //        case WM_PAINT:
    //            PAINTSTRUCT ps;
    //            HDC hdc;
    //            hdc = BeginPaint(hWnd, &ps);

    //            return EndPaint(hWnd, &ps);
    //        case WM_DESTROY:
    //            PostQuitMessage(0);
    //            break;
    //        case WM_SIZE:
    //        case WM_SIZING:
    //            if (m_webViewCtrl != nullptr) {
    //                RECT bounds;
    //                GetClientRect(hWnd, &bounds);
    //                m_webViewCtrl->put_Bounds(bounds);

    //                return true;
    //            }
    //            break;
    //        case WM_WEBVIEW: 
    //            Message* data = (Message*)wParam;
    //            std::wstring response = GetResponse(*data);
    //            m_webView->PostWebMessageAsJson(response.c_str());
    //            return true;

    //    }
    //    return false;
    //}

    //LRESULT CALLBACK AppWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    //    if (auto app = (AppWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA)) {
    //        LRESULT result = 0;
    //        if (app->HandleWindowMessage(hWnd, message, wParam, lParam, &result)) {
    //            return result;
    //        }
    //    }
    //    return DefWindowProc(hWnd, message, wParam, lParam);
    //}

    //HRESULT AppWindow::WebMessageReceived(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args) {
    //    wil::unique_cotaskmem_string message;
    //    args->TryGetWebMessageAsString(&message);

    //    json data = json::parse((std::wstring)message.get());
    //    app::Message cmd = data.template get<app::Message>();
    //    std::wstring response = HandleWebMessage(&cmd);

    //    if (cmd.respond) {
    //        webView->PostWebMessageAsJson(response.c_str());
    //    }

    //    return S_OK;
    //}

    /*void to_json(json& j, const Message& m) {
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
    }*/
}

#endif