#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/raylib-6.0/include/raylib.h"

#define return_defer(value) do { result = (value); goto defer; } while(0)
#define ARRAY_LEN(s) (sizeof(s)/sizeof(s[0]))

#define BUFFER_SIZE 1024
#define SAMPLERATE 44100
#define SAMPLESIZE 32
#define CHANNELS 1
#define ROOT_NOTE 440.0
#define NEXT_SEMITONE powf(2.0, 1.0/12.0)

const KeyboardKey KEYBOARD[] = {
    KEY_Z, KEY_S, KEY_X, KEY_D, KEY_C, KEY_F, KEY_V, KEY_G, KEY_B, KEY_H, KEY_N,
    KEY_J, KEY_M, KEY_K, KEY_COMMA
};

bool notes[ARRAY_LEN(KEYBOARD)];

float clamp_f(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
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

int main(void) {
    InitWindow(800, 600, "Music Key");
    InitAudioDevice();

    float buffer[BUFFER_SIZE];
    SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
    AudioStream synth = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    PlayAudioStream(synth);

    size_t frame_count = 0;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x121218FF));

        int note_playing = 0;
        for (size_t key = 0; key < ARRAY_LEN(KEYBOARD); ++key) {
            notes[key] = IsKeyDown(KEYBOARD[key]);
            if (notes[key]) note_playing += 1;
        }

        if (IsAudioStreamProcessed(synth)) {
            memset(buffer, 0, sizeof(buffer));
            if (note_playing > 0) {
                for (size_t key = 0; key < ARRAY_LEN(KEYBOARD); ++key) {
                    if (notes[key]) {
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
        EndDrawing();
    }

    UnloadAudioStream(synth);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
