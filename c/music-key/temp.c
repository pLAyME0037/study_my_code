#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/raylib-6.0/include/raylib.h"

#define ARRAY_LEN(s) (sizeof(s)/sizeof(s[0]))

#define BUFFER_SIZE 1024
#define SAMPLERATE 44100
#define SAMPLESIZE 32
#define CHANNELS 1
#define ROOT_NOTE 440.0f
#define NEXT_SEMITONE powf(2.0f, 1.0f/12.0f)

#define BPM        120.0f
#define BEAT_SECS  (60.0f/BPM)
#define BAR_BEATS  4
#define BAR_SECS   (BAR_BEATS*BEAT_SECS)
#define BAR_QUANT  32
#define QUANT_SECS (BAR_SECS/BAR_QUANT)

const KeyboardKey KEYBOARD[] = {
    KEY_Z, KEY_S, KEY_X, KEY_D, KEY_C, KEY_F, KEY_V, KEY_G, KEY_B, KEY_H, KEY_N,
    KEY_J, KEY_M, KEY_K, KEY_COMMA
};

bool notes[ARRAY_LEN(KEYBOARD)];

float clamp_f(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

float semitone_to_freq(float semitone) {
    return ROOT_NOTE * powf(NEXT_SEMITONE, semitone);
}

void note_update(size_t frame_count,
                 float  frequency,
                 float  amp,
                 float  buffer[],
                 size_t buf_size)
{
    for (size_t i = 0; i < buf_size; ++i) {
        float time = (float)(frame_count + i) / SAMPLERATE;
        buffer[i] += sinf(2.0f * PI * time * frequency) * amp;
    }
}

typedef struct { int timestamp; } Quant;
typedef struct {
    Quant timestamp;
    bool start;
    int semitone;
} Event;

typedef struct {
    Event *items;
    size_t count;
    size_t capacity;
} Events;

static Events *eve = NULL;

Events *event_new(void) {
    eve = malloc(sizeof(*eve));
    eve->capacity = 16;
    eve->items = malloc(eve->capacity * sizeof(Event));
    eve->count = 0;
    return eve;
}

void add(Event item) {
    if (eve->capacity == eve->count) {
        eve->capacity *= 2;
        eve->items = realloc(eve->items, eve->capacity * sizeof(Event));
    }
    eve->items[eve->count++] = item;
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(800, 600, "Music Key");
    InitAudioDevice();

    float buffer[BUFFER_SIZE];
    SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
    AudioStream synth = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    PlayAudioStream(synth);

    Sound music = LoadSound("./freesound_community-startup-87026.mp3");

    enum { REPLAY, WAIT_FOR_EOB, RECORD } state = REPLAY;
    Events *events = event_new();

    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    float beat_time = 0.0f;
    size_t frame_count = 0;

    while (!WindowShouldClose()) {
        float beat_time_prev = beat_time;
        beat_time += GetFrameTime();

        if (fmodf(beat_time_prev, BEAT_SECS) > fmodf(beat_time, BEAT_SECS)) {
            PlaySound(music);
        }

        if (state == WAIT_FOR_EOB && fmodf(beat_time_prev, BAR_SECS) > fmodf(beat_time, BAR_SECS)) {
            state = RECORD;
            beat_time = 0.0f;
            events->count = 0;
        }

        int quant = (int)(beat_time / QUANT_SECS);

        if (IsKeyPressed(KEY_SPACE)) {
            switch (state) {
            case REPLAY:
                state = WAIT_FOR_EOB;
                break;
            case RECORD:
                for (size_t i = 0; i < events->count; ++i) {
                    Event e = events->items[i];
                    printf("[ %d, %d, %d ]\n", e.timestamp.timestamp, e.start, e.semitone);
                }
                /* fall through */
            case WAIT_FOR_EOB:
                state = REPLAY;
                break;
            }
        }

        int note_playing = 0;
        for (size_t key = 0; key < ARRAY_LEN(KEYBOARD); ++key) {
            if (IsKeyDown(KEYBOARD[key]) && !notes[key]) {
                notes[key] = true;
                if (state == RECORD) {
                    add((Event){
                        .timestamp = { .timestamp = quant },
                        .start = true,
                        .semitone = (int)key,
                    });
                }
            } else if (!IsKeyDown(KEYBOARD[key]) && notes[key]) {
                notes[key] = false;
                if (state == RECORD) {
                    add((Event){
                        .timestamp = { .timestamp = quant },
                        .start = false,
                        .semitone = (int)key,
                    });
                }
            }
            if (notes[key]) note_playing += 1;
        }

        if (IsAudioStreamProcessed(synth)) {
            memset(buffer, 0, sizeof(buffer));
            if (note_playing > 0) {
                for (size_t key = 0; key < ARRAY_LEN(KEYBOARD); ++key) {
                    if (notes[key]) {
                        note_update(frame_count, semitone_to_freq((float)key), 1.0f / note_playing, buffer, BUFFER_SIZE);
                    }
                }
                for (size_t i = 0; i < ARRAY_LEN(buffer); ++i) {
                    buffer[i] = clamp_f(buffer[i], -1.0f, 1.0f);
                }
            }
            frame_count += BUFFER_SIZE;
            UpdateAudioStream(synth, buffer, ARRAY_LEN(buffer));
        }

        BeginDrawing();
        ClearBackground(GetColor(0x121218FF));
        Vector2 center = { (float)GetScreenWidth() - 50.0f, 50.0f };
        int radius = 20;

        switch (state) {
        case REPLAY:
            DrawRing(center, radius * 0.9f, radius, 0, 360, 100, YELLOW);
            break;
        case WAIT_FOR_EOB:
            DrawCircleV(center, radius, BLUE);
            break;
        case RECORD:
            DrawCircleV(center, radius, RED);
            break;
        }

        for (int i = 0; i < BAR_BEATS; ++i) {
            float beat_len = (float)GetScreenWidth() * i / BAR_BEATS;
            DrawLineV((Vector2){ beat_len, 0 }, (Vector2){ beat_len, (float)GetScreenHeight() }, GRAY);
        }

        float x = (fmodf(beat_time, BAR_SECS) / BAR_SECS) * (float)GetScreenWidth();
        DrawLineV((Vector2){ x, 0 }, (Vector2){ x, (float)GetScreenHeight() }, WHITE);

        EndDrawing();
    }

    free(events->items);
    free(events);
    UnloadSound(music);
    UnloadAudioStream(synth);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
