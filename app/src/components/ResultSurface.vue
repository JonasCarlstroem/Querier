<template>
    <div class="resultSurface">
        <div class="frame">
            <v-tabs 
                density="compact"
                :hide-slider="true"
                v-model="tab">
                <v-tab v-for="_tab in _tabs" :value="_tab.value" variant="plain">{{ _tab.title }}</v-tab>
            </v-tabs>
            <div class="result" v-if="_hasResult" :style="`width: ${(_hasError ? 50 : 100)}%;`">
                <v-window v-model="tab">
                    <template v-if="tab === 'output' && _strings !== null">
                        <div v-for="value of _strings">
                            <span style="color: white; word-wrap: break-word; white-space: pre;">{{ value }}</span>
                        </div>
                    </template>

                    <template v-for="_tab in _tabs?.filter(tab => tab.value !== 'output')">
                        <template v-if="_tab.value === tab && _objects !== null">
                            <div v-for="(value, key) of _objects">
                                <span style="color: white; word-wrap: break-word; white-space: pre">{{ _tab.title }}: {{ _result[_tab.value] }}</span>
                            </div>
                        </template>
                    </template>
                </v-window>
            </div>
            <div class="error" v-if="_hasError" :style="`width: ${(_hasResult ? 50 : 100)}%;`">
                <div v-for="(value, key) of _error">
                    <span style="color: red; word-wrap: break-word">{{ key }}: {{ value }}</span>
                </div>
            </div>
        </div>
    </div>
</template>

<script lang="ts">
export default {
    props: {
        hasResult: {
            type: Boolean,
            default: false
        },
        result: {
            type: Object,
            default: null
        },
        hasError: {
            type: Boolean,
            default: false
        },
        error: {
            type: Object,
            default: null
        }
    },
    data() {
        return {
            tab: null as any,
            objectTab: null as any,
            tabs: [{key:"Strings", value:"strings"}, {key: "Functions", value: "functions" }, {key: "Objects", value: "objects"}],
            objectTabs: [{key: "", value: ""}]
        }
    },
    methods: {
        log(item: any) {
            console.log(item);
        }
    },
    computed: {
        _tabs() {
            if(!this.result) return;
            if(!this.result.strings || !this.result.objects) return;

            let tabs = [];
            if(this.result.strings.length > 0) {
                tabs.push({ title: "Output", value: "output" });
            }
            if(this.result.objects.length > 0) {
                for(const key of Object.keys(this.result.objects)) {
                    tabs.push({ title: key, value: key})
                }
            }
            if(tabs.length > 0) {
                console.log(tabs);
                this.tab = tabs[0].value;
            }
            return tabs;
        },

        _objectTabs() {
            let tabs: any[] = [];
            if(this.result.objects) {
                for(const entry of this.result.objects) {
                    console.log("Object tabs", entry);
                    if(this.result.objects[entry]) {
                        // console.log(this.result.objects[entry]);
                        
                    }
                }
            }
            return tabs;
        },

        _hasResult() {
            return this.hasResult;
        },

        _result(): any {
            console.log(this.result);
            return this.result ?? null;
        },

        _strings(): string[] {
            return this._result.strings;
        },

        _functions(): string[] {
            return this._result.functions;
        },

        _objects(): Object[] {
            return this._result.objects;
        },

        _hasError() {
            console.log(this.hasError);
            return this.hasError;
        },

        _error() {
            return this.error ?? null;
        },

        resultObject() {
            if (this.result) {
                try {
                    console.log(this.result);
                    // const obj = JSON.parse(this.result.substring(0, this.result.indexOf("\n")));
                    // console.log(obj);
                    return null;
                }
                catch (exception) {
                    console.log("Error");
                    console.log(exception);
                }
            }
            return null;
        }
    }
}
</script>

<style scoped>
.resultSurface {
    -webkit-box-sizing: border-box !important;
    box-sizing: border-box !important;
    height: 100%;
    resize: vertical;
    margin: 0 auto;
}

.frame {
    -webkit-box-sizing: border-box;
    box-sizing: border-box !important;
    margin: 0 auto;
    padding: 0 auto;
    display: flex;
    flex-direction: column;
    padding: 10px;
    padding-top: 0;
    border: 1px solid white;
    height: 100%;
    overflow: hidden;
}

.result {
    -webkit-box-sizing: border-box !important;
    box-sizing: border-box !important;
    flex: 1;
    /* height: 100%; */
    border: 1px solid white;
    padding: 10px;
    overflow-y: auto;
}

.result-strings {
    box-sizing: border-box !important;
}

.error {
    flex: 1;
    height: 100%;
    border: 1px solid white;
    padding: 10px;
    overflow-y: auto;
}</style>