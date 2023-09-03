#ifndef _QUERIER_APPLICATION_DATA_CPP
#define _QUERIER_APPLICATION_DATA_CPP

#include "ApplicationData.h"

namespace querier {
    void to_json(json& j, const Message& m) {
        j = json{
            { "content", m.content}
        };
    }

    void from_json(const json& j, Message& m) {
        m.content = j.value("content", "");
    }

    void to_json(json& j, const ModuleMessage& m) {
        j = json{
            { "cmd", m.cmd },
            { "result_type", m.result_type },
            { "module_name", m.module_name },
            { "content", m.content }
        };
    }

    void from_json(const json& j, ModuleMessage& m) {
        m.cmd = j.value("cmd", NOMODULECOMMAND);
        m.result_type = j.value("result_type", ModuleResultType::NOMESSAGETYPE);
        m.module_name = j.value("module_name", "");
        m.content = j.value("content", "");
    }

    void to_json(json& j, const QueryMessage& q) {
        j = json{
            { "cmd", q.cmd },
            { "query_name", q.query_name },
            { "content", q.content }
        };
    }

    void from_json(const json& j, QueryMessage& q) {
        q.cmd = j.value("cmd", NOQUERYCOMMAND);
        q.query_name = j.value("query_name", "");
        q.content = j.value("content", "");
    }



    void to_json(json& j, const ApplicationMessage& am) {
        j = json{
            { "cmdtype", am.type },
            { "modmsg", am.modmsg },
            { "querymsg", am.querymsg }
            /*{ "modcmd", am.modcmd },
            { "querycmd", am.querycmd },
            { "message", am.message }*/
        };
    }

    void from_json(const json& j, ApplicationMessage& am) {
        am.type = j.value("cmdtype", CommandType::NOCOMMANDTYPE);
        am.modmsg = j.value("modmsg", ModuleMessage());
        am.querymsg = j.value("querymsg", QueryMessage());
        /*am.modcmd = j.value("modcmd", ModuleCommand::NOMODULECOMMAND);
        am.querycmd = j.value("querycmd", QueryCommand::NOQUERYCOMMAND);
        am.message = j.value("message", Message{});*/
        /*j.at("cmdtype").get_to<CommandType>(am.type);
        j.at("modcmd").get_to<ModuleCommand>(am.modcmd);
        j.at("querycmd").get_to<QueryCommand>(am.querycmd);
        j.at("message").get_to<Message>(am.message);*/
    }
}

#endif