#pragma once

#ifndef _APP_H
    #define _APP_H

#include "AppWindow.h"
#include "NodeJS.h"
#include <locale>
#include <codecvt>
#include "Util.h"

using json = nlohmann::json;

HRESULT WebMessageReceived(ICoreWebView2* webView, ICoreWebView2WebMessageReceivedEventArgs* args);

app::Message ParseMessage(json*);

static nodejs::NodeJS node(L"C:\\Program Files\\nodejs\\node.exe");
static ICoreWebView2* webview;

//void to_json(json&, const app::Message&);
//void from_json(const json&, app::Message&);



#endif