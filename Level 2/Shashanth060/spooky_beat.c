#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SOUNDS 5
#define BUFFER_SIZE 44100 * 2  // Adjust depending on WAV sample size

const char *sound_files[MAX_SOUNDS] = {
    "spooky_sounds/thunder.wav",
    "spooky_sounds/whisper.wav",
    "spooky_sounds/wind.wav",
    "spooky_sounds/organ.wav",
    "spooky_sounds/creak.wav"
};

int transitions[MAX_SOUNDS][MAX_SOUNDS] = {
    {0, 1, 2, 0, 1},  // thunder transitions
    {2, 3, 0, 1, 4},  // whisper transitions
    {1, 0, 4, 2, 3},  // wind transitions
    {4, 1, 3, 0, 2},  // organ transitions
    {3, 2, 1, 4, 0}   // creak transitions
};

// Function to load WAV file into buffer
int load_wav(const char *filename, char *buffer, size_t buffer_size) {
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;

    // Skip the WAV header (44 bytes)
    fseek(file, 44, SEEK_SET);
    fread(buffer, sizeof(char), buffer_size, file);
    fclose(file);
    return 0;
}

int main() {
    srand(time(NULL));

    // Prepare a buffer for combining sounds
    char buffer[BUFFER_SIZE] = {0};
    FILE *output = fopen("spooky_beat.wav", "wb");

    // Write a blank WAV header (to be updated at the end)
    for (int i = 0; i < 44; i++) fputc(0, output);

    // Initialize sequence with a random starting sound
    int current_sound = rand() % MAX_SOUNDS;

    // Generate spooky sequence
    for (int i = 0; i < 10; i++) {  // Adjust length for longer beats
        char sound_buffer[BUFFER_SIZE] = {0};
        load_wav(sound_files[current_sound], sound_buffer, BUFFER_SIZE);
        fwrite(sound_buffer, sizeof(char), BUFFER_SIZE, output);

        // Choose next sound based on Markov transitions
        current_sound = transitions[current_sound][rand() % MAX_SOUNDS];
    }

    // Update WAV header (set correct sizes for file format)
    fseek(output, 4, SEEK_SET);
    int file_size = 44 + BUFFER_SIZE * 10;  // Example file size
    fwrite(&file_size, sizeof(int), 1, output);
    fseek(output, 40, SEEK_SET);
    int data_size = BUFFER_SIZE * 10;
    fwrite(&data_size, sizeof(int), 1, output);

    fclose(output);
    printf("Spooky beat generated: spooky_beat.wav\n");
    return 0;
}
