<template>
    <div>Hello!</div>
    <v-dialog
        :modelValue="showDialog"
        @update:modelValue="$emit('update:modelValue', showDialog)">
        <v-sheet>
            <v-toolbar
                :height="40">
                <v-btn
                    icon
                    dark
                    @click="$emit('update:modelValue', false)">
                    <v-icon>mdi-close</v-icon>
                </v-btn>
                <v-toolbar-title>Settings</v-toolbar-title>
                <v-spacer></v-spacer>
                <v-toolbar-items>
                    <v-btn
                        variant="text"
                        @click="$emit('update:modelValue', false)">
                        Save
                    </v-btn>
                </v-toolbar-items>
            </v-toolbar>
            <v-tabs 
                v-model="tab">
                <v-tab v-for="_tab in tabs" :value="_tab.value">{{ _tab.title }}</v-tab>
            </v-tabs>
            <v-window
                v-model="tab">
                <v-sheet>
                    <v-container>
                        
                    </v-container>
                </v-sheet>
            </v-window>
        </v-sheet>
    </v-dialog>
</template>
<script lang="ts">
export default {
    props: {
        modelValue: Boolean,
        showDialog: Boolean,
        querySettings: {
            type: Object,
            default: { }
        },
        moduleSettings: {
            type: Object,
            default: { }
        }
    },
    emits: [
        'update:modelValue'
    ],
    data() {
        return {
            _showDialog: false,
            tabs: [
                {
                    title: "Query settings",
                    value: "qsett"
                },
                {
                    title: "Module settings",
                    value: "msett"
                }
            ],
            tab: { }
        }
    },
    mounted() {
        this._showDialog = this.showDialog;
        this.tab = this.tabs[0]
    },
    watch: {
        showDialog(old, next) {
            this._showDialog = next;
        }
    },
    computed: {
    }
}
</script>
<style scoped>

</style>