#include "functions.h"
/*
stdio
stdint
stdlib
wad_types
wad_level
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

    // CLEANUP
    free(level.vertexes);
    free(level.things);
    free(level.linedefs);
    free(wad_data.lumps);
    fclose(wad_file);
}
