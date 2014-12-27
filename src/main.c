#include <stdlib.h>
#include <stdio.h>
#include "assembler.h"

int main(int argc, char *argv[]) {
	if(argc == 2) {
		char* l = argv[1];
	}
	FILE* fh = fopen("simple_add.asm", "r");
	assembler_main(fh);
	fclose(fh);
	
	return 0;
}