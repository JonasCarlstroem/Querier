<template>
    <div class="wrapper">
        <div class="action">
            <ActionBar />
        </div>
        <div class="main">
            <TopBar
                :available-languages="availableLanguages" 
                v-model:active-language="currentLanguage"
                @update:active-language="listen"
                @invoke="onInvoke"/>
            <div class="surface">
                <div class="editor" v-bind:style="`height: ${editorHeight}%;`">
                    <EditorSurface ref="editorRef"
                        :language="currentLanguage"
                        v-on:update:code="setCode"/>
                </div>
                <div class="result" v-if="showResult">
                    <ResultSurface ref="resultRef"
                        :hasResult="showResult" 
                        :result="result"/>
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
            const {cmd, message, error} = e.data;
            switch(cmd) {
                case "initialize":
                    if(message) {
                        this.code = message;

                        if(this.editorRef) {
                            this.editorRef.updateCode(this.code);
                        }
                    }
                    break;
                case "result":
                    if(message) {
                        const arr = split(message, "\n");
                        if(this.result.strings === undefined || this.result.strings === null) {
                            this.result.strings = [];
                        }
                        for(const item of arr) {
                            try {
                                const obj = JSON.parse(item);
                                Object.assign(this.result, obj);
                            }
                            catch(ex) {
                                console.log(ex);
                                this.result.strings.push(item);
                                // Object.assign(this.result, { value: item });
                            }
                        }

                        console.log(this.result);
                        if(this.editorRef) {
                            this.editorRef.resizeEditor();
                        }
                    }
                    break;
            }
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
            postWebMessage({ cmd: "invoke", message: this.code });
            this.isRunning = true;
            this.editorRef?.resizeEditor();
        },
        setCode(code:string) {
            this.code = code;
            postWebMessage({cmd: "codesync", message: this.code});
        }
    },
    computed: {
        _hasResult() {
            return this.showResult;
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
