export function postWebMessage(message: { cmd: string, message?: string }) {
    //@ts-ignore
    window.chrome.webview.postMessage(JSON.stringify(message));
}

export function split(val: string, delimiter: string) {
    const arr = [];
    let temp = "";
    for(const char of val) {
        if(char === delimiter) {
            arr.push(temp);
            temp = "";
        }
        else {
            temp += char;
        }
    }
    return arr;
}