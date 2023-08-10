#include <stdlib.h>
#include "header.h"


typedef struct {
  int filepos;  // Start of the lump's data in the file
  int size;     // Size of the lump in bytes.
  char name[8]; // Null-padded if < 8 chars
} filelump_t;


void get_lumps(filelump_t** lumps, header_t* header, FILE* wadfile) {
  filelump_t* temp_lumps = (filelump_t*)malloc(header->numlumps * sizeof(filelump_t));

  fseek(wadfile, header->infotableofs, SEEK_SET);
  for (int i = 0; i < header->numlumps; i++) {
    temp_lumps[i].filepos = 0;
    temp_lumps[i].size = 0;
    memset(temp_lumps[i].name, '\0', sizeof(temp_lumps->name));

    fread(&temp_lumps[i].filepos, 4, 1, wadfile);
    fread(&temp_lumps[i].size, 4, 1, wadfile);
    fread(temp_lumps[i].name, 8, 1, wadfile);
  }

  *lumps = temp_lumps;
}

void dump_lumps(filelump_t** lumps, header_t* header) {
  FILE* out = fopen("C:\\Users\\Nan\\Documents\\wadripper\\lumps.txt", "w");
  filelump_t* temp_lumps = *lumps;

  for (int i = 0; i < header->numlumps; i++) {
    fprintf(out, "%d: { \"offset\": %d, \"size\": %d, \"name\": \"%s\" }\n", i, temp_lumps[i].filepos, temp_lumps[i].size, temp_lumps[i].name);
  }

  fclose(out);
}
