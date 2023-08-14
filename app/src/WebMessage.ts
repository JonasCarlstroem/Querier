export function postWebMessage(message: { cmd: string, message?: string }) {
    //@ts-ignore
    window.chrome.webview.postMessage(JSON.stringify(message));
}