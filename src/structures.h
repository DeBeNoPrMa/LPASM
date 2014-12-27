#ifndef _STRUCTURES_
#define _STRUCTURES_

#include <stdint.h>

// OPCODES
// Defines the OPCODE structure for the OPCODE table
typedef struct OPCODE {
	char mnemonic[5];
	uint8_t opcode;
	uint8_t length;
} Opcode;

// SYMBOLS
// Defines the SYMBOL structures for the SYMBOL table
typedef enum SYMBOL_TYPE {
	Undefined,
	Absolute
} S_type;

typedef struct SYMBOL {
	char name[15];
	int value;
	S_type type;
} Symbol;

typedef struct DATA {
	int value;
} Data;

// INSTRUCTIONS

// Defines an instruction in LittleProc ASM
typedef struct INSTR {
	Opcode *opcode;
	Symbol *symbol;
	int address;
} Instr;

// SECTIONS
typedef enum SECTION_TYPE {
	NONE,
	DATA,
	TEXT
} Section_type;

// Defines a section in LittleProc ASM
typedef struct SECTION {
	Section_type s_type;
	int address;
	int data_size;
	int total_size;
	void *data;
} Section;

#endif