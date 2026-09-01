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

float semitone_to_freq(float semitone) {
    return ROOT_NOTE*powf(NEXT_SEMITONE, semitone);
}

typedef struct {
    float frequency[BUFFER_SIZE];
    int frame_count;
} Note;

void note_update(Note   *note,
                 float   buffer[],
                 size_t  buf_size,
                 size_t *global_frame)
{
    for (size_t i = 0; i < buf_size; ++i) {
        float time = (float)(*global_frame)/SAMPLERATE;
        for (int j = 0; j < note->frame_count; ++j) {
            buffer[i] += sinf(2.*PI*time*note->frequency[j])*0.2;
        }
        *global_frame += 1;
    }
}

void note_new(Note *note) {
    note->frame_count = 0;
}

float note(float semitone) {
    return semitone_to_freq(semitone);
}

void note_add(Note *note, float freq) {
    if (note->frame_count < BUFFER_SIZE) {
        note->frequency[note->frame_count++] = freq;
    }
}

float clamp_f(float d, float min, float max) {
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

// void clamp(void *ptr, void *low, void *height) {
//     if(ptr < low)    ptr = low;
//     if(ptr > height) ptr = height;
// }

int main(void) {
    int result = 0;

    InitWindow(800, 600, "Music Key");
    InitAudioDevice();

    float buffer[BUFFER_SIZE];
    SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
    AudioStream synth = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    PlayAudioStream(synth);

    Note notes = {0};

    note_new(&notes);
    note_add(&notes, note(0)); // A4
    note_add(&notes, note(4)); // C#5
    note_add(&notes, note(7)); // E5

    size_t global_frame = 0;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x121218FF));

        if (IsAudioStreamProcessed(synth)) {
            memset(buffer, 0, sizeof(buffer));
            note_update(&notes, buffer, BUFFER_SIZE, &global_frame);
            for (size_t i = 0; i < ARRAY_LEN(buffer); ++i) {
                buffer[i] = clamp_f(buffer[i], -1., 1.);
            }
            UpdateAudioStream(synth, buffer, ARRAY_LEN(buffer));
        }
        EndDrawing();
    }
    return_defer(1);

defer:
    if (result) {
        UnloadAudioStream(synth);
        CloseAudioDevice();
        CloseWindow();
    }

    return 0;
}
