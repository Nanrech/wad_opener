#include "functions.h"


void get_header(WAD_t* WAD_data, FILE* WAD_file) {
    
    // .identification
    fread(WAD_data->header.WAD_type, 4, 1, WAD_file);
    WAD_data->header.WAD_type[4] = '\0';
    // .numlumps
    fread(&WAD_data->header.num_lumps, 4, 1, WAD_file);
    // .infotableofs
    fread(&WAD_data->header.offset, 4, 1, WAD_file);

    if ((strcmp(WAD_data->header.WAD_type, "IWAD") != 0) && (strcmp(WAD_data->header.WAD_type, "PWAD") != 0)) {
        puts("Error reading WAD. File is either malformed or corrupt. Aborting.");
        exit(-1);
    }

    printf("{\"WAD_type\": \"%s\", ", WAD_data->header.WAD_type);
    printf("\"numlumps\": %d, ", WAD_data->header.num_lumps);
    printf("\"offset\": %d}\n", WAD_data->header.offset);
    
    rewind(WAD_file);
}

void get_lumps(WAD_t* WAD_data, FILE* WAD_file) {

    // Store each lump
    WAD_data->lumps = (lump_t*)malloc(WAD_data->header.num_lumps * sizeof(lump_t));

    // *I hate Windows*
    // This bit of code was meant to help me debug but instead it's become the bane of my existence
    // Windows won't let me write to a file like this, no clue how to fix it
    FILE* out = fopen("C:/Users/Nan/Documents/WAD_WAD/lump_dump.txt", "w");
    fseek(WAD_file, WAD_data->header.offset, SEEK_SET);
    for (int i = 0; i < WAD_data->header.num_lumps; i++) {
        WAD_data->lumps[i].offset = 0;
        WAD_data->lumps[i].size = 0;
        WAD_data->lumps[i].name[9];

        fread(&WAD_data->lumps[i].offset, 4, 1, WAD_file);
        fread(&WAD_data->lumps[i].size, 4, 1, WAD_file);
        fread(WAD_data->lumps[i].name, 8, 1, WAD_file);

        fprintf(out, "%d: { \"offset\": %d, \"size\": %d, \"name\": \"%s\" }\n", i, WAD_data->lumps[i].offset, WAD_data->lumps[i].size, WAD_data->lumps[i].name);
    }

    fclose(out);
    rewind(WAD_file);
}

int get_lump_index_by_name(WAD_t* WAD_data, const char* lump_name) {
    // Return -1 in case you want to check if the lump even exists
    for (int i = 0; i < WAD_data->header.num_lumps; i++) {
        if (strcmp(WAD_data->lumps[i].name, lump_name) == 0) return i;
    }
    return -1;
}

lump_t get_lump_by_index(WAD_t* WAD_data, int index) {
    // Lazy function, not error proof
    // TODO: Re-do this properly?
    return WAD_data->lumps[index];
}

level_t get_level(WAD_t* WAD_data, FILE* WAD_file, const char* level_name) {
    int map_index = get_lump_index_by_name(WAD_data, level_name);
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

    lump = get_lump_by_index(WAD_data, map_index + THINGS);
    map_get_things(&map, &lump, WAD_file);
    
    lump = get_lump_by_index(WAD_data, map_index + LINEDEFS);
    map_get_linedefs(&map, &lump, WAD_file);

    // lump = get_lump_by_index(&WAD_data, map_index + SIDEDEFS);

    lump = get_lump_by_index(WAD_data, map_index + VERTEXES);
    map_get_vertexes(&map, &lump, WAD_file);

    // lump = get_lump_by_index(&WAD_data, map_index + SEGS);
    // lump = get_lump_by_index(&WAD_data, map_index + SSECTORS);
    // lump = get_lump_by_index(&WAD_data, map_index + NODES);
    // lump = get_lump_by_index(&WAD_data, map_index + SECTORS);
    // lump = get_lump_by_index(&WAD_data, map_index + REJECT);
    // lump = get_lump_by_index(&WAD_data, map_index + BLOCKMAP);

    return map;
}

// There's very likely a way to combine all these into a single, elegant function
// It's a bit of a monstrosity as it is, but it works
void map_get_things(level_t* level, lump_t* lump, FILE* WAD_file) {
    int count = lump->size / sizeof(mapthings_t);
    level->things = (mapthings_t*)malloc(count * sizeof(mapthings_t));

    fseek(WAD_file, lump->offset, SEEK_SET);
    for (int i = 0; i < count; i++) {
        level->things[i].x = 0;
        level->things[i].y = 0;
        level->things[i].angle = 0;
        level->things[i].type = 0;
        level->things[i].flags = 0;

        fread(&level->things[i].x, 2, 1, WAD_file);
        fread(&level->things[i].y, 2, 1, WAD_file);
        fread(&level->things[i].angle, 2, 1, WAD_file);
        fread(&level->things[i].type, 2, 1, WAD_file);
        fread(&level->things[i].flags, 2, 1, WAD_file);
        // printf("(%d, %d) %d deg, type: %X, flags: %X\n", level->things[i].x, level->things[i].y, level->things[i].angle, level->things[i].type, level->things[i].flags);
    }
    rewind(WAD_file);
}

void map_get_linedefs(level_t* level, lump_t* lump, FILE* WAD_file) {
    int count = lump->size / sizeof(maplinedef_t);
    level->linedefs = (maplinedef_t*)malloc(count * sizeof(maplinedef_t));

    fseek(WAD_file, lump->offset, SEEK_SET);
    for (int i = 0; i < count; i++) {
        level->linedefs[i].start_vertex = 0;
        level->linedefs[i].end_vertex = 0;
        level->linedefs[i].flags = 0;
        level->linedefs[i].special_type = 0;
        level->linedefs[i].sector_tag = 0;
        level->linedefs[i].front_sidedef = 0;
        level->linedefs[i].back_sidedef = 0;

        fread(&level->linedefs[i].start_vertex, 2, 1, WAD_file);
        fread(&level->linedefs[i].end_vertex, 2, 1, WAD_file);
        fread(&level->linedefs[i].flags, 2, 1, WAD_file);
        fread(&level->linedefs[i].special_type, 2, 1, WAD_file);
        fread(&level->linedefs[i].sector_tag, 2, 1, WAD_file);
        fread(&level->linedefs[i].front_sidedef, 2, 1, WAD_file);
        fread(&level->linedefs[i].back_sidedef, 2, 1, WAD_file);
    }
    rewind(WAD_file);
}

void map_get_vertexes(level_t* level, lump_t* lump, FILE* WAD_file) {
    int count = lump->size / sizeof(mapvertex_t);
    level->vertexes = (mapvertex_t*)malloc(count * sizeof(mapvertex_t));

    fseek(WAD_file, lump->offset, SEEK_SET);
    for (int i = 0; i < count; i++) {
        level->vertexes[i].x = 0;
        level->vertexes[i].y = 0;

        fread(&level->vertexes[i].x, 2, 1, WAD_file);
        fread(&level->vertexes[i].y, 2, 1, WAD_file);
        // printf("[%d, %d]\n", level->vertexes[i].x, level->vertexes[i].y);
    }
    rewind(WAD_file);
}
