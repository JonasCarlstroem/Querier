#pragma once

#ifndef _APP_H
    #define _APP_H

#include "NodeJS.h"
#include "Npm.h"

static npm::NPM npmjs;
static nodejs::NodeJS node(L"C:\\Program Files\\nodejs\\node.exe");

#endif