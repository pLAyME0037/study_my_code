#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "modules/raylib-6.0/include/raylib.h"

#define return_defer(value) do { result = (value); goto defer; } while(0)
#define ARRAY_LEN(s) sizeof(s)/sizeof(s[0])

#define BUFFER_SIZE 1024
#define SAMPLERATE 44100
#define SAMPLESIZE 32
#define CHANNELS 1
#define ROOT_NOTE 440.0
#define NEXT_SEMITONE pow(2.0, 1.0/12.0)

float note(float semitone) {
    return ROOT_NOTE*pow(NEXT_SEMITONE, semitone);
}

int main(void) {
    int result;

    InitWindow(800, 600, "Music Key");
    InitAudioDevice();

    float buffer[BUFFER_SIZE];
    SetAudioStreamBufferSizeDefault(BUFFER_SIZE);
    AudioStream synth = LoadAudioStream(SAMPLERATE, SAMPLESIZE, CHANNELS);
    PlayAudioStream(synth);

    SetTargetFPS(60);
    float synth_frame_count = 0;
    float synth_freq = 0.0;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x121218FF));

        if (IsKeyDown(KEY_Z)) synth_freq = note(0);
        else if (IsKeyDown(KEY_S)) synth_freq = note(1);
        else if (IsKeyDown(KEY_X)) synth_freq = note(2);
        else if (IsKeyDown(KEY_D)) synth_freq = note(3);
        else if (IsKeyDown(KEY_C)) synth_freq = note(4);
        else synth_freq = 0.0;

        if (IsAudioStreamProcessed(synth)) {
            for (size_t i = 0; i < ARRAY_LEN(buffer); ++i) {
                float time = synth_frame_count/SAMPLERATE;
                buffer[i] = sinf(2*PI*time*synth_freq);
                synth_frame_count += 1;
            }
            UpdateAudioStream(synth, buffer, ARRAY_LEN(buffer));
        }
        EndDrawing();
    }
    return_defer(1);

defer:
    if (result) {
        CloseWindow();
        CloseAudioDevice();
    }

    return 0;
}
