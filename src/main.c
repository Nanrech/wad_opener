#include <stdio.h>
#include "filelump.h"


int main(int argc, char* argv[]) {
  // Entry point
  if (argc != 2) {
    printf("Incorrect call. Usage: `%s <filename>`\n", argv[0]);
    return -1;
  }

  FILE* wad_file = fopen(argv[1], "rb");

  if (ferror(wad_file)) {
    printf("Error reading from %s. Check that the file exists and is accessible by the program.\n", argv[1]);
    printf("Usage: `%s <filename>`\n", argv[0]);
    fclose(wad_file);
    return -1;
  }

  // Load wad into memory

  header_t header;
  filelump_t* lumps = NULL;

  get_header(&header, wad_file);
  get_lumps(&lumps, &header, wad_file);
  print_header(&header);
  // dump_lumps(&lumps, &header);
  fclose(wad_file);

  free(lumps);
  return 0;
}