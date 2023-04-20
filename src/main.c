#include "functions.h"

/* 
dependencies:
    stdio
    stdint
    stdlib
*/

int main(int argc, char** argv) {
    /**

    Hello Hello

    If you're reading this, you probably decided to ignore the README.
    This code is, basically, just an experiment. I wanted to see how DOOM's WAD file format worked so
    I hyper-focused on it for a couple of days and eventually figured out how to "rip" lumps
    from a WAD file. This project isn't finished, and to be frank, I don't know what to do
    with it (level editor? map ripper?), so I'm keeping it like this (raw, unfinished, ugly, weak)
    for the time being in case I ever need a refresher on how DOOM works or in case I ever
    want to do the same with Quake's .pak files.

    - Nan

    **/

    if (argc != 2) {
        puts("Error reading WAD file, please supply a valid path.");
        printf("Usage: `%s <filename>`\n", argv[0]);
        return -1;
    }

    FILE* WAD_file = fopen(argv[1], "rb");

    if (!WAD_file) {
        printf("Error reading from %s: %s. Make sure the file exists and is accessible by the program.\n", argv[1], strerror(ferror(WAD_file)));
        printf("Usage: `%s <filename>`\n", argv[0]);
        fclose(WAD_file);
        return -1;
    }

    // Create WAD data instance
    WAD_t WAD_data;
    get_header(&WAD_data, WAD_file);

    // Lump array allocation is done inside this function. NOTE: it does not free the array by itself.
    get_lumps(&WAD_data, WAD_file);

    // Get level from WAD
    level_t level = get_level(&WAD_data, WAD_file, "E1M1");

    if (level.vertexes == NULL) {
        puts("ERROR. Couldn't find level.\n");
        return -1;
    }

    int tot = 0;
    for (int i = 0; i < WAD_data.header.num_lumps; i++) {
        tot += WAD_data.lumps[i].size;
    }
    printf("Total lumps: %d. Total lump size: %d", WAD_data.header.num_lumps, tot);

    // CLEANUP
    free(level.vertexes);
    free(level.things);
    free(level.linedefs);
    free(WAD_data.lumps);
    fclose(WAD_file);
}
