<template>
    <div class="wrapper">
        <div class="action">
            <ActionBar />
        </div>
        <div class="main">
            <TopBar :available-languages="availableLanguages" v-model:active-language="currentLanguage"
                @update:active-language="listen" @invoke="onInvoke" />
            <div class="surface">
                <div class="editor" v-bind:style="`height: ${editorHeight}%;`">
                    <EditorSurface ref="editorRef" :language="currentLanguage" v-on:update:code="setCode" />
                </div>
                <div class="result" v-if="showResult">
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

// const editorSurface = ref<HTMLElement | null>(null);

export default {
    components: {
        'ActionBar': ActionBar,
        'TopBar': TopBar,
        'EditorSurface': EditorSurface,
        'ResultSurface': ResultSurface
    },
    setup() {
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
            editorHeight: 100 as number
        });
        return {
            editorRef,
            resultRef,
            availableLanguages,
            defaultLanguage,
            currentLanguage,
            ...toRefs(state)
        };
    },
    beforeMount() {
        //@ts-ignore
        window.chrome.webview.addEventListener("message", (e) => {
            const { cmd, resultType, message, error } = e.data;
            this.handleCommand(cmd, resultType, message, error);
        });
    },
    methods: {
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
            postWebMessage({ cmd: "invoke", message: this.code });
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
                    if (message) {
                        this.code = message;

                        if (this.editorRef) {
                            this.editorRef.updateCode(this.code);
                        }
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
                        // this.result = message;
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

                        if(this.error.functions === undefined || this.error.functions === null)
                            this.error.functions = [];

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
                    this.result.functions.push(obj.function);
                }
                else if(obj.hasOwnProperty("string")) {
                    this.result.strings.push(obj.string);
                }
                console.log(obj);
            }
            // const msg = JSON.parse(message);
            console.log(this.result);
            
            // for (const item of arr) {
            //     try {
            //         const obj = JSON.parse(item);
            //         Object.assign(this.result, { objects: obj });
            //     }
            //     catch (ex) {
            //         this.result.strings.push(item);
            //         // Object.assign(this.result, { value: item });
            //     }
            // }
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
    display: flex;
    flex-flow: row;
}

.main {
    width: 100%;
    display: flex;
    flex-direction: column;
}

.surface {
    height: 100%;
    width: 100%;
    line-height: 1.5;
    display: flex;
    flex-direction: column;
}

.editor {
    height: 100%;
    width: 100%;
}

.result {
    height: 100%;
    width: 100%;
}
</style>
