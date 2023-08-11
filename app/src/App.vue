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
            <div class="editor">
                <EditorSurface 
                    v-bind:language="currentLanguage"
                    v-on:update:code="setCode"/>
                <ResultSurface />
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

// const editorSurface = ref<HTMLElement | null>(null);

export default {
    components: {
        'ActionBar': ActionBar,
        'TopBar': TopBar,
        'EditorSurface': EditorSurface,
        'ResultSurface': ResultSurface
    },
    setup() {
        const availableLanguages = [
            { name: "JavaScript", id: "javascript", value: "function init() {\n\tconsole.log('hello')\n}" },
            { name: "TypeScript", id: "typescript", value: "function init(): void {\n\tconsole.log('hello')\n}" },
            { name: "HTML", id: "html", value: "<html>\n\t<head>\n\n</head>\n\r<body>\n\n</body>" },
            { name: "JSON", id: "json", value: "{\n\"sample\": \"value\"\n}" },
            { name: "C#", id: "csharp", value: "void main()\n{\n}" },
            { name: "C++", id: "cplusplus", value: "int main(int argc, const** argv) {\nstd::cout << \"Hello, world!\"}" }
        ];
        const defaultLanguage = availableLanguages[0];
        const currentLanguage = toRef(defaultLanguage);
        let code: string = "";
        return {
            availableLanguages,
            defaultLanguage,
            currentLanguage,
            code
        };
    },
    mounted() {
        //@ts-ignore
        window.chrome.webview.addEventListener("message", (e) => {
            console.log(e);
        });
    },
    methods: {
        listen(language: { name: string, id: string, value: string }) {
            console.log("Listener from App");
            this.currentLanguage = language;
            console.log("Causes error");
        },
        onInvoke(a:any, b:any, c:any, d:any) {
            console.log(a);
            console.log(b);
            console.log(c);
            console.log(d);
            const msg = {
                command: "test",
                value: "text"
            };
            //@ts-ignore
            window.chrome.webview.postMessage(JSON.stringify(msg));
        },
        setCode(code:string) {
            this.code = code;
            console.log(this.code);
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
.editor {
    height: 100%;
    width: 100%;
    line-height: 1.5;
    display: flex;
    flex-direction: column;
}
</style>
