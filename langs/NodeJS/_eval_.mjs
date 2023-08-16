const a = {
   b: 1,
   c: "Hello!"
};
const b = { 
   n: 2,
   q: "asd"
};
const q = {
   p: 3,
   s: "dsa"
};

function init(obj) {
   console.log(JSON.stringify(obj));
} 

init.dump();
"Hello!".dump();
a.dump();
b.dump();
q.dump();