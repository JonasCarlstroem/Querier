/**
 * @alias
 */
export type CommandType = "module" | "query";
export type ModuleCommand = "init_module" | "config_module" | "codesync_module" | "invoke_module" | "result_module";
export type QueryCommand = "init_query" | "new_query" | "load_query" | "close_query" | "remove_query";
export type MessageType = "success_result" | "error_result" | "query_response" | "module_response";
export interface Message {
    msg_type?: MessageType;
    content: string;
}

export function postWebMessage(appmessage: {cmdtype: CommandType, modcmd?: ModuleCommand, querycmd?: QueryCommand,  message?: Message }) {
    console.log(appmessage);
    //@ts-ignore
    window.chrome.webview.postMessage(JSON.stringify(appmessage));
}

export function split(val: string, delimiter: string) {
    const arr = [];
    let temp = "";
    for(const char of val) {
        if(char === delimiter) {
            arr.push(temp);
            temp = "";
        }
        else {
            temp += char;
        }
    }
    return arr;
}