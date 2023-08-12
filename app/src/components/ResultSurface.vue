<template>
    <div class="resultSurface">
        <div class="frame">
            Result

            <div class="result" v-if="_hasResult">
                {{ result }}
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
            type: String,
            default: null
        }
    },
    computed: {
        _hasResult() {
            return this.hasResult;
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