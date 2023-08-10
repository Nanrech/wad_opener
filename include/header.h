#include <stdio.h>
#include <string.h>


typedef struct {
  char identification[4]; // "IWAD" or "PWAD" NOT null-terminated
  int numlumps;           // Number of lumps in the WAD
  int infotableofs;       // Pointer to the location of the directory
} header_t;


void get_header(header_t*, FILE*);
void print_header(header_t*);
static inline void get_identification(header_t*, FILE*);
static inline void get_numlumps(header_t*, FILE*);
static inline void get_infotableofs(header_t*, FILE*);


void get_header(header_t* header, FILE* wad_file) {
  get_identification(header, wad_file);
  get_numlumps(header, wad_file);
  get_infotableofs(header, wad_file);
}

void print_header(header_t* header) {
  printf("{ identification: \"");
  
  for (int i = 0; i <= 3; i++) {
    putchar(header->identification[i]);
  }

  printf("\", numlumps: %d, infotableofs: %d }\n", header->numlumps, header->infotableofs);
}


static void get_identification(header_t* header, FILE* wad_file) {
  memset(header->identification, '\0', sizeof(header->identification));
  fread(header->identification, 4, 1, wad_file);
}

static void get_numlumps(header_t* header, FILE* wad_file) {
  header->numlumps = 0;
  fread(&(header->numlumps), 4, 1, wad_file);
}

static void get_infotableofs(header_t* header, FILE* wad_file) {
  header->infotableofs = 0;
  fread(&(header->infotableofs), 4, 1, wad_file);
}
