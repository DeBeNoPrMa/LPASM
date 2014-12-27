#ifndef _ASSEMBLER_
#define _ASSEMBLER_

#include "structures.h"
#include "errors.h"

#define INITIAL_SECTION_SIZE 40

uint8_t assembler_main(FILE *fh_in, FILE *fh_out);

#endif