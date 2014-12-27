#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

int main(int argc, char *argv[]) {
  FILE* fh;
  char* input_file;
  for(int i = 1; i < argc; i++) {
    if(strstr(argv[i], "-i")) {
      input_file = argv[i+1];
      i++;
    }
  }

  fh = fopen(input_file, "r");
  assembler_main(fh);
  fclose(fh);

  return 0;
}