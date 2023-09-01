<template>
    <div class="editorSurface" ref="editorSurface" :height="editorHeight">

    </div>
</template>

<script lang="ts">
import { ref } from 'vue';
import * as monaco from 'monaco-editor';
import { reactive } from 'vue';
import { toRefs } from 'vue';
import { postWebMessage } from '@/Utils';

export default {
    expose: ["initEditor", "resizeEditor", "updateCode"],
    props: {
        language: {
            type: Object,
            default: {},
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
        this.postInitialize();
    },
    methods: {
        initEditor(modelValue: string) {
            if (this.editorSurface !== null) {
                const editorOptions = {
                    automaticLayout: true,
                    language: this.language.id,
                    model: null
                };

                monaco.languages.typescript.javascriptDefaults.addExtraLib([
                    "function dump(obj: any): void;",
                    "function sleep(ms: Number): void;"
                ].join('\n'));

                monaco.languages.typescript.javascriptDefaults.setEagerModelSync(true);

                const model = monaco.editor.createModel(modelValue, this.language.id);

                this.editor = monaco.editor.create(this.editorSurface, editorOptions);

                this.editor.setModel(model);

                this.editor.onDidChangeModelContent((e) => {
                    if (!e.isFlush) {
                        const value = this.editor?.getValue();
                        this.$emit('update:code', value);
                    }
                });
            }
        },
        resizeEditor() {
            this.editor?.layout();
        },
        updateCode(code: string) {
            this.editor?.setValue(code);
        },
        postInitialize() {
            // postWebMessage({ cmdtype: "module", modcmd: "init_module" })
        }
    },
    computed: {

    },
    watch: {
        language: {
            handler() {
                if (!this.editor) return;
                const model = this.editor.getModel();
                if (model !== null) {
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