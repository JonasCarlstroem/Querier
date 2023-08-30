import './assets/main.css'

import EditorWorker from 'monaco-editor/esm/vs/editor/editor.worker.js?worker';
import JSONWorker from 'monaco-editor/esm/vs/language/json/json.worker.js?worker';
import HTMLWorker from 'monaco-editor/esm/vs/language/html/html.worker.js?worker';
import CSSWorker from 'monaco-editor/esm/vs/language/css/css.worker.js?worker';
import TSWorker from 'monaco-editor/esm/vs/language/typescript/ts.worker.js?worker';

initMonacoEnvironment();

import { createApp } from 'vue'
import App from './App.vue'

import 'material-design-icons-iconfont/dist/material-design-icons.css';
import '@mdi/font/css/materialdesignicons.css';
import 'vuetify/styles';
import { createVuetify } from 'vuetify';
import { VDataTable } from 'vuetify/labs/VDataTable';
import * as components from 'vuetify/components';
import * as directives from 'vuetify/directives';

const vuetify = createVuetify({
    components: {
        ...components,
        VDataTable
    },
    directives
});

createApp(App).use(vuetify).mount('#app');

function initMonacoEnvironment() {
    self.MonacoEnvironment = {
        getWorker: function(workerId: string, label: string) {
            if(label === "json") {
                return new JSONWorker();
            }
            if(label === "css" || label === "scss" || label === "less") {
                return new CSSWorker();
            }
            if(label === "html") {
                return new HTMLWorker();
            }
            if(label === "javascript" || label === "typescript") {
                return new TSWorker();
            }
            return new EditorWorker();
        }
    }
}