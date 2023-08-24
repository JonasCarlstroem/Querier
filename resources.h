#pragma once

#ifndef _SCIRPT_PAD_RESOURCES
#define _SCIRPT_PAD_RESOURCES

#define MODULE_INTERFACE __declspec(dllexport)
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#endif