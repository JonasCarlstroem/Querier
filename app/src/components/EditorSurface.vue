<template>
    <div class="editorSurface" ref="editorSurface" :height="editorHeight">

    </div>
</template>

<script lang="ts">
import { ref } from 'vue';
import * as monaco from 'monaco-editor';
import { reactive } from 'vue';
import { toRefs } from 'vue';

export default {
    expose: ["resizeEditor"],
    props: {
        language: {
            type: Object,
            default: { },
        }
    },
    emits: [
        "update:code"
    ],
    setup(props) {
        const state = reactive({
            code: props.language.value as string,
            editorHeight: "100%" as string
        });
        const editorSurface = ref<HTMLElement | null>(null);

        return {
            ...toRefs(state),
            editorSurface,
            editor: null as monaco.editor.IStandaloneCodeEditor | null
        }
    },
    mounted() {
        if(this.editorSurface !== null) {
            console.log(this.language);
            this.editor = monaco.editor.create(this.editorSurface, {
                value: this.code,
                language: this.language.id,
                automaticLayout: true
            });

            this.editor.onDidChangeModelContent((e) => {
                const value = this.editor?.getValue();
                this.$emit('update:code', value);
            });
        }
    },
    methods: {
        resizeEditor() {
            this.$nextTick(() => {
                this.editorHeight = "50%;";
                this.editor?.layout();
                console.log("Next tick");
                console.log(this.editorSurface?.style.height);
            });
        }
    },
    computed: {
        
    },
    watch: {
        language: {
            handler() {
                console.log("language");
                if(!this.editor) return;
                const model = this.editor.getModel();
                if(model !== null) {
                    monaco.editor.setModelLanguage(model, this.language.id);
                }
                this.editor.getModel()
            },
            deep: true,
            immediate: true
        }
    }
}
</script>

<style scoped>
.editorSurface {
    height: 100%;
    border: 1px solid white;
}
</style>