#include "AppWindow.h"

namespace app {
    AppWindow::AppWindow(HINSTANCE hInstance, int nCmdShow) : hInst(hInstance), nShow(nCmdShow) {
        wcEx = WNDCLASSEX();

        wcEx.cbSize = sizeof(WNDCLASSEX);
        wcEx.style = CS_HREDRAW | CS_VREDRAW;
        wcEx.lpfnWndProc = WndProc;
        wcEx.cbClsExtra = 0;
        wcEx.cbWndExtra = 0;
        wcEx.hInstance = hInstance;
        wcEx.hIcon = LoadIcon(wcEx.hInstance, IDI_APPLICATION);
        wcEx.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcEx.lpszMenuName = NULL;
        wcEx.lpszClassName = szWindowClass;
        wcEx.hIconSm = LoadIcon(wcEx.hInstance, IDI_APPLICATION);

        if (!RegisterClassExW(&wcEx)) {
            MessageBox(NULL,
                       _T("Call to RegisterClassEx failed!"),
                       _T("ScriptPad Desktop Application"),
                       NULL);

            THROW_EXCEPTION(exception_code);
        }

        hWnd = CreateWindowEx(
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
                       _T("ScriptPad Desktop Application"),
                       NULL);

            THROW_EXCEPTION(exception_code);
        }
    }

    bool AppWindow::show() {
        ShowWindow(hWnd,
                   nShow);
        UpdateWindow(hWnd);

        create_webview();
    }

    void AppWindow::create_webview() {
        CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
                                                 Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                                                 [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

                                                     // Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
                                                     env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                                         [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
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
                                                                 [this](ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                                                     wil::unique_cotaskmem_string message;
                                                                     args->TryGetWebMessageAsString(&message);
                                                                     std::wstring data = message.get();
                                                                     Message cmd = parse_message(json::parse(data));

                                                                     if (cmd.respond) {
                                                                         std::wstring response = get_response(cmd);
                                                                         webview->PostWebMessageAsJson(response.c_str());
                                                                     }

                                                                     return S_OK;
                                                                 }
                                                             ).Get(), &token);

                                                             return S_OK;
                                                         }).Get());
                                                     return S_OK;
                                                 }).Get());
    }

    Message AppWindow::parse_message(json data) {
        return data.template get<Message>();
    }

    std::wstring AppWindow::get_response(Message cmd) {
        return std::wstring();
    }

    LRESULT CALLBACK AppWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        PAINTSTRUCT ps;
        HDC hdc;

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
}