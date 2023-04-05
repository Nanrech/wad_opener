#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "wad_types.h"


void get_header(WAD_t*, FILE*);
void get_lumps(WAD_t*, FILE*);

int get_lump_index_by_name(WAD_t*, const char*);
lump_t get_lump_by_index(WAD_t*, int);

level_t get_level(WAD_t*, FILE*, const char*);

void map_get_things(level_t*, lump_t*, FILE*);
void map_get_linedefs(level_t*, lump_t*, FILE*);
void map_get_vertexes(level_t*, lump_t*, FILE*);