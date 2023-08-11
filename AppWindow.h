#pragma once

#ifndef _APP_WINDOW_H
    #define _APP_WINDOW_H

#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"
#include "Process.h"
#include "Util.h"
#include <nlohmann/json.hpp>

namespace app {
    using json = nlohmann::json;
    using namespace Microsoft::WRL;

    static TCHAR szWindowClass[] = _T("DesktopApp");
    static TCHAR szTitle[] = _T("Script pad desktop application");

    enum Command { INVOKE };

    struct Message {
        Command _cmd;
        std::string cmd;
        std::string value;
        bool respond{ 0 };
    };

    class AppWindow {
    public:
        AppWindow(HINSTANCE, int);

        bool show();

    private:
        HINSTANCE hInst{ 0 };
        WNDCLASSEX wcEx{ 0 };
        HWND hWnd{ 0 };
        int nShow;

        void create_webview();
        Message parse_message(json);
        std::wstring get_response(Message);

        static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
        static wil::com_ptr<ICoreWebView2Controller> webviewController;
        static wil::com_ptr<ICoreWebView2> webview;
    };

    void to_json(json& j, const Message& m) {
        j = json{ { "cmd", m.cmd }, { "value", m.value }, {"cmd", m._cmd} };
    }

    void from_json(const json& j, Message& m) {
        j.at("cmd").get_to(m.cmd);
        j.at("cmd").get_to(m._cmd);
        j.at("value").get_to(m.value);
    }
} //namespace app

#endif