#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/raylib-6.0/include/raylib.h"

#define BUFFER_SIZE 1024
#define SAMPLERATE 44100
#define SAMPLESIZE 32
#define CHANNELS 1
#define ROOT_NOTE 440.0f
#define NEXT_SEMITONE powf(2.0f, 1.0f/12.0f)

float semitone_to_freq(float semitone) {
    return ROOT_NOTE * powf(NEXT_SEMITONE, semitone);
}

typedef struct {
    float frequency[BUFFER_SIZE];
    int frame_count; // used as active frequency count
} Note;

float note_freq(float semitone) {
    return semitone_to_freq(semitone);
}

void note_init(Note *n) {
    n->frame_count = 0;
}

void note_add(Note *n, float freq) {
    if (n->frame_count < BUFFER_SIZE) {
        n->frequency[n->frame_count++] = freq;
    }
}

float clamp_f(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

void note_update(Note   *n,
                 float   buffer[],
                 size_t  buf_size,
                 size_t *global_frame)
{
    for (size_t i = 0; i < buf_size; ++i) {
        float time = (float)(*global_frame) / SAMPLERATE;
        for (int j = 0; j < n->frame_count; ++j) {
            buffer[i] += sinf(2.0f * PI * n->frequency[j] * time) * 0.2f;
        }
        *global_frame += 1;
    }
}

int main(void) {
    InitWindow(800, 600, "Music Key");
    InitAudioDevice();

    float buffer[BUFFER_SIZE];
    SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
    AudioStream synth = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    PlayAudioStream(synth);

    Note notes = {0};
    note_init(&notes);
    note_add(&notes, note_freq(0)); // A4
    note_add(&notes, note_freq(4)); // C#5
    note_add(&notes, note_freq(7)); // E5

    size_t global_frame = 0;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x121218FF));

        if (IsAudioStreamProcessed(synth)) {
            memset(buffer, 0, sizeof(buffer));
            note_update(&notes, buffer, BUFFER_SIZE, &global_frame);
            for (size_t i = 0; i < BUFFER_SIZE; ++i) {
                buffer[i] = clamp_f(buffer[i], -1.0f, 1.0f);
            }
            UpdateAudioStream(synth, buffer, BUFFER_SIZE);
        }
        EndDrawing();
    }

    UnloadAudioStream(synth);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
