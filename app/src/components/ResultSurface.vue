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
                            <Table :items="_objects[_tab.value]" :key="_tab.value">
                                <template v-slot:main="{ no }">
                                    <div>Hello!</div>
                                </template>
                                <template v-slot:other="{ yes }">
                                    <div>Hi</div>
                                </template>
                            </Table>
                            <!-- <v-table fixed-header>
                                <thead>
                                    <tr>
                                        <th class="text-left" v-for="n of Object.keys(_objects[_tab.value])" :key="n">
                                            {{ n }}
                                        </th>
                                    </tr>
                                </thead>
                                <tbody>
                                    <tr v-for="n of Object.values(_objects[_tab.value])">
                                        <td> {{  n }}</td>
                                    </tr>
                                </tbody>
                            </v-table> -->
                            <!-- <div v-for="(value, key) of _objects">
                                <span style="color: white; word-wrap: break-word; white-space: pre">{{ _tab.title }}: {{ _result[_tab.value] }}</span>
                            </div> -->
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
import Table from "./ux/Table/Table.vue"

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
    components: {
        "Table": Table
    },
    data() {
        return {
            tab: null as any,
            objectTab: null as any,
            tabs: [{key:"Strings", value:"strings"}, {key: "Functions", value: "functions" }, {key: "Objects", value: "objects"}],
            objectTabs: [{key: "", value: ""}],
            headers: [
                {
                    key: "name",
                    title: "Test",
                    align: "start"
                },
                {
                    key: "age",
                    title: "test2",
                    align: "end"
                }
            ],
            items: [
                {
                    name: "Jonas",
                    age: 29
                },
                {
                    name: "Mickis",
                    age: 30
                }
            ]
        }
    },
    methods: {
        log(item: any) {
            console.log(item);
        }
    },
    computed: {
        _headers() {
            const headers = [
                {
                    key: "name",
                    title: "Test",
                    align: "start"
                },
                {
                    key: "age",
                    title: "test2",
                    align: "end"
                }
            ];
            return headers;
        },
        _items() {
            return [
                {
                    name: "Jonas",
                    age: 29
                },
                {
                    name: "Mickis",
                    age: 30
                }
            ]
        },
        _tabs() {
            if(!this.result) return;
            if(!this.result.strings || !this.result.objects) return;

            let tabs = [];
            if(this.result.strings.length > 0) {
                tabs.push({ title: "Output", value: "output" });
            }
            if(this.result.objects.length > 0) {
                for(const key of Object.keys(this.result.objects)) {
                    tabs.push({ title: "Object", value: key})
                }
            }
            if(tabs.length > 0) {
                this.tab = tabs[0].value;
            }
            return tabs;
        },

        _objectTabs() {
            let tabs: any[] = [];
            if(this.result.objects) {
                for(const entry of this.result.objects) {
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
            return this.result ?? null;
        },

        _strings(): string[] {
            return this._result.strings;
        },

        _functions(): string[] {
            return this._result.functions;
        },

        _objects(): any {
            let obj = {};
            const entries = Object.entries(this._result.objects);
            console.log(Object.entries(this._result.objects));
            console.log(Object.keys(this._result.objects));
            console.log(Object.values(this._result.objects));
            let count = 0;
            for(let temp of Object.values(this._result.objects)) {
                Object.assign(obj, {[`${count}`]:temp});
                count++;
            }
            console.log(obj);
            return Object.values(this._result.objects);
        },

        _hasError() {
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
    width: 100%;
    height: 100%;
    display: flex;
    /* margin: 0 auto; */
}

.frame {
    -webkit-box-sizing: border-box;
    box-sizing: border-box !important;
    display: flex;
    flex-direction: column;
    padding: 10px;
    padding-top: 0;
    /* border: 1px solid white; */
    height: 100%;
    width: 100%;
}

.result {
    -webkit-box-sizing: border-box !important;
    box-sizing: border-box !important;
    height: 100%;
    width: 100%;
    display: flex;
    padding: 10px;
    /* border: 1px solid white; */
    overflow: auto;
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
}

.v-window {
    overflow: visible;
}
</style>