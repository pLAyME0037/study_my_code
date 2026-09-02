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

float clamp_f(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

float semitone_to_freq(float semitone) {
    return ROOT_NOTE * powf(NEXT_SEMITONE, semitone);
}

typedef struct {
    bool  playing;
    float frequency[BUFFER_SIZE];
    int   frame_count;
} Note;

void note_new(Note *note) {
    note->frame_count = 0;
    note->playing = false;
}

float note_freq(float semitone) {
    return semitone_to_freq(semitone);
}

void note_add(Note *note, float freq) {
    if (note->frame_count < BUFFER_SIZE) {
        note->frequency[note->frame_count++] = freq;
    }
}

void note_update(Note   *note,
                 float   amp,
                 float   buffer[],
                 size_t  buf_size,
                 size_t  global_frame)
{
    if (!note->playing) return;
    for (size_t i = 0; i < buf_size; ++i) {
        float time = (float)(global_frame + i) / SAMPLERATE;
        for (int j = 0; j < note->frame_count; ++j) {
            buffer[i] += sinf(2.0f * PI * time * note->frequency[j]) * amp;
        }
    }
}

const KeyboardKey KEYBOARD[] = { KEY_Z, KEY_S, KEY_X };

Note notes[ARRAY_LEN(KEYBOARD)];

int main(void) {
    InitWindow(800, 600, "Music Key");
    InitAudioDevice();

    float buffer[BUFFER_SIZE];
    SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
    AudioStream synth = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    PlayAudioStream(synth);

    for (size_t i = 0; i < ARRAY_LEN(KEYBOARD); ++i) {
        note_new(&notes[i]);
        note_add(&notes[i], note_freq((float)i));
    }

    size_t global_frame = 0;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x121218FF));

        int note_playing = 0;
        for (size_t key = 0; key < ARRAY_LEN(KEYBOARD); ++key) {
            notes[key].playing = IsKeyDown(KEYBOARD[key]);
            if (notes[key].playing) note_playing += 1;
        }

        if (IsAudioStreamProcessed(synth)) {
            memset(buffer, 0, sizeof(buffer));
            if (note_playing > 0) {
                float amp = 1.0f / (float)note_playing;
                for (size_t key = 0; key < ARRAY_LEN(KEYBOARD); ++key) {
                    note_update(&notes[key], amp, buffer, BUFFER_SIZE, global_frame);
                }
                for (size_t i = 0; i < ARRAY_LEN(buffer); ++i) {
                    buffer[i] = clamp_f(buffer[i], -1.0f, 1.0f);
                }
            }
            global_frame += BUFFER_SIZE;
            UpdateAudioStream(synth, buffer, ARRAY_LEN(buffer));
        }
        EndDrawing();
    }

    UnloadAudioStream(synth);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
