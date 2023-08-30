<template>
    <div class="wrapper" @mouseup="endDragging()">
        <div class="action" :style="{
            width: `${resize.divider.horizontal}%`
        }">
            <ActionBar ref="actionBar" />
        </div>
        <v-divider id="dividerhorizontal" class="divideh" @mousedown="startDragging('horizontal')" vertical thickness="10"></v-divider>
        <div class="main" :style="{
            width: `${100 - resize.divider.horizontal}%`
        }">
            <TopBar ref="topBar" :available-languages="availableLanguages" v-model:active-language="currentLanguage"
                @update:active-language="listen" @invoke="onInvoke" />
            <div class="surface">
                <div class="editor" :style="{
                    height: `${(showResult ? resize.divider.vertical : 100)}%`
                }">
                    <EditorSurface ref="editorRef" :language="currentLanguage" v-on:update:code="setCode" />
                </div>
                <v-divider id="dividervertical" class="dividev" @mousedown="startDragging('vertical')" thickness="10" v-if="showResult"></v-divider>
                <div class="result" v-if="showResult" :style="{
                    height: `${100 - resize.divider.vertical}%`
                }">
                    <ResultSurface ref="resultRef" 
                        :hasResult="showResult" 
                        :result="result"
                        :hasError="hasError"
                        :error="error" />
                </div>
            </div>
        </div>
    </div>
</template>

<script lang="ts">
import { toRef } from 'vue';
import ActionBar from './components/ActionBar.vue';
import TopBar from './components/TopBar.vue';
import EditorSurface from './components/EditorSurface.vue';
import ResultSurface from './components/ResultSurface.vue';
import { reactive } from 'vue';
import { toRefs } from 'vue';
import { ref } from 'vue';
//@ts-ignore
import type { ILanguage } from '@/interface/ILanguage';
import { postWebMessage, split } from './Utils';

//####TODO
// wait for init message before creating editor

export default {
    components: {
        'ActionBar': ActionBar,
        'TopBar': TopBar,
        'EditorSurface': EditorSurface,
        'ResultSurface': ResultSurface
    },
    setup() {
        const actionBar = ref<InstanceType<typeof ActionBar> | null>(null);
        const topBar = ref<InstanceType<typeof TopBar> | null>(null);
        const editorRef = ref<InstanceType<typeof EditorSurface> | null>(null);
        const resultRef = ref<InstanceType<typeof ResultSurface> | null>(null);

        const availableLanguages: ILanguage[] = [
            { name: "JavaScript", id: "javascript", value: "function init() {\n\tconsole.log('hello')\n}" },
            { name: "TypeScript", id: "typescript", value: "function init(): void {\n\tconsole.log('hello')\n}" },
            { name: "HTML", id: "html", value: "<html>\n\t<head>\n\n</head>\n\r<body>\n\n</body>" },
            { name: "JSON", id: "json", value: "{\n\"sample\": \"value\"\n}" },
            { name: "C#", id: "csharp", value: "void main()\n{\n}" },
            { name: "C++", id: "cplusplus", value: "int main(int argc, const** argv) {\nstd::cout << \"Hello, world!\"}" }
        ];

        const defaultLanguage = availableLanguages[0];
        const currentLanguage = toRef(defaultLanguage);

        const state = reactive({
            code: currentLanguage.value.value as string,
            showResult: false as boolean,
            isRunning: false as boolean,
            result: {} as any,
            hasError: false as boolean,
            error: {} as any,
            editorHeight: 100 as number,
            resize: {
                divider: {
                    horizontal: 15,
                    vertical: 70
                },
                direction: ""
            },
            pause: false
        });

        return {
            editorRef,
            resultRef,
            actionBar,
            topBar,
            availableLanguages,
            defaultLanguage,
            currentLanguage,
            ...toRefs(state)
        };
    },
    beforeMount() {
        //@ts-ignore
        window.chrome.webview.addEventListener("message", (e) => {
            console.log("Message received");
            const { cmd, resultType, message, error } = e.data;
            this.handleCommand(cmd, resultType, message, error);
        });
    },
    methods: {
        getSizePercentage(left: number, right: number) {
            const percentage = (left / right) * 100;
            if(percentage >= 10 && percentage <= 90) 
                return Number(percentage.toFixed(2));
            return percentage <= 10 ? 10 : 90;
        },
        handleResizeVertical(e: any) {
            const topBarHeight = this.topBar!.$el.clientHeight;
            this.resize.divider.vertical = this.getSizePercentage((e.pageY - topBarHeight), (window.innerHeight - topBarHeight));
        },
        handleResizeHorizontal(e: any) {
            this.resize.divider.horizontal = this.getSizePercentage(e.pageX, window.innerWidth);
        },
        handleDragging(e: any) {
            console.log(e);
            const percentage = (e.pageX / window.innerWidth) * 100;

            if(percentage >= 10 && percentage <= 90) {
                this.resize.divider.horizontal = Number(percentage.toFixed(2));
            }
        },
        startDragging(direction: string) {
            console.log(direction);
            this.resize.direction = direction;
            if(direction === "vertical")
                document.addEventListener('mousemove', this.handleResizeVertical);
            else if(direction === "horizontal")
                document.addEventListener('mousemove', this.handleResizeHorizontal);
        },
        endDragging() {
            const dir = this.resize.direction;
            if(dir === "vertical")
                document.removeEventListener('mousemove', this.handleResizeVertical);
            else if(dir === "horizontal")
                document.removeEventListener('mousemove', this.handleResizeHorizontal);
        },
        listen(language: { name: string, id: string, value: string }) {
            console.log("Listener from App");
            this.currentLanguage = language;
            console.log("Causes error");
        },
        onInvoke() {
            this.showResult = true;
            this.editorHeight = 50;
            this.result = {};
            this.error = {};
            this.hasError = false;
            postWebMessage({ cmd: "invoke" });
            this.isRunning = true;
            this.editorRef?.resizeEditor();
        },
        setCode(code: string) {
            this.code = code;
            postWebMessage({ cmd: "codesync", message: this.code });
        },
        handleCommand(cmd: string, resultType: string, message: string, error: string) {
            switch (cmd) {
                case "initialize":
                        console.log("Initialize");
                        this.code = message;
                        console.log(this.editorRef);
                        if (this.editorRef) {
                            console.log("Has ref");
                            // this.editorRef.updateCode(this.code);
                            this.editorRef.initEditor(this.code);
                        }
                    break;
                case "result":
                    this.handleResult(resultType, message, error);
                    break;
            }
        },

        handleResult(resultType: string, message: string, error: string) {
            switch (resultType) {
                case "success":
                    if (message) {
                        console.log(message);
                        if(this.result.strings === undefined || this.result.strings === null)
                            this.result.strings = [];

                        if(this.result.functions === undefined || this.result.functions === null)
                            this.result.functions = [];

                        if(this.result.objects === undefined || this.result.functions === null)
                            this.result.objects = [];

                        this.setResult(message);

                        if (this.editorRef) {
                            this.editorRef.resizeEditor();
                        }
                    }
                    break;
                case "error":
                    if (error) {
                        this.hasError = true;
                        if(this.error.strings === undefined || this.error.strings === null)
                            this.error.strings = [];

                        if(this.error.objects === undefined || this.error.objects === null)
                            this.error.objects = [];

                        this.setError(error);
                    }
                    break;
            }
        },

        setResult(message: string) {
            const arr = split(message, "\n");
            for(const item of arr) {
                console.log(item);
                const obj = JSON.parse(item);

                if(obj.hasOwnProperty("object")) {
                    this.result.objects.push(obj.object);
                }
                else if(obj.hasOwnProperty("function")) {
                    this.result.strings.push(obj.function + '\r\n');
                }
                else if(obj.hasOwnProperty("string")) {
                    this.result.strings.push(obj.string);
                }
                console.log(obj);
            }
            
            console.log(this.result);
        },
        setError(error: string) {
            const arr = split(error, "\n");
            for(const item of arr) {
                try {
                    const obj = JSON.parse(item);
                    Object.assign(this.error, obj);
                }
                catch(ex) {
                    this.error.strings.push(item);
                }
            }
        }
    },
    computed: {
        _hasResult() {
            return this.showResult;
        },
        _hasError() {
            return this.hasError;
        }
    }
}
</script>

<style scoped>
.wrapper {
    box-sizing: border-box;
    width: 100%;
    height: 100%;
    margin: 0;
    padding: 0;
    display: flex;
    flex-flow: row;
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
    overflow: hidden;
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
    margin: 0 auto;
}
</style>
