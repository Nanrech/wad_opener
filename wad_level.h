#include <stdint.h>


// map level
typedef struct {
    int16_t x;
    int16_t y;
    int16_t angle;
    int16_t type;
    int16_t flags;
} mapthings_t;

typedef struct {
    int16_t start_vertex;
    int16_t end_vertex;
    int16_t flags;
    int16_t special_type;
    int16_t sector_tag;
    int16_t front_sidedef;
    int16_t back_sidedef;
} maplinedef_t;

typedef struct {
    int16_t x;
    int16_t y;
    char name_upper_texture[8];
    char name_lower_texture[8];
    char name_middle_texture[8];
    int16_t sector_facing;
} mapsidedef_t;

typedef struct {
    int16_t x;
    int16_t y;
} mapvertex_t;

typedef struct {
    int16_t start_vertex;
    int16_t end_vertex;
    int16_t angle;
    int16_t linedef;
    int16_t direction;
    int16_t offset;
} mapsegment_t;

typedef struct {
    int16_t segment_count;
    int16_t first_segment;
} mapsubsector_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t dx;
    int16_t dy;
    int16_t r_bbox[4];
    int16_t l_bbox[4];
    int16_t r_child;
    int16_t l_child;
} mapnode_t;

typedef struct {
    int16_t floor_height;
    int16_t ceiling_height;
    char name_floor_texture[8];
    char name_ceiling_texture[8];
    int16_t light;
    int16_t special_type;
    int16_t tag;
} mapsector_t;

// REJECT

// BLOCKMAP

enum LUMP_INDICES {
    MAP = 0,
    THINGS = 1,
    LINEDEFS = 2,
    SIDEDEFS = 3,
    VERTEXES = 4,
    SEGS = 5,
    SSECTORS = 6,
    NODES = 7,
    SECTORS = 8,
    REJECT = 9,
    BLOCKMAP = 10,
};