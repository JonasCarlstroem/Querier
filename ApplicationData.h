#pragma once

#ifndef _QUERIER_APPLICATION_DATA_H
#define _QUERIER_APPLICATION_DATA_H

#include <string.h>
#include "resource.h"

namespace querier {
    enum ModuleResultType {
        NOMESSAGETYPE = 0,
        SUCCESS_RESULT,
        ERROR_RESULT
    };

    enum CommandType {
        NOCOMMANDTYPE = 0,
        MODULE,
        QUERY
    };

    enum ModuleCommand {
        NOMODULECOMMAND = 0,
        CONFIG_MODULE,
        CODESYNC_MODULE,
        INVOKE_MODULE,
        RESULT_MODULE
    };

    enum QueryCommand {
        NOQUERYCOMMAND = 0,
        INIT_QUERY,
        INIT_QUERY_MODULE,
        INVOKE_QUERY_MODULE,
        CODESYNC_QUERY_MODULE,
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

    struct Message {
        std::string content;
    };


    struct ModuleMessage : public Message {
        ModuleCommand cmd;
        ModuleResultType result_type;
        std::string module_name;
    };

    struct QueryMessage : public Message {
        QueryCommand cmd;
        ModuleResultType result_type;
        std::string query_name;
    };

    struct ApplicationMessage {
        CommandType type;
        ModuleMessage modmsg;
        QueryMessage querymsg;
    };

    void to_json(json& j, const Message&);
    void from_json(const json& j, Message&);

    void to_json(json& j, const QueryMessage&);
    void from_json(const json& j, QueryMessage&);

    void to_json(json& j, const ApplicationMessage&);
    void from_json(const json& j, ApplicationMessage&);

    NLOHMANN_JSON_SERIALIZE_ENUM(ModuleResultType, {
        { NOMESSAGETYPE, "no_message_type" },
        { SUCCESS_RESULT, "success_result" },
        { ERROR_RESULT, "error_result" }
    });

    NLOHMANN_JSON_SERIALIZE_ENUM(CommandType, {
        { NOCOMMANDTYPE, "no_command_type" },
        { MODULE, "module" },
        { QUERY, "query" }
    });

    NLOHMANN_JSON_SERIALIZE_ENUM(ModuleCommand, {
        { NOMODULECOMMAND, "no_module_command" },
        { CONFIG_MODULE, "config_module" },
        { CODESYNC_MODULE, "codesync_module" },
        { INVOKE_MODULE, "invoke_module" },
        { RESULT_MODULE, "result_module" }
    });

    NLOHMANN_JSON_SERIALIZE_ENUM(QueryCommand, {
        { NOQUERYCOMMAND, "no_query_command" },
        { INIT_QUERY, "init_query" },
        { INIT_QUERY_MODULE, "init_query_module" },
        { INVOKE_QUERY_MODULE, "invoke_query_module" },
        { CODESYNC_QUERY_MODULE, "codesync_query_module" },
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