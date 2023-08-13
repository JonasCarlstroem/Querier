<template>    
    <v-container fluid class="ma-0">
        <v-row no-gutters style="height: 25px;">
            <v-col cols="1">
                <v-btn icon="mdi-play" density="compact" @click="$emit('invoke')">
                </v-btn>
            </v-col>
            <v-col cols="1">
                <v-btn icon="mdi-pause" density="compact">
                </v-btn>
            </v-col>
            <v-col cols="1">
                <v-btn icon="mdi-stop" density="compact">
                </v-btn>
            </v-col>
        
            <!-- <v-spacer></v-spacer> -->

            <!-- <v-col cols="2">
                <v-text-field
                        readonly
                        single-line
                        variant="plain" 
                        density="compact" >Language</v-text-field>
            </v-col> -->
            <v-col cols="1">
                <select name="language" id="language-select" style="width: 50px;">
                    <option value="">--Select Language--</option>
                    <option v-for="lang of availableLanguages" :key="'id'">
                    </option>
                </select>
                <!-- <v-select
                        ref="lang"
                        density="compact"
                        variant="outlined"
                        single-line
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
        "invoke"
    ],
    setup() {
        const lang = ref<HTMLElement | null>(null);
        return {
            lang
        }
    },
    data() {
        return {
            currentLanguage: this.activeLanguage
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