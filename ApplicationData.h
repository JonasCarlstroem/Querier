#pragma once

#ifndef _QUERIER_APPLICATION_DATA_H
#define _QUERIER_APPLICATION_DATA_H

#include <string.h>
#include "resource.h"

namespace querier {
    enum MessageType {
        NOMESSAGETYPE = 0,
        SUCCESS_RESULT,
        ERROR_RESULT,
        QUERY_RESPONSE,
        MODULE_RESPONSE
    };

    struct Message {
        MessageType msg_type;
        std::string content{ 0 };
    };

    enum CommandType {
        NOCOMMANDTYPE = 0,
        MODULE,
        QUERY
    };

    enum ModuleCommand {
        NOMODULECOMMAND = 0,
        INIT_MODULE,
        CONFIG_MODULE,
        CODESYNC_MODULE,
        INVOKE_MODULE,
        RESULT_MODULE
    };

    enum QueryCommand {
        NOQUERYCOMMAND = 0,
        INIT_QUERY,
        NEW_QUERY,
        LOAD_QUERY,
        CLOSE_QUERY,
        REMOVE_QUERY
    };

    enum ResultType {
        NORESULT,
        ISSUCCESS,
        ISERROR
    };

    struct ApplicationMessage {
        CommandType type = NOCOMMANDTYPE;
        ModuleCommand modcmd = NOMODULECOMMAND;
        QueryCommand querycmd = NOQUERYCOMMAND;
        Message message;
    };

    void to_json(json& j, const Message& m);
    void from_json(const json& j, Message& m);

    void to_json(json& j, const ApplicationMessage&);
    void from_json(const json& j, ApplicationMessage&);

    NLOHMANN_JSON_SERIALIZE_ENUM(MessageType, {
        { NOMESSAGETYPE, "no_message_type" },
        { SUCCESS_RESULT, "success_result" },
        { ERROR_RESULT, "error_result" },
        { QUERY_RESPONSE, "query_response" },
        { MODULE_RESPONSE, "module_response"}
    });

    NLOHMANN_JSON_SERIALIZE_ENUM(CommandType, {
        { NOCOMMANDTYPE, "no_command_type" },
        { MODULE, "module" },
        { QUERY, "query" }
    });

    NLOHMANN_JSON_SERIALIZE_ENUM(ModuleCommand, {
        { NOMODULECOMMAND, "no_module_command" },
        { INIT_MODULE, "init_module" },
        { CONFIG_MODULE, "config_module" },
        { CODESYNC_MODULE, "codesync_module" },
        { INVOKE_MODULE, "invoke_module" },
        { RESULT_MODULE, "result_module" }
    });

    NLOHMANN_JSON_SERIALIZE_ENUM(QueryCommand, {
        { NOQUERYCOMMAND, "no_query_command" },
        { INIT_QUERY, "init_query" },
        { NEW_QUERY, "new_query" },
        { LOAD_QUERY, "load_query" },
        { CLOSE_QUERY, "close_query" },
        { REMOVE_QUERY, "remove_query" }
    });

    NLOHMANN_JSON_SERIALIZE_ENUM(ResultType, {
        { NORESULT, "noresult" },
        { ISSUCCESS, "success" },
        { ISERROR, "error" }
    });
}

#endif