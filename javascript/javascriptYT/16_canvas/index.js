class V2 {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }

    add(that) {
        return new V2(this.x + that.x, this.y + that.y);
    }

    scale(scaler) {
        return new V2(this.x*scaler, this.y*scaler);
    }
}

function fillCircle(context, center, radius, color = "green") {
    context.beginPath();
    context.arc(center.x, center.y, radius, 0, 2*Math.PI, false);
    context.fillStyle = color;
    context.fill();
}

(() => {
    const canvas = document.getElementById("render");
    const context = canvas.getContext("2d");
    const width  = window.innerWidth;
    const height = window.innerHeight;
    canvas.width = width;
    canvas.height = height;
    const radius = 60;
    const speed = 300;

    let start;
    // let x = radius + 10;
    // let y = radius + 10;
    // let delta_w = 300;
    // let delta_h = 300;
    let color = "red";
    let pos = new V2(radius + 10, radius + 10);
    let veloscity = new V2(0, 0);

    let directionMap = {
        "KeyW": new V2(0, -speed),
        "KeyS": new V2(0, speed),
        "KeyA": new V2(-speed, 0),
        "KeyD": new V2(speed, 0),
    }

    function step(timestamp) {
        if (start === undefined) {
            start = timestamp;
        }
        const deltaTime= (timestamp - start)*0.001;
        start = timestamp;

        const width  = window.innerWidth;
        const height = window.innerHeight;
        canvas.width = width;
        canvas.height = height;

        pos = pos.add(veloscity.scale(deltaTime));
        if (pos.x + radius >= width) pos.x = pos.x - 2;
        if (pos.x - radius <= 0) pos.x = pos.x + 2;
        if (pos.y + radius >= width) pos.y = pos.y - 2;
        if (pos.y - radius <= 0) pos.y = pos.y + 2;

        // x += delta_w * deltaTime;
        // y += delta_h * deltaTime;

        context.clearRect(0, 0, width, height);
        fillCircle(context, pos, radius, color);

        window.requestAnimationFrame(step);
    }
    window.requestAnimationFrame(step);

    // context.moveTo(0, 0);
    // context.lineTo(width, height);
    // context.stroke();

    let num = 135*3;
    const title = document.getElementById("header");
    title.textContent = num;

    document.addEventListener("keydown", (event) => {
        // console.log(event);
        if (event.code in directionMap) {
            veloscity = directionMap[event.code];
        }

        switch (event.code) {
        case "KeyB": color = "blue";   break;
        case "KeyO": color = "orange"; break;
        case "KeyG": color = "green";  break;
        default: color = "red";
        }
    });

    document.addEventListener("keyup", () => {
        veloscity = new V2(0 ,0);
    });
})();
