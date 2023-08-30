<template>    
    <v-container fluid class="ma-0 pa-0">
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
            <v-col cols="1" offset="1">
                <v-input
                    density="compact"
                    :hide-details="true">hello</v-input>
                <!-- <v-select
                    ref="lang"
                    height="30"
                    variant="outlined"
                    class="h-0"
                    :items="availableLanguages"
                    :item-title="'name'"
                    :item-value="'id'"
                    v-model="currentLanguage"></v-select> -->
            </v-col>
        </v-row>
    </v-container>
</template>

<script lang="ts">
import { ref } from 'vue';

export default {
    props: {
        activeLanguage: {
            type: Object,
            default: { }
        },
        availableLanguages: Array
    },
    emits: [
        "update:activeLanguage",
        "invoke",
        "config"
    ],
    setup() {
        const lang = ref<HTMLElement | null>(null);
        return {
            lang
        }
    },
    data() {
        return {
            currentLanguage: this.activeLanguage,
            dialog: false
        }
    },
    watch: {
        currentLanguage: function(a, b) {
            console.log("Watcher");
            this.$emit('update:activeLanguage', b);
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