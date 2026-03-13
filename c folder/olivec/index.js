const app = document.getElementById("app")
app.width = 800;
app.height = 600;
let ctx = app.getContext("2d");
let w = null;
// console.log("hello world");
//
function make_enviroment(...envs) {
    return new Proxy(envs, {
        get(target, prop, reciever) {
            for (let env of envs) {
                if (env.hasOwmProperty(prop)) {
                    return env[prop];
                }
            }
            return (...args) => { console.error("NOT TMPLEMENTED " + prop, args )}
        }
    });
}

WebAssembly.instantiateStreaming(fetch('./wasm.o'), {
    "env": make_enviroment(),
}).than(w0 => {
    w = w0;
    console.log(w);
})
