<template>
    <div class="resultSurface">
        <div class="frame">
            <div class="result" v-if="_hasResult" :style="`width: ${(_hasError ? 50 : 100)}%;`">
                <!-- <span style="white-space: pre">{{ result }}</span> -->
                <v-tabs
                    v-model="tab">
                    <v-tab v-for="_tab in _tabs" :value="_tab.value">{{ _tab.title }}</v-tab>
                </v-tabs>
                <v-window v-model="tab">
                    <template v-if="tab === 'strings' && _strings !== null">
                        <div v-for="value of _strings">
                            <span style="color: white; word-wrap: break-word; white-space: pre;">{{ value }}</span>
                        </div>
                    </template>

                    <template v-if="tab === 'functions' && _functions !== null">
                        <div v-for="value of _functions">
                            <span style="color: white; word-wrap: break-word; white-space: pre;">{{ value }}</span>
                        </div>
                    </template>

                    <template v-if="tab === 'objects' && _objects !== null">
                        <v-tabs 
                            v-model="objectTab">
                            <v-tab v-for="_objTab in _objectTabs" :value="_objTab"> {{ _objTab.constructor.name }} </v-tab>
                        </v-tabs>
                        <v-window v-model="objectTab">
                            {{ objectTab }}
                        </v-window>
                        <div v-for="(value, key) of _objects">
                            <span style="color: white; word-wrap: break-word; white-space: pre">{{ key }}: {{ value }}</span>
                        </div>
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
            let tabs = [];
            if(this.result) {
                for(const entry of Object.keys(this.result)) {
                    if(this.result[entry].length > 0)
                        tabs.push({ title: entry[0].toUpperCase() + entry.substring(1, entry.length), value: entry });
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
    box-sizing: border-box;
    height: 100%;
    resize: vertical;
}

.frame {
    display: flex;
    flex-direction: row;
    padding: 10px;
    border: 1px solid white;
    height: 100%;
}

.result {
    flex: 1;
    height: 100%;
    border: 1px solid white;
    padding: 10px;
    overflow-y: auto;
}

.error {
    flex: 1;
    height: 100%;
    border: 1px solid white;
    padding: 10px;
    overflow-y: auto;
}</style>