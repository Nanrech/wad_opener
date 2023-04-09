#include "functions.h"


void get_header(WAD_t* wad_data, FILE* wad_file) {
    
    // .identification
    fread(wad_data->header.wad_type, 4, 1, wad_file);
    wad_data->header.wad_type[4] = '\0';
    // .numlumps
    fread(&wad_data->header.num_lumps, 4, 1, wad_file);
    // .infotableofs
    fread(&wad_data->header.offset, 4, 1, wad_file);

    printf("{\"wad_type\": \"%s\", ", wad_data->header.wad_type);
    printf("\"numlumps\": %d, ", wad_data->header.num_lumps);
    printf("\"offset\": %d}\n", wad_data->header.offset);
    
    rewind(wad_file);
}

void get_lumps(WAD_t* wad_data, FILE* wad_file) {

    // Store each lump
    wad_data->lumps = (lump_t*)malloc(wad_data->header.num_lumps * sizeof(lump_t));

    // *I hate Windows*
    // This bit of code was meant to help me debug but instead it's become the bane of my existence
    // Windows won't let me write to a file like this, no clue how to fix it
    FILE* out = fopen("C:/Users/Nan/Documents/wad_wad/lump_dump.txt", "w");
    fseek(wad_file, wad_data->header.offset, SEEK_SET);
    for (int i = 0; i < wad_data->header.num_lumps; i++) {
        wad_data->lumps[i].offset = 0;
        wad_data->lumps[i].size = 0;
        wad_data->lumps[i].name[9];

        fread(&wad_data->lumps[i].offset, 4, 1, wad_file);
        fread(&wad_data->lumps[i].size, 4, 1, wad_file);
        fread(wad_data->lumps[i].name, 8, 1, wad_file);

        fprintf(out, "%d: { \"offset\": %d, \"size\": %d, \"name\": \"%s\" }\n", i, wad_data->lumps[i].offset, wad_data->lumps[i].size, wad_data->lumps[i].name);
    }

    fclose(out);
    rewind(wad_file);
}

int get_lump_index_by_name(WAD_t* wad_data, const char* lump_name) {
    // Return -1 in case you want to check if the lump even exists
    for (int i = 0; i < wad_data->header.num_lumps; i++) {
        if (strcmp(wad_data->lumps[i].name, lump_name) == 0) return i;
    }
    return -1;
}

lump_t get_lump_by_index(WAD_t* wad_data, int index) {
    // Lazy function, not error proof
    // TODO: Re-do this properly?
    return wad_data->lumps[index];
}

level_t get_level(WAD_t* wad_data, FILE* wad_file, const char* level_name) {
    int map_index = get_lump_index_by_name(wad_data, level_name);
    level_t map;
    // Don't look too much into this, please.
    map.linedefs = NULL;
    map.nodes = NULL;
    map.sectors = NULL;
    map.segments = NULL;
    map.sidedefs = NULL;
    map.ssectors = NULL;
    map.things = NULL;
    map.vertexes = NULL;

    if (map_index == -1) return map;
    lump_t lump;

    lump = get_lump_by_index(wad_data, map_index + THINGS);
    map_get_things(&map, &lump, wad_file);
    
    lump = get_lump_by_index(wad_data, map_index + LINEDEFS);
    map_get_linedefs(&map, &lump, wad_file);

    // lump = get_lump_by_index(&wad_data, map_index + SIDEDEFS);

    lump = get_lump_by_index(wad_data, map_index + VERTEXES);
    map_get_vertexes(&map, &lump, wad_file);

    // lump = get_lump_by_index(&wad_data, map_index + SEGS);
    // lump = get_lump_by_index(&wad_data, map_index + SSECTORS);
    // lump = get_lump_by_index(&wad_data, map_index + NODES);
    // lump = get_lump_by_index(&wad_data, map_index + SECTORS);
    // lump = get_lump_by_index(&wad_data, map_index + REJECT);
    // lump = get_lump_by_index(&wad_data, map_index + BLOCKMAP);

    return map;
}

// There's very likely a way to combine all these into a single, elegant function
// It's a bit of a monstrosity as it is, but it works
void map_get_things(level_t* level, lump_t* lump, FILE* wad_file) {
    int count = lump->size / sizeof(mapthings_t);
    level->things = (mapthings_t*)malloc(count * sizeof(mapthings_t));

    fseek(wad_file, lump->offset, SEEK_SET);
    for (int i = 0; i < count; i++) {
        level->things[i].x = 0;
        level->things[i].y = 0;
        level->things[i].angle = 0;
        level->things[i].type = 0;
        level->things[i].flags = 0;

        fread(&level->things[i].x, 2, 1, wad_file);
        fread(&level->things[i].y, 2, 1, wad_file);
        fread(&level->things[i].angle, 2, 1, wad_file);
        fread(&level->things[i].type, 2, 1, wad_file);
        fread(&level->things[i].flags, 2, 1, wad_file);
        // printf("(%d, %d) %d deg, type: %X, flags: %X\n", level->things[i].x, level->things[i].y, level->things[i].angle, level->things[i].type, level->things[i].flags);
    }
    rewind(wad_file);
}

void map_get_linedefs(level_t* level, lump_t* lump, FILE* wad_file) {
    int count = lump->size / sizeof(maplinedef_t);
    level->linedefs = (maplinedef_t*)malloc(count * sizeof(maplinedef_t));

    fseek(wad_file, lump->offset, SEEK_SET);
    for (int i = 0; i < count; i++) {
        level->linedefs[i].start_vertex = 0;
        level->linedefs[i].end_vertex = 0;
        level->linedefs[i].flags = 0;
        level->linedefs[i].special_type = 0;
        level->linedefs[i].sector_tag = 0;
        level->linedefs[i].front_sidedef = 0;
        level->linedefs[i].back_sidedef = 0;

        fread(&level->linedefs[i].start_vertex, 2, 1, wad_file);
        fread(&level->linedefs[i].end_vertex, 2, 1, wad_file);
        fread(&level->linedefs[i].flags, 2, 1, wad_file);
        fread(&level->linedefs[i].special_type, 2, 1, wad_file);
        fread(&level->linedefs[i].sector_tag, 2, 1, wad_file);
        fread(&level->linedefs[i].front_sidedef, 2, 1, wad_file);
        fread(&level->linedefs[i].back_sidedef, 2, 1, wad_file);
    }
    rewind(wad_file);
}

void map_get_vertexes(level_t* level, lump_t* lump, FILE* wad_file) {
    int count = lump->size / sizeof(mapvertex_t);
    level->vertexes = (mapvertex_t*)malloc(count * sizeof(mapvertex_t));

    fseek(wad_file, lump->offset, SEEK_SET);
    for (int i = 0; i < count; i++) {
        level->vertexes[i].x = 0;
        level->vertexes[i].y = 0;

        fread(&level->vertexes[i].x, 2, 1, wad_file);
        fread(&level->vertexes[i].y, 2, 1, wad_file);
        // printf("[%d, %d]\n", level->vertexes[i].x, level->vertexes[i].y);
    }
    rewind(wad_file);
}
