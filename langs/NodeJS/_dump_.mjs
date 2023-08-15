Object.prototype.dump = function() {
    switch(typeof(this)) {
        case "object":
            console.log(JSON.stringify(this));
            return this;
        default:
            console.log(this);
            return this;
    }
};

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

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}