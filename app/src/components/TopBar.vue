<template>    
    <v-container fluid class="pa-0">
        <v-row no-gutters justify="start" class="pa-1">
            <v-col cols="2">
                <v-btn-group
                    density="compact"
                    class="ma-0 pa-0 h-auto">
                    <v-btn
                        flat
                        stacked
                        size="30"
                        elevation="0"
                        variant="text"
                        @click="$emit('invoke')">
                        <v-icon color="green-darken-3">mdi-play</v-icon>
                    </v-btn>
                    <v-btn
                        flat
                        stacked
                        size="30"
                        elevation="0"
                        variant="text"
                        @click="">
                        <v-icon color="yellow-darken-1">mdi-pause</v-icon>
                    </v-btn>
                    <v-btn
                        flat
                        stacked
                        size="30"
                        elevation="0"
                        variant="text"
                        @click="">
                        <v-icon color="red-darken-3">mdi-stop</v-icon>
                    </v-btn>
                </v-btn-group>
            </v-col>
            <v-col cols="1">
                <v-btn 
                    stacked
                    size="30"
                    elevation="0"
                    variant="text"
                    @click="$emit('config')">
                    <v-icon>mdi-cog</v-icon>
                </v-btn>
            </v-col>
            <v-col cols="2" offset="1">
                <span>{{ moduleName }}</span> <span>{{ _moduleVersion }}</span>
            </v-col>
            <v-col cols="1">
                <span>{{ queryName }}</span>
            </v-col>
            <v-row no-gutters justify="end" class="h-auto">
                <v-col cols="1">
                    <v-btn
                        stacked
                        size="30"
                        elevation="0"
                        variant="text"
                        @click="$emit('close-query')">
                        <v-icon>mdi-close</v-icon>
                    </v-btn>
                </v-col>
            </v-row>
        </v-row>
    </v-container>
</template>

<script lang="ts">
import { ref } from 'vue';

export default {
    props: {
        activeModule: {
            type: Object,
            default: { }
        },
        availableModules: Array,
        moduleName: String,
        moduleVersion: String,
        queryName: String
    },
    emits: [
        "update:activeLanguage",
        "invoke",
        "config",
        "close-query"
    ],
    setup() {
        const lang = ref<HTMLElement | null>(null);
        return {
            lang
        }
    },
    data() {
        return {
            currentLanguage: this.activeModule,
            dialog: false
        }
    },
    watch: {
        currentLanguage: function(a, b) {
            console.log("Watcher");
            this.$emit('update:activeLanguage', b);
        }
    },
    computed: {
        _moduleVersion() {
            return this.moduleVersion?.replace('\r\n', '');
        }
    },
    mounted() {
        if(this.lang !== null) {
            
        }
    }
}
</script>

<style scoped>
.topbar {
    border: 1px solid white;
    line-height: 1.5;
    padding: 5px;
}
</style>