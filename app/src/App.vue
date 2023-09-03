<template>
    <div class="wrapper" @mouseup="endDragging()">
        <div class="action" :style="{
            width: `${resize.divider.horizontal}%`
        }">
            <ActionBar ref="actionBar" />
        </div>
        <v-divider id="dividerhorizontal" class="divideh" @mousedown="startDragging('horizontal')" vertical
            thickness="10"></v-divider>
        <div class="main" :style="{
            width: `${100 - resize.divider.horizontal}%`
        }">
            <v-tabs 
                class="mt-1 ml-1"
                density="compact"
                :align-tabs="'start'"
                :height="20"
                v-model="query"
                :hide-slider="false">
                <v-tab v-for="_tab in _tabs" :value="_tab.value" variant="plain">
                    {{ _tab.name }}
                </v-tab>
                <v-tab @click="postNewQuery" varian="text">
                    <!-- New Query -->
                    <v-icon start>mdi-plus</v-icon>
                </v-tab>
            </v-tabs>
            <v-window v-model="query" class="h-100">
                <div class="query-surface">
                    <TopBar ref="topBar" :available-modules="availableLanguages" :query-name="query.query_name" :module-name="query.query_module" :module-version="query.query_module_version" v-model:active-module="currentLanguage"
                        @update:active-language="listen" @invoke="invoke" @config="showSettingsWindow" />
                    <div class="surface">
                        <div class="editor" :style="{
                            height: `${(query.showResult ? resize.divider.vertical : 100)}%`
                        }">
                            <EditorSurface ref="editorRef" v-if="modulesInitialized" :language="currentLanguage" @update:code="setCode" @loaded="postInitQuery" />
                        </div>
                        <v-divider id="dividervertical" class="dividev" @mousedown="startDragging('vertical')" thickness="10"
                            v-if="query.showResult"></v-divider>
                        <div class="result" v-if="query.showResult" :style="{
                            height: `${100 - resize.divider.vertical}%`
                        }">
                            <ResultSurface ref="resultRef" :hasResult="query.showResult" :result="query.result" :hasError="query.showError"
                                :error="query.error" />
                        </div>
                    </div>
                </div>
            </v-window>
        </div>
    </div>

    <Settings :show-dialog="showSettings" v-model="showSettings" />
</template>

<script lang="ts">
import { toRef, reactive, toRefs, ref } from 'vue';
import ActionBar from './components/ActionBar.vue';
import TopBar from './components/TopBar.vue';
import EditorSurface from './components/EditorSurface.vue';
import ResultSurface from './components/ResultSurface.vue';
import Settings from './components/Settings.vue';
//@ts-ignore
import type { ILanguage } from '@/interface/ILanguage';
import { postWebMessage, split } from './Utils';
import type { CommandType, ModuleCommand, QueryCommand, Message, Query, Module, ModuleMessage, QueryMessage } from './Utils';

//####TODO
// wait for init message before creating editor

export default {
    components: {
        'ActionBar': ActionBar,
        'TopBar': TopBar,
        'EditorSurface': EditorSurface,
        'ResultSurface': ResultSurface,
        'Settings': Settings
    },
    setup() {
        const actionBar = ref<InstanceType<typeof ActionBar> | null>(null);
        const topBar = ref<InstanceType<typeof TopBar> | null>(null);
        const editorRef = ref<InstanceType<typeof EditorSurface> | null>(null);
        const resultRef = ref<InstanceType<typeof ResultSurface> | null>(null);

        const availableModules: ILanguage[] = [
            { name: "JavaScript", id: "javascript", value: "function init() {\n\tconsole.log('hello')\n}" },
            { name: "TypeScript", id: "typescript", value: "function init(): void {\n\tconsole.log('hello')\n}" },
            { name: "HTML", id: "html", value: "<html>\n\t<head>\n\n</head>\n\r<body>\n\n</body>" },
            { name: "JSON", id: "json", value: "{\n\"sample\": \"value\"\n}" },
            { name: "C#", id: "csharp", value: "void main()\n{\n}" },
            { name: "C++", id: "cplusplus", value: "int main(int argc, const** argv) {\nstd::cout << \"Hello, world!\"}" }
        ];

        const defaultModule = availableModules[0];
        const currentModule = toRef(defaultModule);

        const state = reactive({
            code: currentModule.value.value as string,
            // showResult: false as boolean,
            // isRunning: false as boolean,
            // result: {} as any,
            // hasError: false as boolean,
            // error: {} as any,
            editorHeight: 100 as number,
            resize: {
                divider: {
                    horizontal: 8,
                    vertical: 75
                },
                direction: ""
            },
            showSettings: false,
            queries: [] as Query[],
            query: {} as Query,
            modules: [] as Module[],
            modulesInitialized: false,
            queriesInitialized: false
        });

        return {
            editorRef,
            resultRef,
            actionBar,
            topBar,
            availableLanguages: availableModules,
            defaultLanguage: defaultModule,
            currentLanguage: currentModule,
            ...toRefs(state)
        };
    },
    beforeMount() {
        //@ts-ignore
        window.chrome.webview.addEventListener("message", (e) => {
            const { cmdtype, modmsg, querymsg } = e.data;
            this.handleCommand(cmdtype, modmsg, querymsg);
        });

        document.addEventListener(
            "keydown",
            (event) => {
                const { key } = event;
                switch(key) {
                    case "F5":
                        this.invoke();
                        break;
                    default:
                        break;
                }
            }
        );
    },
    mounted() {
        this.postGetModules();
    },
    methods: {
        showSettingsWindow(opt: any) {
            this.showSettings = true;
        },
        closeSettingsWindow() {
            this.showSettings = false;
        },
        getSizePercentage(left: number, right: number) {
            const percentage = (left / right) * 100;
            if (percentage >= 5 && percentage <= 95)
                return Number(percentage.toFixed(2));
            return percentage <= 5 ? 5 : 95;
        },
        handleResizeVertical(e: any) {
            const topBarHeight = this.topBar!.$el.clientHeight;
            this.resize.divider.vertical = this.getSizePercentage((e.pageY - topBarHeight), (window.innerHeight - topBarHeight));
        },
        handleResizeHorizontal(e: any) {
            this.resize.divider.horizontal = this.getSizePercentage(e.pageX, window.innerWidth);
        },
        startDragging(direction: string) {
            console.log(direction);
            this.resize.direction = direction;
            if (direction === "vertical")
                document.addEventListener('mousemove', this.handleResizeVertical);
            else if (direction === "horizontal")
                document.addEventListener('mousemove', this.handleResizeHorizontal);
        },
        endDragging() {
            const dir = this.resize.direction;
            if (dir === "vertical")
                document.removeEventListener('mousemove', this.handleResizeVertical);
            else if (dir === "horizontal")
                document.removeEventListener('mousemove', this.handleResizeHorizontal);
        },
        listen(language: { name: string, id: string, value: string }) {
            console.log("Listener from App");
            this.currentLanguage = language;
            console.log("Causes error");
        },
        invoke() {
            this.editorHeight = 50;
            this.query.showResult = true;
            this.query.result = {
                objects: [],
                functions: [],
                strings: []
            };
            this.query.error = {};
            this.query.showError = false;
            this.postInvokeQueryModule();
            this.query.isRunning = true;
            this.editorRef?.resizeEditor();
        },
        setCode(code: string) {
            this.query.query_source_content = code;
            this.postCodeSyncQueryModule(code);
        },
        handleCommand(cmdtype: CommandType, modmsg: ModuleMessage, querymsg: QueryMessage) {
            switch (cmdtype) {
                case "module":
                    this.handleModuleCommand(modmsg);
                    break;
                case "query":
                    this.handleQueryCommand(querymsg);
                    break;
            }
        },
        handleModuleCommand(message: ModuleMessage) {
            switch (message.cmd) {
                case "get_modules":
                    const modules = JSON.parse(message!.content!);
                    this.modules = modules;
                    this.modulesInitialized = true;
                    console.log(modules);
                    break;
                case "init_module":
                    console.log(message);
                    this.code = message.content!;
                    if (this.editorRef) {
                        console.log(this.code);
                        this.editorRef.initEditor(this.code);
                    }
                    break;
                case "config_module":
                    break;
                case "result_module":
                    this.handleResult(message);
                    break;
                case "invoke_module":
                    break;
            }
        },
        handleQueryCommand(message: QueryMessage) {
            switch (message.cmd) {
                case "init_query":
                    let queries = JSON.parse(message.content!);
                    this.queries = queries;
                    this.query = queries[0];
                    this.postInitQueryModule();
                    break;
                case "init_query_module":
                    this.query.query_source_content = message.content!;
                    if (this.editorRef) {
                        this.editorRef.initEditor(this.query.query_source_content);
                    }
                    break;
                case "new_query":
                    break;
                case "load_query":
                    break;
                case "close_query":
                    break;
                case "remove_query":
                    break;
            }
        },
        handleResult(message: ModuleMessage) {
            switch (message.result_type) {
                case "success_result":
                    if (message.content) {
                        if (this.query.result.strings === undefined || this.query.result.strings === null)
                            this.query.result.strings = [];

                        if (this.query.result.functions === undefined || this.query.result.functions === null)
                            this.query.result.functions = [];

                        if (this.query.result.objects === undefined || this.query.result.functions === null)
                            this.query.result.objects = [];

                        this.setResult(message.content);

                        if (this.editorRef) {
                            this.editorRef.resizeEditor();
                        }
                    }
                    break;
                case "error_result":
                    if (message.content) {
                        this.query.showError = true;
                        if (this.query.error.strings === undefined || this.query.error.strings === null)
                            this.query.error.strings = [];

                        if (this.query.error.objects === undefined || this.query.error.objects === null)
                            this.query.error.objects = [];

                        this.setError(message.content);
                    }
                    break;
            }
        },

        setResult(message: string) {
            const arr = split(message, "\n");
            for (const item of arr) {
                const obj = JSON.parse(item);

                if (obj.hasOwnProperty("object")) {
                    this.query.result.objects?.push(obj.object);
                }
                else if (obj.hasOwnProperty("function")) {
                    this.query.result.strings?.push(obj.function + '\r\n');
                }
                else if (obj.hasOwnProperty("string")) {
                    this.query.result.strings?.push(obj.string);
                }
            }
        },
        setError(error: string) {
            const arr = split(error, "\n");
            for (const item of arr) {
                try {
                    const obj = JSON.parse(item);
                    Object.assign(this.query.error, obj);
                }
                catch (ex) {
                    this.query.error.strings.push(item);
                }
            }
        },
        postGetModules() {
            postWebMessage({
                cmdtype: "module",
                modmsg: {
                    cmd: "get_modules"
                }
            });
        },
        postInitQuery() {
            postWebMessage({ 
                cmdtype: "query", 
                querymsg: { 
                    cmd: "init_query", 
                    query_name: this.query.query_name 
                } 
            });
        },
        postNewQuery() {
            postWebMessage({ 
                cmdtype: "query", 
                querymsg: {
                    cmd: "new_query", 
                    query_name: this.query.query_name 
                } 
            });
        },
        postInitQueryModule() {
            postWebMessage({ 
                cmdtype: "query", 
                querymsg: {
                    cmd:"init_query_module", 
                    query_name: this.query.query_name
                } 
            });
        },
        postInvokeQueryModule() {
            postWebMessage({ 
                cmdtype: "query", 
                querymsg: { 
                    cmd: "invoke_query_module", 
                    query_name: this.query.query_name
                } 
            });
        },
        postConfigModule() {
            postWebMessage({ 
                cmdtype: "module", 
                modmsg: { 
                    cmd: "config_module", 
                    module_name: this.query.query_module 
                } 
            });
        },
        postCodeSyncQueryModule(content: string) {
            postWebMessage({
                cmdtype: "query", 
                querymsg: { 
                    cmd: "codesync_query_module", 
                    query_name: this.query.query_name, 
                    content: content 
                }
            });
        }
    },
    watch: {
        query: {
            handler(a, b) {
                this.editorRef?.updateCode(a.query_source_content);
            }
        }
    },
    computed: {
        _hasResult() {
            return this.query.showResult;
        },
        _showError() {
            return this.query.showError;
        },
        _tabs() {
            const tabs = this.queries.map((x: any) : { name: string, value: Query } => { 
                console.log(x);
                return { 
                    name: x.query_name[0].toUpperCase() + x.query_name.substring(1, x.query_name.length), 
                    value: x
                }
            });
            if(tabs.length > 0) {
                console.log(tabs);
                this.query = tabs[0].value;
            }
            return tabs;
        }
    }
}
</script>

<style scoped>

.wrapper {
    font-family: Consolas, 'Courier New', monospace;
    color: white;
    box-sizing: border-box;
    width: 100%;
    height: 100%;
    margin: 0;
    padding: 0;
    display: flex;
    flex-direction: row;
}

.divideh {
    cursor: ew-resize;
}

.dividev {
    cursor: ns-resize;
}

.main {
    box-sizing: border-box !important;
    width: 100%;
    display: flex;
    flex-direction: column;
}

.surface {
    box-sizing: border-box !important;
    height: 100%;
    width: 100%;
    line-height: 1.5;
    display: flex;
    flex-direction: column;
    overflow: hidden;
}

.editor {
    height: 100%;
    width: 100%;
}

.result {
    -webkit-box-sizing: border-box;
    box-sizing: border-box !important;
    height: 100%;
    width: 100%;
    overflow: hidden;
}

.query-surface {
    width: 100%;
    height: 100%;
    display: flex;
    flex-direction: column;
}
.query-close {
    position: absolute;
    top: 0;
    right: 0;
}
</style>
@/types/ILanguage