#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

int main(int argc, char *argv[]) {
  FILE *fh_in, *fh_out;
  char* input_file;
  char* output_file;
  for(int i = 1; i < argc; i++) {
    if(strstr(argv[i], "-i")) {
      input_file = argv[i+1];
      i++;
    } else if (strstr(argv[i], "-o")) {
      output_file = argv[i+1];
      i++;
    }
  }

  fh_in = fopen(input_file, "r");
  fh_out = fopen(output_file, "w+");
  assembler_main(fh_in, fh_out);
  fclose(fh_in);
  fclose(fh_out);

  return 0;
}