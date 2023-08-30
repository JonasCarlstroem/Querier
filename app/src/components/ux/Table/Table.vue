<template>
    <div class="table-wrapper">
        <table>
            <tr>
                <th v-for="(obj, i) in Object.keys(items)">
                    <span>{{ obj }}</span>
                </th>
            </tr>
            <tr>
                <td v-for="(obj, i) in Object.values(items)">
                    <span style="word-wrap: break-word; white-space: pre;">{{ obj }}</span>
                </td>
            </tr>
        </table>
    </div>
</template>

<script lang="ts">
export default {
    name: "Table",
    props: {
        columns: {
            type: Array<Object>,
            default: []
        },
        items: {
            type: Array<Object>,
            default: []
        },
        key: {
            default: ""
        }
    },
    mounted() {
        console.log(this.$slots);
        console.log(Object.keys(this.items));
    },
    methods: {
        log(obj: any, opt?: any) {
            console.log(obj, opt);
        }
    },
    computed: {
        _columns() {
            const keys = Object.keys(this.items);
            return keys.map((key, i) => {
                return {
                    title: key,
                    id: i
                }
            });
        }
    }
}
</script>

<style scoped>

table {
    border-collapse: collapse;
}

tr {
    padding: 10px;
    margin: 10px;
    overflow-y: hidden;
}

th, td {
    padding-left: 10px;
    padding-right: 10px;
    border: 1px solid rgb(68, 68, 68);
}

th {
    background-color: rgb(187, 187, 187);
    color: black;
}
</style>