#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/raylib-6.0/include/raylib.h"

#define ARRAY_SIZE(arr)	(sizeof(arr) / sizeof((arr)[0]))

#define BUFFER_SIZE	1024
#define SAMPLERATE	44100
#define SAMPLESIZE	32
#define CHANNELS	1
#define ROOT_NOTE	440.0f
#define TWO_PI		(2.0f * PI)

#define BPM			120.0f
#define BEAT_SECS	(60.0f / BPM)
#define BAR_BEATS	4
#define BAR_SECS	(BAR_BEATS * BEAT_SECS)
#define BAR_QUANT	32
#define QUANT_SECS	(BAR_SECS / BAR_QUANT)

#define INIT_CAPACITY 16

enum app_state {
    STATE_REPLAY,
    STATE_WAIT_FOR_EOB,
    STATE_RECORD,
};

static const KeyboardKey key_map[] = {
    KEY_Z, KEY_S, KEY_X, KEY_D, KEY_C, KEY_F, KEY_V, KEY_G, KEY_B, KEY_H,
    KEY_N, KEY_J, KEY_M, KEY_K, KEY_COMMA
};

#define NOTE_COUNT ARRAY_SIZE(key_map)

struct event {
    int timestamp;
    bool start;
    int semitone;
};

struct event_list {
    struct event *items;
    size_t count;
    size_t capacity;
};

struct app_ctx {
    enum app_state state;
    struct event_list *events;
    AudioStream synth;
    float beat_time;
    float beat_time_prev;
    int record_bar_amount;
    int quant_prev;
};

static bool notes_replay[NOTE_COUNT];
static bool notes_monitor[NOTE_COUNT];
static float note_freqs[NOTE_COUNT];
static float note_phase_steps[NOTE_COUNT];
static float note_phases[NOTE_COUNT];

static inline float clamp_f(float val, float min, float max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

static void init_note_frequencies(void) {
    float next_semitone = powf(2.0f, 1.0f / 12.0f);
    size_t i;

    for (i = 0; i < NOTE_COUNT; ++i) {
        note_freqs[i] = ROOT_NOTE * powf(next_semitone, (float)i);
        note_phase_steps[i] = (TWO_PI * note_freqs[i]) / (float)SAMPLERATE;
        note_phases[i] = 0.0f;
    }
}

static struct event_list *event_list_alloc(void) {
    struct event_list *el;

    el = malloc(sizeof(*el));
    if (!el) return NULL;

    el->items = malloc(INIT_CAPACITY * sizeof(*el->items));
    if (!el->items) {
        free(el);
        return NULL;
    }

    el->capacity = INIT_CAPACITY;
    el->count = 0;
    return el;
}

static void event_list_free(struct event_list *el) {
    if (!el) return;
    free(el->items);
    free(el);
}

static int event_list_add(struct event_list *el, struct event item) {
    struct event *new_items;
    size_t new_cap;

    if (el->count == el->capacity) {
        new_cap = el->capacity * 2;
        new_items = realloc(el->items, new_cap * sizeof(*new_items));
        if (!new_items)
            return -1;
        el->items = new_items;
        el->capacity = new_cap;
    }

    el->items[el->count++] = item;
    return 0;
}

static void note_synthesize(size_t note_idx,
                            float  amp,
                            float *buffer,
                            size_t size)
{
    float step = note_phase_steps[note_idx];
    float phase = note_phases[note_idx];
    size_t i;

    for (i = 0; i < size; ++i) {
        buffer[i] += sinf(phase) * amp;
        phase += step;
        if (phase >= TWO_PI) phase -= TWO_PI;
    }

    note_phases[note_idx] = phase;
}

static int get_current_bars(const struct app_ctx *ctx) {
    int bars;

    if (ctx->state == STATE_RECORD) {
        bars = (int)(ctx->beat_time / BAR_SECS) + 1;
    } else {
        bars = ctx->record_bar_amount;
    }

    return (bars < 1) ? 1 : bars;
}

static void replay_update_events(struct app_ctx *ctx, int quant) {
    size_t i;

    if (ctx->events->count == 0 || quant == ctx->quant_prev) return;

    for (i = 0; i < ctx->events->count; ++i) {
        struct event *ev = &ctx->events->items[i];

        if (ev->timestamp > ctx->quant_prev && ev->timestamp <= quant) {
            notes_replay[ev->semitone] = ev->start;
        }
    }
    ctx->quant_prev = quant;
}

static void handle_state_replay(struct app_ctx *ctx, float loop_duration) {
    if (ctx->beat_time >= loop_duration) {
        ctx->beat_time = fmodf(ctx->beat_time, loop_duration);
        ctx->quant_prev = -1;
    }

    replay_update_events(ctx, (int)(ctx->beat_time / QUANT_SECS));
}

static void handle_state_wait_eob(struct app_ctx *ctx) {
    size_t i;

    if (fmodf(ctx->beat_time_prev, BAR_SECS) <= fmodf(ctx->beat_time, BAR_SECS)) {
        return;
    }

    ctx->state = STATE_RECORD;
    ctx->beat_time = 0.0f;
    ctx->quant_prev = -1;
    ctx->events->count = 0;
    memset(notes_replay, 0, sizeof(notes_replay));

    for (i = 0; i < NOTE_COUNT; ++i) {
        struct event ev = { .timestamp = 0, .start = true, .semitone = (int)i };

        if (notes_monitor[i]) event_list_add(ctx->events, ev);
    }
}

static void update_state_timeline(struct app_ctx *ctx, float loop_duration) {
    switch (ctx->state) {
    case STATE_REPLAY:
        handle_state_replay(ctx, loop_duration);
    break;
    case STATE_WAIT_FOR_EOB:
        handle_state_wait_eob(ctx);
    break;
    case STATE_RECORD:
        ctx->quant_prev = (int)(ctx->beat_time / QUANT_SECS);
    break;
    }
}

static void handle_space_pressed(struct app_ctx *ctx) {
    if (!IsKeyPressed(KEY_SPACE)) return;

    memset(notes_replay, 0, sizeof(notes_replay));

    if (ctx->state == STATE_REPLAY) {
        ctx->state = STATE_WAIT_FOR_EOB;
        return;
    }

    if (ctx->state == STATE_RECORD) {
        ctx->record_bar_amount = (int)(ctx->beat_time / BAR_SECS) + 1;
        if (ctx->record_bar_amount < 1) ctx->record_bar_amount = 1;
        ctx->beat_time = 0.0f;
        ctx->quant_prev = -1;
    }

    ctx->state = STATE_REPLAY;
}

static void handle_keyboard_input(struct app_ctx *ctx) {
    int quant_record = (int)(ctx->beat_time / QUANT_SECS);
    size_t i;

    for (i = 0; i < NOTE_COUNT; ++i) {
        bool key_down = IsKeyDown(key_map[i]);
        struct event ev;

        if (key_down == notes_monitor[i]) continue;

        notes_monitor[i] = key_down;
        if (ctx->state != STATE_RECORD) continue;

        ev = (struct event){
            .timestamp = quant_record,
            .start = key_down,
            .semitone = (int)i
        };
        event_list_add(ctx->events, ev);
    }
}

static void audio_process(AudioStream synth) {
    float buffer[BUFFER_SIZE];
    float amp;
    int playing = 0;
    size_t i;

    if (!IsAudioStreamProcessed(synth)) return;

    memset(buffer, 0, sizeof(buffer));

    for (i = 0; i < NOTE_COUNT; ++i) {
        if (notes_monitor[i] || notes_replay[i]) playing++;
    }

    if (playing == 0) {
        UpdateAudioStream(synth, buffer, BUFFER_SIZE);
        return;
    }

    amp = 1.0f / (float)playing;
    for (i = 0; i < NOTE_COUNT; ++i) {
        if (notes_monitor[i] || notes_replay[i]) {
            note_synthesize(i, amp, buffer, BUFFER_SIZE);
        }
    }

    for (i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = clamp_f(buffer[i], -1.0f, 1.0f);
    }

    UpdateAudioStream(synth, buffer, BUFFER_SIZE);
}

static void render_status_ring(enum app_state state) {
    Vector2 pos = { (float)GetScreenWidth() - 50.0f, 50.0f };

    switch (state) {
    case STATE_REPLAY:
        DrawRing(pos, 18.0f, 20.0f, 0, 360, 100, YELLOW);
    break;
    case STATE_WAIT_FOR_EOB:
        DrawCircleV(pos, 20.0f, BLUE);
    break;
    case STATE_RECORD:
        DrawCircleV(pos, 20.0f, RED);
    break;
    }
}

static void render_view(const struct app_ctx *ctx, int bars, float total_sec) {
    float quant_len = (float)GetScreenWidth() / (float)(BAR_QUANT * bars);
    float semitone_h = (float)GetScreenHeight() / (float)NOTE_COUNT;
    float beat_len;
    float playhead_x;
    int   total_beats;
    int   b;
    size_t i;

    BeginDrawing();
    ClearBackground(GetColor(0x121218FF));

    render_status_ring(ctx->state);

    for (i = 0; i < ctx->events->count; ++i) {
        struct event *ev = &ctx->events->items[i];
        Vector2 pos = {
            (float)ev->timestamp * quant_len,
            (float)ev->semitone * semitone_h + (semitone_h * 0.5f)
        };
        DrawCircleV(pos, 6.0f, ev->start ? RED : BLUE);
    }

    total_beats = BAR_BEATS * bars;
    beat_len = (float)GetScreenWidth() / (float)total_beats;
    for (b = 1; b < total_beats; ++b) {
        Vector2 p1 = { beat_len * (float)b, 0.0f };
        Vector2 p2 = { beat_len * (float)b, (float)GetScreenHeight() };
        DrawLineV(p1, p2, GRAY);
    }

    playhead_x = (fmodf(ctx->beat_time, total_sec) / total_sec) * (float)GetScreenWidth();
    DrawLineV((Vector2){ playhead_x, 0.0f },
              (Vector2){ playhead_x, (float)GetScreenHeight() },
              WHITE);

    EndDrawing();
}

int main(void) {
    struct app_ctx ctx = {
        .state = STATE_REPLAY,
        .record_bar_amount = 1,
        .quant_prev = -1,
    };
    int bars;
    float total_sec;

    init_note_frequencies();

    ctx.events = event_list_alloc();
    if (!ctx.events) return 1;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(800, 600, "Music Key");
    InitAudioDevice();

    SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
    ctx.synth = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    PlayAudioStream(ctx.synth);

    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    while (!WindowShouldClose()) {
        ctx.beat_time_prev = ctx.beat_time;
        ctx.beat_time += GetFrameTime();

        bars = get_current_bars(&ctx);
        total_sec = BAR_SECS * (float)bars;

        update_state_timeline(&ctx, total_sec);
        handle_space_pressed(&ctx);
        handle_keyboard_input(&ctx);
        audio_process(ctx.synth);
        render_view(&ctx, bars, total_sec);
    }

    UnloadAudioStream(ctx.synth);
    CloseAudioDevice();
    CloseWindow();
    event_list_free(ctx.events);

    return 0;
}
