Object.prototype.dump = function() {
    switch(typeof(this)) {
        case "object":
            console.log(JSON.stringify({object: this}));
            return this;
        case "function":
            console.log(JSON.stringify({function: this.toString()}));
            return this;
        default:
            console.log(JSON.stringify({string: this}));
            return this;
    }
};

function dump(obj) {
    console.log(typeof(obj));
    if(typeof(obj) === "object") {
        console.log("obj is an object");
        console.log(obj);
    }
    else if(typeof(obj) === "function") {
        console.log("obj is a function");
        console.log(obj.toString());
    }
    else {
        console.log("obj is neither object or function");
        console.log(typeof(obj));
    }
}

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}