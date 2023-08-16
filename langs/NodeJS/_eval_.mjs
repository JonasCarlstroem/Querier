const a = {
   b: 1,
   c: "Hello!"
};

function init(obj) {
   console.log(JSON.stringify(obj));
} 

init.dump();
sleep(2000);
dump("Hello!");
"Hello!".dump();
a.dump();