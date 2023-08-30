<template>
    <div>Hello!</div>
    <v-dialog
        :modelValue="showDialog"
        @update:modelValue="$emit('update:modelValue', showDialog)">
        <v-sheet>
            <v-toolbar>
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
            <v-card>
                <v-card-title>
                    <span class="text-h5">Settings</span>
                </v-card-title>
                <v-card-text>
                    <v-container>
                        <v-row>
                            <v-col cols="12" sm="6" md="4">
                            </v-col>
                        </v-row>
                    </v-container>
                </v-card-text>
            </v-card>
        </v-sheet>
    </v-dialog>
</template>
<script lang="ts">
export default {
    props: {
        modelValue: Boolean,
        showDialog: Boolean
    },
    emits: [
        'update:modelValue'
    ],
    data() {
        return {
            _showDialog: false
        }
    },
    mounted() {
        this._showDialog = this.showDialog;
    },
    watch: {
        showDialog(old, next) {
            this._showDialog = next;
        }
    }
}
</script>
<style scoped>

</style>