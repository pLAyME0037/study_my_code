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

#define BPM         120.0
#define BEAT_SECS   (60.0/BPM)
#define BAR_BEATS   4
#define BAR_SECS    (BAR_BEATS*BEAT_SECS)
#define BAR_QUANT   32
#define QUANT_SECS  (BAR_SECS/BAR_QUANT)

const KeyboardKey KEY_MAP[] = {
    KEY_Z, KEY_S, KEY_X, KEY_D, KEY_C, KEY_F, KEY_V, KEY_G, KEY_B, KEY_H,
    KEY_N, KEY_J, KEY_M, KEY_K, KEY_COMMA
};
#define NEXT_SEMITONE powf(2.0, 1.0/12.0)
#define INIT_CAPACITY 16
#define NOTE_COUNT    ARRAY_LEN(KEY_MAP)

typedef struct {
    int  timestamp;
    bool start;
    int  semitone;
} Event;

typedef struct {
    Event *items;
    size_t count;
    size_t capacity;
} Events;

bool notes_replay[ARRAY_LEN(KEY_MAP)];
bool notes_monitor[ARRAY_LEN(KEY_MAP)];

float clamp_f(float f, float min, float max) {
    if (f < min) return min;
    if (f > max) return max;
    return f;
}

// void clamp(void *ptr, void *low, void *height) {
//     if(ptr < low)    ptr = low;
//     if(ptr > height) ptr = height;
// }

float semitone_to_freq(float semitone) {
    return ROOT_NOTE*powf(NEXT_SEMITONE, semitone);
}

void note_update(int    frame_count,
                 float  frequency,
                 float  amp,
                 float  buffer[],
                 size_t buf_size)
{
    for (size_t i = 0; i < buf_size; ++i) {
        float time = (float)(frame_count + i)/SAMPLERATE;
        buffer[i] += sinf(2.*PI*time*frequency)*amp;
    }
}

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

Event *peek(Events *e) {
    if (e == NULL || e->count == 0) return NULL;
    return &e->items[e->count - 1];
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(800, 600, "Music Key");
    InitAudioDevice();

    float buffer[BUFFER_SIZE];
    SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
    AudioStream synth = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    PlayAudioStream(synth);

    Sound music = LoadSound("./h-beats-clock-beat-effect-402705.mp3");

    enum { REPLAY, WAIT_FOR_EOB, RECORD } state = REPLAY;
    Events *events = event_new();

    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    float beat_time = 0.0f;
    size_t frame_count = 0;
    int record_bar_amount;
    int quant_for_play;

    while (!WindowShouldClose()) {
        float beat_time_prev = beat_time;
        beat_time += GetFrameTime();
        int quant = beat_time/QUANT_SECS;

        if (fmodf(beat_time_prev, BEAT_SECS) > fmodf(beat_time, BEAT_SECS)) {
            // PlaySound(music);
            // UpdateMusicStream(music);
        }
        switch (state) {
        case REPLAY:
            if (events->count > 0 && record_bar_amount > 0) {
                Event *last_event = peek(events);
                record_bar_amount = (last_event->timestamp + BAR_QUANT - 1)/BAR_QUANT;
                if (record_bar_amount < 1) record_bar_amount = 1;
                quant_for_play = quant%(record_bar_amount*BAR_QUANT);
                for (size_t i = 0; i < events->count; ++i) {
                    if (events->items[i].timestamp == quant_for_play) {
                        notes_replay[events->items[i].semitone] = events->items[i].start;
                    }
                }
            }
        break;
        case WAIT_FOR_EOB:
            if (fmodf(beat_time_prev, BAR_SECS) > fmodf(beat_time, BAR_SECS)) {
                state = RECORD;
                quant = 0;
                beat_time = 0.0;
                for (size_t i = 0; i < ARRAY_LEN(notes_monitor); ++i) {
                    if (notes_monitor[i]) {
                        add((Event) {
                            .timestamp = 0,
                            .start = true,
                            .semitone = (int)i,
                        });
                    }
                }
            }
        break;
        case RECORD:;
        }

        if (IsKeyPressed(KEY_SPACE)) {
            switch (state) {
            case REPLAY:
                state = WAIT_FOR_EOB;
                events->count = 0;
            break;
            case RECORD:
                // for (size_t i = 0; i < events->count; ++i) {
                //     Event e = events->items[i];
                //     printf("[ %d, %d, %d ]\n", e.timestamp.timestamp, e.start, e.semitone);
                // }
                record_bar_amount = (quant + BAR_QUANT - 1)/BAR_QUANT;
                if (record_bar_amount < 1) record_bar_amount = 1;
                beat_time = 0.0;
                state = REPLAY;
            break;
            case WAIT_FOR_EOB:
                state = REPLAY;
            break;
            }
        }

        int note_playing = 0;
        for (size_t key = 0; key < ARRAY_LEN(KEY_MAP); ++key) {
            if (IsKeyDown(KEY_MAP[key]) && !notes_monitor[key]) {
                notes_monitor[key] = true;
                if (state == RECORD) {
                    add((Event) {
                        .timestamp = quant,
                        .start = true,
                        .semitone = (int)key,
                    });
                }
            } else if (!IsKeyDown(KEY_MAP[key]) && notes_monitor[key]) {
                notes_monitor[key] = false;
                if (state == RECORD) {
                    add((Event) {
                        .timestamp = quant,
                        .start = false,
                        .semitone = (int)key,
                    });
                }
            }
            // if (notes_monitor[key]) note_playing += 1;
        }

        if (IsAudioStreamProcessed(synth)) {
            memset(buffer, 0, sizeof(buffer));
            note_playing = 0;
            for (size_t key = 0; key < ARRAY_LEN(notes_monitor); ++key) {
                if (notes_monitor[key] || notes_replay[key]) {
                    note_playing += 1;
                }
            }
            if (note_playing > 0) {
                for (size_t key = 0; key < ARRAY_LEN(notes_monitor); ++key) {
                    if (notes_monitor[key] || notes_replay[key]) {
                        note_update(frame_count, semitone_to_freq(key), 1.0/note_playing, buffer, BUFFER_SIZE);
                    }
                }
                for (size_t i = 0; i < ARRAY_LEN(buffer); ++i) {
                    buffer[i] = clamp_f(buffer[i], -1., 1.);
                }
            }
            frame_count += BUFFER_SIZE;
            UpdateAudioStream(synth, buffer, ARRAY_LEN(buffer));
        }
        BeginDrawing();
        ClearBackground(GetColor(0x121218FF));
        Vector2 note_pos = {GetScreenWidth() - 50, 50};
        int radius = 20;
        switch (state) {
        case REPLAY: {
            DrawRing(note_pos, radius*0.9, radius, 0, 360, 100, YELLOW);
        } break;
        case WAIT_FOR_EOB: {
            DrawCircleV(note_pos, radius, BLUE);
        } break;
        case RECORD: {
            DrawCircleV(note_pos, radius, RED);
        } break;
        }

        record_bar_amount = 1;
        if (events->count > 0) {
            Event *last_event = peek(events);
            record_bar_amount = (last_event->timestamp + BAR_QUANT - 1)/BAR_QUANT;
            if (record_bar_amount < 1) record_bar_amount = 1;
        }

        float quant_len = (float)GetScreenWidth()/(BAR_QUANT*record_bar_amount);
        float semitone_height = (float)GetScreenHeight()/ARRAY_LEN(KEY_MAP);
        for (size_t i = 0; i < events->count; ++i) {
            Vector2 note_pos = {
                events->items[i].timestamp*quant_len,
                events->items[i].semitone*semitone_height + (semitone_height/2.0)
            };
            Color color = events->items[i].start ? RED : BLUE;
            DrawCircleV(note_pos, 6, color);
        }

        int total_beat = BAR_BEATS*record_bar_amount;
        float beat_len = (float)GetScreenWidth()/total_beat;
        for (int i = 1; i < total_beat; ++i) {
            DrawLineV((Vector2){beat_len*i, 0},
                      (Vector2){beat_len*i, GetScreenHeight()},
                      GRAY);
        }
        float total_loop_sec = BAR_SECS*record_bar_amount;
        float ph = (fmodf(beat_time, total_loop_sec)/total_loop_sec)*GetScreenWidth();
        DrawLineV((Vector2){ph, 0},
                  (Vector2){ph, GetScreenHeight()},
                  WHITE);

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
