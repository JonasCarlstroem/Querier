function dump(obj) {
    console.log(typeof(obj));
    if(typeof(obj) === "object") {
        console.log(obj);
    }
    else if(typeof(obj) === "function") {
        console.log(obj);
    }
    else {
        console.log(typeof(obj));
    }
}

global.dump = dump;
Object.prototype.dump = function() {
    console.log(this);
};