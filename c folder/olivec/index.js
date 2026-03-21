const app = document.getElementById("app")
app.width = 800;
app.height = 600;
let ctx = app.getContext("2d");
let w = null;
// console.log("hello world");

function make_enviroment(...envs) {
    return new Proxy(envs, {
        get(target, prop, reciever) {
            for (let env of envs) {
                if (env.hasOwnProperty(prop)) {
                    return env[prop];
                }
            }
            return (...args) => { console.error("NOT TMPLEMENTED " + prop, args )}
        }
    });
}

WebAssembly.instantiateStreaming(fetch("./wasm.wasm"), {
    "env": make_enviroment()
}).then(w0 => {
    w = w0;
    const buffer = w.instance.exports.memory.buffer; 
    const pixels = w.instance.exports.render();
    const image  = new ImageData(
                   new Uint8ClampedArray(buffer, pixels, app.width * app.height*4), app.width);
    ctx.putImageData(image, 0, 0);
})

