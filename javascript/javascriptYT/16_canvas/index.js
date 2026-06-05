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

    len() {
        return Math.sqrt(this.x * this.x + this.y * this.y);
    }

    normalize() {
        const n = this.len();
        return new V2(this.x/n, this.y/n);
    }

    dist(that) {
        return this.sub(that).len();
    }
}

function polarV2(mag, dirtory) {
    return new V2(Math.cos(dirtory) * mag, Math.sin(dirtory) * mag);
}

let playerColor = "#f36ba3";
let enermyColor = "#cdd6f4";
let particalColor = enermyColor;

const PLAYER_RADIUS = 30;
const PLAYER_SPEED = 500;
const BULLET_SPEED = 1000;
const BULLET_RADIUS = 30;
const BULLET_LIFETIME = 5.0;
const TUTORIAL_POPUP_SPEED = 1.6;
const ENERMY_SPEED = PLAYER_SPEED/2;
const ENERMY_RADIUS = PLAYER_RADIUS;
const PARTICAL_COUNT = 50.0;
const PARTICAL_RADIUS = 10.0;
const PARTICAL_MAG = BULLET_SPEED;
const PARTICAL_LIFETIME = 1.0;
const ENERMY_SPAWN_COOLDOWN = 2.0;
const ENERMY_SPAWN_DISTANCE = 1600.0;

const directionMap = {
    "KeyW": new V2(0, -1.0),
    "KeyS": new V2(0, 1.0),
    "KeyA": new V2(-1.0, 0),
    "KeyD": new V2(1.0, 0),
}

const TutorialState = Object.freeze({
    "LearningMovemeant" : 0,
    "LearningShooting" : 1,
    "Finish" : 2
});

const TutorialMessages = Object.freeze([
    "WASD to move around.",
    "Left Mouse Click to Shoot",
    ""
]);

class TutorialPopUp {
    constructor(text = "Blank") {
        this.alpha = 0.0;
        this.deltaAlpha = 0.0;
        this.text = text;
        this.onFadedOut = undefined;
        this.onFadedIn = undefined;
    }

    update(deltaTime) {
        this.alpha += this.deltaAlpha * deltaTime;

        if (this.deltaAlpha < 0.0 && this.alpha <= 0.0) {
            this.deltaAlpha = 0.0;
            this.alpha = 0.0;

            if (this.onFadedOut !== undefined) {
                this.onFadedOut();
            }

        } else if (this.deltaAlpha > 0.0 && this.alpha >= 1.0) {
            this.deltaAlpha = 0.0;
            this.alpha = 1.0;

            if (this.onFadedIn !== undefined) {
                this.onFadedIn();
            }
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
        this.deltaAlpha = TUTORIAL_POPUP_SPEED;
    }

    fadeOut() {
        this.deltaAlpha = -TUTORIAL_POPUP_SPEED;
    }
}

class Tutorial {
    constructor() {
        this.state = 0;
        this.popUp = new TutorialPopUp(TutorialMessages[this.state]);
        this.popUp.fadeIn();
        this.popUp.onFadedOut = () => {
            this.popUp.text = TutorialMessages[this.state];
            this.popUp.fadeIn();
        };
    }

    update(deltaTime) {
        this.popUp.update(deltaTime);
    }

    render(context) {
        this.popUp.render(context);
    }

    playerMoved() {
        if (this.state === TutorialState.LearningMovemeant) {
            this.popUp.fadeOut();
            this.state += 1;
        }
    }

    playerShot() {
        if (this.state === TutorialState.LearningShooting) {
            this.popUp.fadeOut();
            this.state += 1;
        }
    }
}

class Partical {
    constructor(pos, velosity, lifetime, radius) {
        this.pos = pos;
        this.velosity = velosity;
        this.lifetime = lifetime;
        this.radius = radius;
    }

    render(context) {
        fillCircle(context, this.pos, this.radius, particalColor);
    }

    update(deltaTime) {
        this.pos = this.pos.add(this.velosity.scale(deltaTime));
        this.lifetime -= deltaTime;
    }
}

function particalBuster(partical, center) {
    const N = Math.random() * PARTICAL_COUNT;
    for (let i = 0; i < N; i++) {
        partical.push(new Partical(
            center,
            polarV2(Math.random() * PARTICAL_MAG, Math.random() * 2 * Math.PI),
            Math.random() * PARTICAL_LIFETIME,
            Math.random() * PARTICAL_RADIUS + 10.0));
    }
}

class Enermy {
    constructor(pos) {
        this.pos = pos;
        this.isDead = false;
    }

    update(deltaTime, followPos) {
        let velosity = followPos.sub(this.pos)
                                .normalize()
                                .scale(ENERMY_SPEED * deltaTime);

        this.pos = this.pos.add(velosity);
    }

    render(context) {
        fillCircle(context, this.pos, ENERMY_RADIUS, enermyColor);
    }
}

class Bullet {
    constructor(pos, velosity) {
        this.pos = pos;
        this.velosity = velosity;
        this.lifetime = BULLET_LIFETIME;
    }

    update(deltaTime) {
        this.pos = this.pos.add(this.velosity.scale(deltaTime));
        this.lifetime -= deltaTime;
    }

    render(context) {
        fillCircle(context, this.pos, BULLET_RADIUS, playerColor);
    }
}

function renderEntities(context, entities) {
    for (let entity of entities) {
        entity.render(context);
    }
}

class Game {
    constructor() {
        this.playerPos = new V2(PLAYER_RADIUS + 10, PLAYER_RADIUS + 10);
        this.mousePos = new V2(0, 0);
        this.pressedKeys = new Set();
        this.tutorial = new Tutorial();
        this.playerLearnedToMove = false;
        this.bullets = [];
        this.enermies = [];
        this.particals = [];
        this.enermySpawnCooldown = 0.0;
    }

    update(deltaTime) {
        let velocity = new V2(0, 0);
        let moved = false;

        for (let key of this.pressedKeys) {
            if (key in directionMap) {
                velocity = velocity.add(directionMap[key].scale(PLAYER_SPEED));
                moved = true;
            }
        }
        if (moved) {
            this.tutorial.playerMoved();
        }

        this.playerPos = this.playerPos.add(velocity.scale(deltaTime));
        this.tutorial.update(deltaTime);

        for (let bullet of this.bullets) {
            for (let enermy of this.enermies) {
                if (!enermy.isDead 
                    && enermy.pos.dist(bullet.pos) <= BULLET_RADIUS + ENERMY_RADIUS) {

                    enermy.isDead = true;
                    bullet.lifetime = 0.0;
                    particalBuster(this.particals, enermy.pos);
                }
            }
        }

        for (let bullet of this.bullets) {
            bullet.update(deltaTime);
        }
        this.bullets = this.bullets.filter((bullet) => bullet.lifetime > 0.0);

        for (let enermy of this.enermies) {
            enermy.update(deltaTime, this.playerPos);
        }
        this.enermies = this.enermies.filter((enermy) => !enermy.isDead);

        for (let partical of this.particals) {
            partical.update(deltaTime, this.playerPos);
        }
        this.particals = this.particals.filter((partical) => partical.lifetime > 0.0);

        if (this.tutorial.state === TutorialState.Finish) {
            this.enermySpawnCooldown -= deltaTime;
            if (this.enermySpawnCooldown <= 0.0) {
                this.spawnEnermy();
                this.enermySpawnCooldown = ENERMY_SPAWN_COOLDOWN;
            }
        }
    }

    render(context) {
        const width  = context.canvas.width;
        const height = context.canvas.height;

        if (this.playerPos.x + PLAYER_RADIUS >= width)  { this.playerPos.x = width - PLAYER_RADIUS - 2; }
        if (this.playerPos.x - PLAYER_RADIUS <= 0)      { this.playerPos.x = PLAYER_RADIUS + 2; }
        if (this.playerPos.y + PLAYER_RADIUS >= height) { this.playerPos.y = height - PLAYER_RADIUS - 2; }
        if (this.playerPos.y - PLAYER_RADIUS <= 0)      { this.playerPos.y = PLAYER_RADIUS + 2; }

        context.clearRect(0, 0, width, height);
        fillCircle(context, this.playerPos, PLAYER_RADIUS, playerColor);

        renderEntities(context, this.bullets);
        renderEntities(context, this.particals);
        renderEntities(context, this.enermies);

        this.tutorial.render(context);
    }

    spawnEnermy() {
        let direction = Math.random() * 2 * Math.PI;
        this.enermies.push(new Enermy(this.playerPos.add(polarV2(ENERMY_SPAWN_DISTANCE, direction))));
    }

    keyDown(event) {
        this.pressedKeys.add(event.code);
    }

    keyUp(event) {
        this.pressedKeys.delete(event.code);
    }

    mouseMove(event) {
    }

    mouseDown(event) {
        this.tutorial.playerShot();
        const mousePos = new V2(event.offsetX, event.offsetY);
        const bulletVel = mousePos.sub(this.playerPos)
                                  .normalize()
                                  .scale(BULLET_SPEED);

        this.bullets.push(new Bullet(this.playerPos, bulletVel));
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
        case "KeyB": playerColor = "#89b4fa";   break;
        case "KeyO": playerColor = "#fab387"; break;
        case "KeyG": playerColor = "#a6e3a1";  break;
        default:;
        }
    });

    document.addEventListener("keyup", (event) => {
        game.keyUp(event);
    });

    document.addEventListener("mousemove", (event) => {
        game.mouseMove(event);
    });

    document.addEventListener("mousedown", (event) => {
        game.mouseDown(event);
    });
})();
