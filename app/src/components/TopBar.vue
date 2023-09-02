<template>    
    <v-container fluid class="pa-0">
        <v-row no-gutters justify="start" class="pa-1">
            <v-col cols="1">
                <v-btn 
                    icon="mdi-play" 
                    density="compact" 
                    @click="$emit('invoke')">
                </v-btn>
            </v-col>
            <v-col cols="1">
                <v-btn 
                    icon="mdi-pause" 
                    density="compact"
                    @click="">
                </v-btn>
            </v-col>
            <v-col cols="1">
                <v-btn 
                    icon="mdi-stop" 
                    density="compact"
                    @click="">
                </v-btn>
            </v-col>
            <v-col cols="1" offset="1">
                <v-btn 
                    icon="mdi-cog" 
                    density="compact" 
                    @click="$emit('config')">
                </v-btn>
            </v-col>
            <v-col cols="2" offset="1">
                <span>{{ moduleName }}</span> <span>{{ _moduleVersion }}</span>
            </v-col>
            <v-col cols="1">
                <span>{{ queryName }}</span>
            </v-col>
            <v-col cols="1" offset="2">
                <v-btn
                    icon="mdi-close"
                    density="compact"
                    @click="$emit('close-query')">
                </v-btn>
            </v-col>
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