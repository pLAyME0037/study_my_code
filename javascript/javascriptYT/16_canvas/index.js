'strict mode';

class Color {
    constructor(r, g, b, a) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
    }

    toRgba() {
        return `rgba(${this.r * 255}, ${this.g * 255}, ${this.b * 255}, ${this.a})`;
    }

    withAlpha(a) {
        return new Color(this.r, this.g, this.b, a);
    }

    invert() {
        return new Color(1.0 - this.r, 1.0 - this.g, 1.0 - this.b, this.a);
    }

    greyScale() {
        let x = Math.min(this.r, this.g, this.b);
        return new Color(x, x, x, this.a);
    }

    static hex(hexcolor) {
        let matches = hexcolor.match(/#([0-9a-z]{2})([0-9a-z]{2})([0-9a-z]{2})/i);
        if (matches) {
            let [, r, g, b] = matches;
            return new Color(parseInt(r, 16)/255.0,
                             parseInt(g, 16)/255.0,
                             parseInt(b, 16)/255.0,
                             1.0);
        } else {
            throw `could not parse ${hexcolor} as color`;
        }
    }
}

class V2 {
    constructor(x, y) {
        this.x = x;
        this.y = y;
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

let playerColor = Color.hex("#f36ba3");
let enermyColor = Color.hex("#cdd6f4");
let particalColor = enermyColor;

const PLAYER_MAX_HEALTH     = 100/2;
const HEALTH_BAR_HEIGHT     = 25;
const PLAYER_RADIUS         = 30;
const PLAYER_SPEED          = 500;
const BULLET_SPEED          = 1000;
const BULLET_RADIUS         = 30;
const BULLET_LIFETIME       = 5.0;
const TUTORIAL_POPUP_SPEED  = 1.6;
const ENERMY_SPEED          = PLAYER_SPEED/2;
const ENERMY_RADIUS         = PLAYER_RADIUS;
const PARTICAL_COUNT        = 50.0;
const PARTICAL_RADIUS       = 10.0;
const PARTICAL_MAG          = BULLET_SPEED;
const PARTICAL_LIFETIME     = 1.0;
const ENERMY_SPAWN_COOLDOWN = 2.0;
const ENERMY_SPAWN_DISTANCE = 1600.0;
const ENERMY_HIT_POINT      = PLAYER_MAX_HEALTH/5;

const directionMap = {
    "KeyW": new V2(0, -1.0),
    "KeyS": new V2(0, 1.0),
    "KeyA": new V2(-1.0, 0),
    "KeyD": new V2(1.0, 0),
}

const TutorialState = Object.freeze({
    "LearningMovemeant" : 0,
    "LearningShooting" : 1,
    "Finish" : 2,
});

const TutorialMessages = Object.freeze([
    "WASD to move around",
    "Left Mouse Click to Shoot",
    "",
]);

function greyScaleFilter(color) {
    return color.greyScale();
}

function idFilter(color) {
    return color;
}

let globalFillFilter = idFilter;

function fillCircle(context, center, radius, color) {
    context.beginPath();
    context.arc(center.x, center.y, radius, 0, 2*Math.PI, false);
    context.fillStyle = globalFillFilter(color).toRgba();
    context.fill();
}

function fillRect(context, x, y, w, h, color) {
    context.fillStyle = globalFillFilter(color).toRgba();
    context.fillRect(x, y, w, h);
}

function renderMessage(context, color, size, align, msg) {
    const width  = context.canvas.width;
    const height = context.canvas.height;

    context.fillStyle = `${color}`;
    context.font = `${size}px LexendMega-Regular`;
    context.textAlign = `${align}`;
    context.fillText(msg, width/2, height/2);
}

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
        const color = `rgba(255, 255, 255, ${this.alpha})`;
        renderMessage(context, color, 18, "center", this.text);
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
        const alpha = this.lifetime/PARTICAL_LIFETIME;
        fillCircle(context, this.pos, this.radius, enermyColor.withAlpha(alpha));
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
            Math.random() * PARTICAL_RADIUS + 10.0)
        );
    }
}

function renderEntities(context, entities) {
    for (let entity of entities) {
        entity.render(context);
    }
}

class Player {
    health = PLAYER_MAX_HEALTH;

    constructor(pos) {
        this.pos = pos;
    }

    render(context) {
        fillCircle(context, this.pos, PLAYER_RADIUS, playerColor);
    }

    update(velocity, deltaTime) {
        this.pos = this.pos.add(velocity.scale(deltaTime));
    }

    shootAt(target) {
        const bulletDir = target.sub(this.pos)
                                  .normalize();
        const bulletVel = bulletDir.scale(BULLET_SPEED);
        const bulletPos = this.pos
                              .add(bulletDir
                              .scale(PLAYER_RADIUS + BULLET_RADIUS));

        return new Bullet(bulletPos, bulletVel);
    }

    damage(hitPoint) {
        this.health = Math.max(this.health - hitPoint, 0.0);
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

class Game {
    player = new Player(new V2(PLAYER_RADIUS + 10, PLAYER_RADIUS + 10));
    mousePos = new V2(0, 0);
    pressedKeys = new Set();
    tutorial = new Tutorial();
    bullets = [];
    enermies = [];
    particals = [];
    enermySpawnRate = ENERMY_SPAWN_COOLDOWN;
    enermySpawnCooldown = this.enermySpawnRate;
    paused = false;

    update(deltaTime) {
        let velocity = new V2(0, 0);
        let moved = false;

        if (this.paused) return;

        for (let key of this.pressedKeys) {
            if (key in directionMap) {
                velocity = velocity.add(directionMap[key].scale(PLAYER_SPEED));
                moved = true;
            }
        }
        if (moved) {
            this.tutorial.playerMoved();
        }

        this.player.update(velocity, deltaTime);

        this.tutorial.update(deltaTime);

        for (let enermy of this.enermies) {
            if (!enermy.isDead) {
                for (let bullet of this.bullets) {
                    if (enermy.pos.dist(bullet.pos) <= BULLET_RADIUS + ENERMY_RADIUS) {
                        bullet.lifetime = 0.0;
                        enermy.isDead = true;
                        particalBuster(this.particals, enermy.pos);
                    }
                }
            }

            if (!enermy.isDead) {
                if (enermy.pos.dist(this.player.pos) <= PLAYER_RADIUS + ENERMY_RADIUS) {
                    this.player.damage(ENERMY_HIT_POINT);
                    enermy.isDead = true;
                    particalBuster(this.particals, enermy.pos);
                }
            }
        }

        for (let bullet of this.bullets) {
            bullet.update(deltaTime);
        }
        this.bullets = this.bullets.filter((bullet) => bullet.lifetime > 0.0);

        for (let enermy of this.enermies) {
            enermy.update(deltaTime, this.player.pos);
        }
        this.enermies = this.enermies.filter((enermy) => !enermy.isDead);

        for (let partical of this.particals) {
            partical.update(deltaTime, this.player.pos);
        }
        this.particals = this.particals.filter((partical) => partical.lifetime > 0.0);

        if (this.tutorial.state !== TutorialState.Finish) return;
        this.enermySpawnCooldown -= deltaTime;
        if (this.enermySpawnCooldown <= 0.0) {
            this.spawnEnermy();
            this.enermySpawnCooldown = this.enermySpawnRate;
            this.enermySpawnRate = Math.max(0.01, this.enermySpawnRate - 0.01);
        }
    }

    render(context) {
        const width  = context.canvas.width;
        const height = context.canvas.height;

        if (this.player.pos.x + PLAYER_RADIUS >= width)  { this.player.pos.x = width - PLAYER_RADIUS - 2; }
        if (this.player.pos.x - PLAYER_RADIUS <= 0)      { this.player.pos.x = PLAYER_RADIUS + 2; }
        if (this.player.pos.y + PLAYER_RADIUS >= height) { this.player.pos.y = height - PLAYER_RADIUS - 2; }
        if (this.player.pos.y - PLAYER_RADIUS <= 0)      { this.player.pos.y = PLAYER_RADIUS + 2; }

        context.clearRect(0, 0, width, height);
        this.player.render(context);

        renderEntities(context, this.bullets);
        renderEntities(context, this.particals);
        renderEntities(context, this.enermies);

        if (this.paused) {
            renderMessage(context, "#9ca0b0", 24, "center", "PAUSED (space to play)")
        } else {
            this.tutorial.render(context);
        }

        fillRect(context, 0, 0, width * (this.player.health/PLAYER_MAX_HEALTH), HEALTH_BAR_HEIGHT, playerColor);
    }

    spawnEnermy() {
        let direction = Math.random() * 2 * Math.PI;
        this.enermies.push(new Enermy(this.player.pos.add(polarV2(ENERMY_SPAWN_DISTANCE, direction))));
    }

    toggleGamePause() {
        this.paused = !this.paused;
        if (this.paused) {
            globalFillFilter = greyScaleFilter;
        } else {
            globalFillFilter = idFilter;
        }
    }

    keyDown(event) {
        if (event.code === "Space") {
            this.toggleGamePause();
        }
        this.pressedKeys.add(event.code);
    }

    keyUp(event) {
        this.pressedKeys.delete(event.code);
    }

    mouseMove(event) {
    }

    mouseDown(event) {
        // if (this.paused) return;
        this.tutorial.playerShot();
        const mousePos = new V2(event.offsetX, event.offsetY);
        this.bullets.push(this.player.shootAt(mousePos));
    }
}

const game = new Game();

(() => {
    const canvas = document.getElementById("render");
    const context = canvas.getContext("2d");

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
        case "KeyB": playerColor = Color.hex("#89b4fa"); break;
        case "KeyO": playerColor = Color.hex("#fab387"); break;
        case "KeyG": playerColor = Color.hex("#a6e3a1"); break;
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
