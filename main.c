#include "functions.h"
/*
stdio
stdint
stdlib
wad_types
wad_level
*/

/*
Hello Hello

If you're reading this, you probably decided to ignore the "warning" in the readme file
This code is, basically, just an experiment. I wanted to see how DOOM's WAD file format worked so
I hyper-focused on it for a couple of days and eventually figured out how to "rip" data, or lumps as they're called
from a wad file. This project isn't finished, and to be frank, I don't know what to do
with it (level editor? map ripper?), so I'm keeping it like this (raw, unfinished, ugly)
for the time being in case I ever need a refresher on how to rip lumps from a wad file
again, or in case I ever want to do the same with Quake's .pak files.

- Nan R. 2023
*/

int main(int argc, char** argv) {
    // TODO: Replace const with argv member
    const char* WADpath = "doom1.wad";
    FILE* wad_file = fopen(WADpath, "rb");
    if (!wad_file) {
        printf("Error reading from %s: %s\n", WADpath, strerror(ferror(wad_file)));
        fclose(wad_file);
        return -1;
    }

    // Create WAD data instance
    WAD_t wad_data;
    get_header(&wad_data, wad_file);

    // Lump array allocation is done inside this function now. NOTE: it does not free the array by itself.
    get_lumps(&wad_data, wad_file);

    // Get level from WAD
    level_t level = get_level(&wad_data, wad_file, "E1M1");

    if (level.vertexes == NULL) {
        puts("ERROR. Couldn't find level.\n");
        return -1;
    }

    // CLEANUP
    free(level.vertexes);
    free(level.things);
    free(level.linedefs);
    free(wad_data.lumps);
    fclose(wad_file);
}
