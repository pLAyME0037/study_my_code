class V2 {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }

    render(context) {
    }

    add(that) {
        return new V2(this.x + that.x, this.y + that.y);
    }

    sub(that) {
        return new V2(this.x - that.x, this.y - that.y);
    }

    scale(scaler) {
        return new V2(this.x*scaler, this.y*scaler);
    }

    length() {
        return Math.sqrt(this.x * this.x, this.y * this.y);
    }
}

class TutorialPopUp {
    constructor(text = "Blank") {
        this.alpha = 0.0;
        this.deltaAlpha = 0.0;
        this.text = text;
    }

    update(deltaTime) {
        this.alpha += this.deltaAlpha * deltaTime;

        if (this.deltaAlpha < 0.0 && this.alpha <= 0.0) {
            this.deltaAlpha = 0.0;
            this.alpha = 0.0;
        } else if (this.deltaAlpha > 0.0 && this.alpha >= 1.0) {
            this.deltaAlpha = 0.0;
            this.alpha = 1.0;
        }
    }

    render(context) {
        const width  = context.canvas.width;
        const height = context.canvas.height;

        context.fillStyle = `rgba(255, 255, 255, ${this.alpha})`;
        // context.font = "2rem CaskaydiaMono Nerd Font Mono";
        context.font = "1rem LexendMega-Regular";
        // context.center = true;
        context.textAlign = "center";
        context.fillText(this.text, width/2, height/2);
    }

    fadeIn() {
        this.deltaAlpha = 1.0;
        this.alpha = 0.0;
    }
     
    fadeOut() {
        this.deltaAlpha = -1.0;
        this.alpha = 1.0;
    }
}

const radius = 30;
const speed = 500;
const directionMap = {
    "KeyW": new V2(0, -1.0),
    "KeyS": new V2(0, 1.0),
    "KeyA": new V2(-1.0, 0),
    "KeyD": new V2(1.0, 0),
}

let color = "red";

class Game {
    constructor() {
        this.pos = new V2(radius + 10, radius + 10);
        this.pressedKeys = new Set();
        this.popUp = new TutorialPopUp("Use WASD to move around.");
        this.popUp.fadeIn();
        this.playerLearnedToMove = false;
    }

    update(deltaTime) {
        let velocity = new V2(0, 0);

        for (let key of this.pressedKeys) {
            if (key in directionMap) {
                velocity = velocity.add(directionMap[key].scale(speed));
            }
        }

        if (!this.playerLearnedToMove && velocity.length() > 0.0) {
            this.playerLearnedToMove = true;
            this.popUp.fadeOut();
        }

        this.pos = this.pos.add(velocity.scale(deltaTime));
        this.popUp.update(deltaTime);
    }

    render(context) {
        const width  = context.canvas.width;
        const height = context.canvas.height;

        if (this.pos.x + radius >= width)  { this.pos.x = width - radius - 2; }
        if (this.pos.x - radius <= 0)      { this.pos.x = radius + 2; }
        if (this.pos.y + radius >= height) { this.pos.y = height - radius - 2; }
        if (this.pos.y - radius <= 0)      { this.pos.y = radius + 2; }

        context.clearRect(0, 0, width, height);
        fillCircle(context, this.pos, radius, color);
        this.popUp.render(context);
    }

    keyDown(event) {
        this.pressedKeys.add(event.code);
    }

    keyUp(event) {
        this.pressedKeys.delete(event.code);
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
    const game = new Game();

    let start;

    function step(timestamp) {
        if (start === undefined) {
            start = timestamp;
        }
        const deltaTime = (timestamp - start)*0.001;
        start = timestamp;

        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;

        game.update(deltaTime);
        game.render(context);

        // x += delta_w * deltaTime;
        // y += delta_h * deltaTime;
        
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
        game.keyDown(event);

        switch (event.code) {
        case "KeyB": color = "blue";   break;
        case "KeyO": color = "orange"; break;
        case "KeyG": color = "green";  break;
        default:;
        }
    });

    document.addEventListener("keyup", (event) => {
        game.keyUp(event);
    });
})();
