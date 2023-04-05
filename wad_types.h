#include <stdint.h>
#include "wad_level.h"


typedef struct {
    char wad_type[5];
    int num_lumps;
    int offset;
} header_t;

typedef struct {
    int offset;
    int size;
    char name[9];
} lump_t;

typedef struct {
    mapthings_t* things;
    maplinedef_t* linedefs;
    mapsidedef_t* sidedefs;
    mapvertex_t* vertexes;
    mapsegment_t* segments;
    mapsubsector_t* ssectors;
    mapnode_t* nodes;
    mapsector_t* sectors;
    // reject
    // blockmap
} level_t;

typedef struct {
    header_t header;
    lump_t* lumps;
} WAD_t;
