<template>
    <div class="resultSurface">
        <div class="frame">
            Result

            <div class="result" v-if="_hasResult">
                <!-- <span style="white-space: pre">{{ result }}</span> -->

                <div v-for="(value, key) of _items">
                    {{ key }}: {{ value }}
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
        }
    },
    computed: {
        _hasResult() {
            return this.hasResult;
        },

        _items() {
            return this.result ?? null;
        },

        resultObject() {
            if(this.result) {
                try {
                    console.log(this.result);
                    // const obj = JSON.parse(this.result.substring(0, this.result.indexOf("\n")));
                    // console.log(obj);
                    return null;
                }
                catch(exception) {
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
    height: 100%;
    border: 1px solid white;
    padding: 10px;
    resize: vertical;
}
.frame {
    padding: 10px;
    border: 1px solid white;
    height: 100%;
}
.result {
    height: 100%;
}
</style>