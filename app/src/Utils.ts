export type ModuleResultType = "success_result" | "error_result";
export type CommandType = "module" | "query";
export type ModuleCommand = "init_module" | "config_module" | "codesync_module" | "invoke_module" | "result_module" | "get_modules";

export type QueryCommand = "init_query" | "init_query_module" | "invoke_query_module" | "new_query" | "load_query" | "close_query" | "remove_query" | "codesync_query_module";
export type MessageType = "success_result" | "error_result" | "query_response" | "module_response";

export type LanguageType = "interpreter" | "compiler";

export interface Message {
    content?: string;
}

export interface QueryMessage extends Message {
    cmd: QueryCommand;
    query_name: string;
}

export interface ModuleMessage extends Message {
    cmd: ModuleCommand;
    module_name?: string;
    result_type?: ModuleResultType;
}

export interface Result {
    objects: string[];
    functions: string[];
    strings: string[];
}

export class Query {
    query_name: string = "";
    query_path: string = "";
    query_module: string = "";
    query_module_version: string = "";
    query_source: string = "";
    query_source_content: string = "";
    query_libraries: string[] = [];
    unsaved_changes: boolean = false;

    isRunning?: boolean = false;
    showResult?: boolean = false;
    result: Result = {
        objects: [] = [],
        functions: [] = [],
        strings: [] = []
    };
    showError?: boolean = false;
    error: any = {};
}

export class Module {
    name?: string;
    sourceFileExtension?: string;
    library?: string;
    type?: LanguageType;
}

export function postWebMessage(appmessage: {cmdtype: CommandType, modmsg?: ModuleMessage, querymsg?: QueryMessage }) {
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