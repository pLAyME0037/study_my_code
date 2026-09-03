#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/raylib-6.0/include/raylib.h"

#define return_defer(value) do { result = (value); goto defer; } while(0)
#define ARRAY_LEN(s) (sizeof(s)/sizeof(s[0]))

#define BUFFER_SIZE 1024
#define SAMPLERATE  44100
#define SAMPLESIZE  32
#define CHANNELS    1
#define ROOT_NOTE   440.0
#define TWO_PI      (2.0f * PI)

#define BPM        120.0
#define BEAT_SECS  (60.0/BPM)
#define BAR_BEATS  4
#define BAR_SECS   (BAR_BEATS*BEAT_SECS)
#define BAR_QUANT  32
#define QUANT_SECS (BAR_SECS/BAR_QUANT)

#define NEXT_SEMITONE powf(2.0, 1.0/12.0)
#define INIT_CAPACITY 16

const KeyboardKey KEY_MAP[] = {
    KEY_Z, KEY_S, KEY_X, KEY_D, KEY_C, KEY_F, KEY_V, KEY_G, KEY_B, KEY_H, KEY_N,
    KEY_J, KEY_M, KEY_K, KEY_COMMA
};
#define NOTE_COUNT ARRAY_LEN(KEY_MAP)

enum app_state {
    STATE_REPLAY,
    STATE_WAIT_FOR_EOB,
    STATE_RECORD,
};

typedef struct {
    int   timestamp;
    bool  start;
    int   semitone;
} Event;

typedef struct {
    Event *items;
    size_t count;
    size_t capacity;
} Events;

typedef struct {
    enum app_state state;
    Events *events;
    AudioStream synth;
    float beat_time;
    float beat_time_prev;
    int record_bar_amount;
    int quant_prev;
} AppCtx;

static bool notes_replay[ARRAY_LEN(KEY_MAP)];
static bool notes_monitor[ARRAY_LEN(KEY_MAP)];
static float note_phase[ARRAY_LEN(KEY_MAP)];
static float note_phase_step[ARRAY_LEN(KEY_MAP)];
static float note_freq[ARRAY_LEN(KEY_MAP)];

static inline float clamp_f(float f, float min, float max) {
    if (f < min) return min;
    if (f > max) return max;
    return f;
}

static void note_freq_new(void) {
    // return ROOT_NOTE*powf(NEXT_SEMITONE, semitone);
    for (size_t i = 0; i < NOTE_COUNT; ++i) {
        note_freq[i] = ROOT_NOTE * powf(NEXT_SEMITONE, (float)i);
        note_phase_step[i] = (TWO_PI * note_freq[i]) / (float)SAMPLERATE;
        note_phase[i] = 0.0f;
    }
}

static Events *events_new(void) {
    Events *es;

    es = malloc(sizeof(*es));
    if (!es) return NULL;

    es->items = malloc(INIT_CAPACITY * sizeof(*es->items));
    if (!es->items) {
        free(es);
        return NULL;
    }
    es->capacity = INIT_CAPACITY;
    es->count = 0;

    return es;
}

static int events_add(Events *es, Event item) {
    if (es->capacity == es->count) {
        es->capacity *= 2;
        es->items = realloc(es->items, es->capacity * sizeof(*es->items));
        if (!es->items) return -1;
    }
    es->items[es->count++] = item;
    return 0;
}

Event *peek(Events *e) {
    if (e == NULL || e->count == 0) return NULL;
    return &e->items[e->count - 1];
}

void note_synthesize_update(size_t note_idx,
                            float  amp,
                            float  buffer[],
                            size_t buf_size)
{
    float step  = note_phase_step[note_idx];
    float phase = note_phase[note_idx];
    for (size_t i = 0; i < buf_size; ++i) {
        buffer[i] += sinf(phase)*amp;
        phase += step;
        if (phase >= TWO_PI) phase -= TWO_PI;
    }
    note_phase[note_idx] = phase;
}

static void events_free(Events *es) {
    if (!es) return;
    free(es->items);
    free(es);
}

static int get_current_bars(const AppCtx *ctx) {
    int bars;

    if (ctx->state == STATE_RECORD) {
        bars = (int)(ctx->beat_time / BAR_SECS) + 1;
    } else {
        bars = ctx->record_bar_amount;
    }
    return (bars < 1) ? 1 : bars;
}

static void replay_update_events(AppCtx *ctx, int quant) {
    if (ctx->events->count == 0 || quant == ctx->quant_prev) return;
    for (size_t i = 0; i < ctx->events->count; ++i) {
        Event *ev = &ctx->events->items[i];
        if (ev->timestamp > ctx->quant_prev && ev->timestamp <= quant) {
            notes_replay[ev->semitone] = ev->start;
        }
    }
    ctx->quant_prev = quant;
}

static void handle_state_replay(AppCtx *ctx, float loop_duration) {
    if (ctx->beat_time >= loop_duration) {
        ctx->beat_time = fmodf(ctx->beat_time, loop_duration);
        ctx->quant_prev = -1;
    }
    replay_update_events(ctx, (int)(ctx->beat_time / QUANT_SECS));
}

static void handle_state_wait_eob(AppCtx *ctx) {
    if (fmodf(ctx->beat_time_prev, BAR_SECS) <= fmodf(ctx->beat_time, BAR_SECS)) {
        return;
    }

    ctx->state         = STATE_RECORD;
    ctx->beat_time     = 0.f;
    ctx->quant_prev    = -1;
    ctx->events->count = 0;
    memset(notes_replay, 0, sizeof(notes_replay));

    for (size_t i = 0; i < NOTE_COUNT; ++i) {
        if (notes_monitor[i]) {
            events_add(ctx->events, (Event) {
                .timestamp = 0,
                .start     = true,
                .semitone  = (int)i,
            });
        }
    }
}

static void update_state_timeline(AppCtx *ctx, float loop_duration) {
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

static void handle_key_pressed(AppCtx *ctx, int key) {
    if (!IsKeyPressed(key)) return;

    memset(notes_replay, 0, sizeof(notes_replay));

    if (ctx->state == STATE_REPLAY) {
        ctx->state = STATE_WAIT_FOR_EOB;
        return;
    }

    if (ctx->state == STATE_RECORD) {
        ctx->record_bar_amount = (int)(ctx->beat_time / BAR_SECS) + 1;
        if (ctx->record_bar_amount < 1) ctx->record_bar_amount = 1;
        ctx->beat_time  = 0.f;
        ctx->quant_prev = -1;
    }
    ctx->state = STATE_REPLAY;
}

static void handle_keyboard_input(AppCtx *ctx) {
    for (size_t i = 0; i < NOTE_COUNT; ++i) {
        Event ev;
        if (IsKeyDown(KEY_MAP[i]) == notes_monitor[i]) continue;
        notes_monitor[i] = IsKeyDown(KEY_MAP[i]);
        if (ctx->state != STATE_RECORD) continue;
        events_add(ctx->events, (Event) {
            .timestamp = (int)(ctx->beat_time / QUANT_SECS),
            .start     = IsKeyDown(KEY_MAP[i]),
            .semitone  = (int)i,
        });
    }
}

static void audio_process(AudioStream synth) {
    float buffer[BUFFER_SIZE];
    float amp;
    int   playing = 0;
    if (!IsAudioStreamProcessed(synth)) return;

    memset(buffer, 0, sizeof(buffer));

    for (size_t i = 0; i < NOTE_COUNT; ++i) {
        if (notes_monitor[i] || notes_replay[i]) playing++;
    }

    if (playing == 0) {
        UpdateAudioStream(synth, buffer, BUFFER_SIZE);
        return;
    }

    amp = 1.0f / (float)playing;
    for (size_t i = 0; i < NOTE_COUNT; ++i) {
        if (notes_monitor[i] || notes_replay[i]) {
            note_synthesize_update(i, amp, buffer, BUFFER_SIZE);
        }
    }

    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
        buffer[i] = clamp_f(buffer[i], -1.f, 1.f);
    }
    UpdateAudioStream(synth, buffer, BUFFER_SIZE);
}

static void render_status_ring(enum app_state state) {
    Vector2 note_pos = {GetScreenWidth() - 50, 50};
    int radius = 20;
    switch (state) {
    case STATE_REPLAY: {
        DrawRing(note_pos, radius*0.9, radius, 0, 360, 100, YELLOW);
    } break;
    case STATE_WAIT_FOR_EOB: {
        DrawCircleV(note_pos, radius, BLUE);
    } break;
    case STATE_RECORD: {
        DrawCircleV(note_pos, radius, RED);
    } break;
    }
}

static void render_view(AppCtx *ctx, int bars, float total_sec) {
    float semitone_height = (float)GetScreenHeight()/(float)NOTE_COUNT;

    BeginDrawing();
    ClearBackground(GetColor(0x121218FF));

    render_status_ring(ctx->state);

    for (size_t i = 0; i < ctx->events->count; ++i) {
        Event *e = &ctx->events->items[i];
        Vector2 note_pos = {
            e->timestamp*(float)GetScreenWidth()/(float)(BAR_QUANT*bars),
            e->semitone*semitone_height + (semitone_height/2.0)
        };
        Color color = e->start ? RED : BLUE;
        DrawCircleV(note_pos, 6, color);
    }

    int total_beat = BAR_BEATS*bars;
    float beat_len = (float)GetScreenWidth()/total_beat;
    for (int i = 1; i < total_beat; ++i) {
        DrawLineV((Vector2){beat_len*i, 0.f},
                  (Vector2){beat_len*i, GetScreenHeight()},
                  GRAY);
    }
    float total_loop_sec = BAR_SECS*bars;
    float ph = (fmodf(ctx->beat_time, total_loop_sec)/total_loop_sec)*GetScreenWidth();
    DrawLineV((Vector2){ph, 0.f},
              (Vector2){ph, GetScreenHeight()},
              WHITE);

    EndDrawing();
}

int main(void) {
    AppCtx ctx = {
        .state             = STATE_REPLAY,
        .record_bar_amount = 1,
        .quant_prev        = -1,
    };
    int bars = get_current_bars(&ctx);

    note_freq_new();

    ctx.events = events_new();
    if (!ctx.events) return 1;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(800, 600, "Music Key");
    InitAudioDevice();

    SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
    ctx.synth = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    PlayAudioStream(ctx.synth);

    // Sound music = LoadSound("./h-beats-clock-beat-effect-402705.mp3");

    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    while (!WindowShouldClose()) {
        ctx.beat_time_prev = ctx.beat_time;
        ctx.beat_time += GetFrameTime();

        update_state_timeline(&ctx, BAR_SECS * bars);
        handle_key_pressed(&ctx, KEY_SPACE);
        handle_keyboard_input(&ctx);
        audio_process(ctx.synth);
        render_view(&ctx, bars, BAR_SECS * bars);
    }

    UnloadAudioStream(ctx.synth);
    CloseAudioDevice();
    CloseWindow();
    events_free(ctx.events);

    return 0;
}
