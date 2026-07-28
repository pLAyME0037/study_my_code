const app = document.getElementById("app")
let ctx = app.getContext("2d");
let w = null;

function make_enviroment(...envs) {
    return new Proxy(envs, {
        get(target, prop, reciever) {
            for (let env of envs) {
                if (env.hasOwnProperty(prop)) {
                    return env[prop];
                }
            }
            return (...args) => { console.error("NOT IMPLEMENTED " + prop, args )}
        }
    });
}

WebAssembly.instantiateStreaming(fetch("bin/ddd.wasm"), {
    "env": make_enviroment({
        "atan2f": Math.atan2,
        "cosf": Math.cos,
        "sinf": Math.sin,
    })
}).then(w0 => {
    w = w0;
    app.width = w.instance.exports.get_width();
    app.height = w.instance.exports.get_height();

    let prev = null;
    function first(timestamp) {
        prev = timestamp;
        window.requestAnimationFrame(loop);
    }

    function loop(timestamp) {
        const dt = timestamp - prev;
        prev = timestamp;

        const pixels = w.instance.exports.render(dt*0.001);
        const buffer = w.instance.exports.memory.buffer;
        const image  = new ImageData(
                       new Uint8ClampedArray(buffer, pixels, app.width * app.height * 4), 
                       app.width,
                       app.height);
        ctx.putImageData(image, 0, 0);

        window.requestAnimationFrame(loop);
    }
    window.requestAnimationFrame(first);
})

