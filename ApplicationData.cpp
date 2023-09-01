#ifndef _QUERIER_APPLICATION_DATA_CPP
#define _QUERIER_APPLICATION_DATA_CPP

#include "ApplicationData.h"

namespace querier {
    void to_json(json& j, const Message& m) {
        j = json{
            { "msg_type", m.msg_type },
            { "content", m.content }
        };
    }

    void from_json(const json& j, Message& m) {
        m.msg_type = j.value("msg_type", NOMESSAGETYPE);
        m.content = j.value("content", "");
    }

    void to_json(json& j, const ApplicationMessage& am) {
        j = json{
            { "cmdtype", am.type },
            { "modcmd", am.modcmd },
            { "querycmd", am.querycmd },
            { "message", am.message }
        };
    }

    void from_json(const json& j, ApplicationMessage& am) {
        am.type = j.value("cmdtype", CommandType::NOCOMMANDTYPE);
        am.modcmd = j.value("modcmd", ModuleCommand::NOMODULECOMMAND);
        am.querycmd = j.value("querycmd", QueryCommand::NOQUERYCOMMAND);
        am.message = j.value("message", Message{});
        /*j.at("cmdtype").get_to<CommandType>(am.type);
        j.at("modcmd").get_to<ModuleCommand>(am.modcmd);
        j.at("querycmd").get_to<QueryCommand>(am.querycmd);
        j.at("message").get_to<Message>(am.message);*/
    }
}

#endif