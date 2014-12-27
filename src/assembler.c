#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "assembler.h"

#define OPCODE_TABLE_SIZE 11

const Opcode oc_table[OPCODE_TABLE_SIZE] = {
	[0]= {.mnemonic = "ldr1", .opcode = 0x0, .length = 1},
	[1]= {.mnemonic = "ldr2", .opcode = 0x1, .length = 1},
	[2]= {.mnemonic = "add",  .opcode = 0x2, .length = 0},
	[3]= {.mnemonic = "str1", .opcode = 0x3, .length = 1},
	[4]= {.mnemonic = "str2", .opcode = 0xA, .length = 1},
	[5]= {.mnemonic = "cmp",  .opcode = 0x4, .length = 0},
	[6]= {.mnemonic = "bre",  .opcode = 0x5, .length = 1},
	[7]= {.mnemonic = "jmp",  .opcode = 0x6, .length = 1},
	[8]= {.mnemonic = "subs", .opcode = 0x7, .length = 0},
	[9]= {.mnemonic = "fi",   .opcode = 0x8, .length = 0},
	[10]={.mnemonic = "nop",  .opcode = 0x9, .length = 0}
};

Symbol s_table[256];
Section sec_table[15];

Opcode* last_instruction_found;


void parse_section(char *line) {
	
}

void parse_symbol(char *line) {

}

void parse_instruction(char *line) {

}

uint8_t is_section_declaration(char *line) {
	if(strstr(line, ".data") != NULL) return 1;
	if(strstr(line, ".text") != NULL) return 1;
	return 0;
}

uint8_t is_symbol_declaration(char *line) {
	if(strstr(line, ":") != NULL) return 1;
	return 0;
}

uint8_t is_instruction(char *line) {
	for (int i = 0; i < OPCODE_TABLE_SIZE; i++) {
		if(strstr(line, oc_table[i].mnemonic) != NULL) {
			last_instruction_found = &oc_table[i];
			return 1;
		}
	}
	return 0;
}

uint8_t is_empty_line(char *line) {
	int i = 0;
	while(line[i] != '\n'){
		if(line[i] != ' ') return 0;
		i++;
	}
	return 1;
}

uint8_t assembler_main(FILE *fh) {
	if (fh == NULL) {
		printf("Input file does not exist or can't be opened\n");
		return ERR_FILENOTFOUND;
	}
	char line[100];
	while (fgets(line, 100, fh) != NULL){
		if (!is_empty_line(line)) {
			if(is_section_declaration(line)) {
				printf("Section: %s", line);
				//parse_section(line);
			} else if (is_symbol_declaration(line)) {
				printf("Symbol: %s", line);
				//parse_symbol(line);
			} else if (is_instruction(line)) {
				printf("Instruction: %s", line);
				//parse_instruction(line);
			} else {
				printf("Data: %s", line);
				//parse_data(line);
			}
		}
	}
	
	return 0;
}