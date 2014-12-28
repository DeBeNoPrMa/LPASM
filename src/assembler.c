#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "assembler.h"

#define OPCODE_TABLE_SIZE 11

Opcode oc_table[OPCODE_TABLE_SIZE] = {
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

int mem_table[256];

int num_sections;
int num_symbols;

int current_line;

Opcode* last_instruction_found;
S_type last_section_found;

int current_address;
Section* current_section;

int find_symbol(char* symb) {
  for (int i = 0; i < num_symbols; i++) {
    if(strstr(s_table[i].name, symb)) {
      return i;
    }
  }

  return -1;
}

Symbol* add_undefined_symbol(char *name) {
  memcpy(s_table[num_symbols].name, name, strlen(name));
  s_table[num_symbols].value = -1;
  s_table[num_symbols].type = Undefined;

  num_symbols++;

  return &s_table[num_symbols-1];
}


// Adds instruction to current section
Instr* add_instruction() {
  Instr *instruction;

  if(current_section->data_size == current_section->total_size) {
    current_section->total_size *= 2;
    current_section->data = realloc(current_section->data, current_section->total_size);
    printf("Realloc\n");
  }

  instruction = (Instr*)current_section->data;
  instruction = &instruction[current_section->data_size];
  current_section->data_size++;

  return instruction;
}

// Adds data to current section
Data* add_data() {
  Data *data;

  if(current_section->data_size == current_section->total_size) {
    current_section->total_size *= 2;
    current_section->data = realloc(current_section->data, current_section->total_size);
  }

  data = (Data*)current_section->data;
  data = &data[current_section->data_size];
  current_section->data_size++;

  return data;
}

void parse_section(char *line) {
  char* address;

  address = strtok(line, " \n");
  address = strtok(NULL, " \n");

  sec_table[num_sections].s_type = last_section_found;
  sec_table[num_sections].address = strtol(address, NULL, 16);
  sec_table[num_sections].data_size = 0;
  sec_table[num_sections].total_size = INITIAL_SECTION_SIZE;

  if (last_section_found == DATA) {
    sec_table[num_sections].data = (Data*)malloc(sizeof(Data)*INITIAL_SECTION_SIZE);
  } else {
    sec_table[num_sections].data = (Instr*)malloc(sizeof(Instr)*INITIAL_SECTION_SIZE);
  }

  current_address = sec_table[num_sections].address;
  current_section = &sec_table[num_sections];
  num_sections++;
}

void parse_symbol(char *line) {
  char* name;
  int index;

  name = strtok(line, " :\n");
  index = find_symbol(name);
  if(index == -1) {
    memcpy(s_table[num_symbols].name, name, strlen(name));
    s_table[num_symbols].value = current_address;
    s_table[num_symbols].type = Absolute;
  } else {
    s_table[index].value = current_address;
    s_table[index].type = Absolute;
  }

  num_symbols++;
}

void parse_instruction(char *line) {
  char* param;
  int length;

  Instr *instruction;

  instruction = add_instruction();

  instruction->opcode = last_instruction_found;
  length = instruction->opcode->length;

  if(length == 1) {
    param = strtok(line, " \n");
    param = strtok(NULL, " \n");

    if(isalpha(param[0])) { // Param is a label
      int index;
      index = find_symbol(param);
      if(index != -1) {   // We have found an existing symbol entry
        instruction->symbol = &s_table[index];
        instruction->address = instruction->symbol->value;
      } else {      // No entry found, we add one and leave it as undefined
        instruction->symbol = add_undefined_symbol(param);
      }
    } else { // Param is an address
      instruction->address = strtol(param, NULL, 16);
    }
  }

  current_address++;
}

void parse_data(char *line) {
  char* data;
  Data* data_ptr;

  data = strtok(line, " \n");
  data_ptr = add_data();

  data_ptr->value = strtol(data, NULL, 16);

  current_address++;
}

uint8_t is_section_declaration(char *line) {
  if(strstr(line, ".data") != NULL) { 
    last_section_found = DATA;
    return 1;
  }

  if(strstr(line, ".text") != NULL) {
    last_section_found = TEXT;
    return 1;
  }

  last_section_found = NONE;
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

void generate_mem_map() {
  Instr* instructions;
  Data* data;
  int mem_slot;
  int d;
  for(int i = 0; i < num_sections; i++) {
    for(int j = 0; j < sec_table[i].data_size; j++) {
      mem_slot = sec_table[i].address + j;
      if(mem_table[mem_slot] != 0) {
        printf("Data overlap has been detected on address %i", mem_slot);
      }
      if(sec_table[i].s_type == TEXT) {
        instructions = (Instr*)sec_table[i].data;
        d = (instructions[j].opcode->opcode * 0x100) + (instructions[j].symbol == NULL ? instructions[j].address : instructions[j].symbol->value);
      } else {
        data = (Data*)sec_table[i].data;
        d = data[j].value;
      }
      mem_table[mem_slot] = d;
    }
  }
}

void write_output(FILE* fh_out) {
  fprintf(fh_out, "WIDTH=12;\n");
  fprintf(fh_out, "WIDTH=256;\n");
  fprintf(fh_out, "ADDRESS_RADIX=DEC;\n");
  fprintf(fh_out, "DATA_RADIX=HEX;\n");
  fprintf(fh_out, "CONTENT BEGIN\n");

  for(int i = 0; i < 256; i++) {
    if(mem_table[i] == 0 && mem_table[i+1] == 0) {
      int range_start = i;
      int range_finish = i-1;
      while(mem_table[i] == 0) {
        i++;
        range_finish++;
      }
      i--;
      fprintf(fh_out, "  [%i..%i]:%6.3X;\n", range_start, range_finish, 0);
    } else {
      fprintf(fh_out, "  %-8i:%6.3X;\n", i, mem_table[i]);
    }
  }

  fprintf(fh_out, "END;\n");
}

void release_mem() {
  for(int i = 0; i < num_sections; i++) {
    free(sec_table[i].data);
  }
}

uint8_t assembler_main(FILE *fh_in, FILE *fh_out) {
  num_symbols = 0;
  num_sections = 0;
  current_address = 0;
  current_line = 0;

  if (fh_in == NULL) {
    printf("Input file does not exist or can't be opened\n");
    return ERR_FILENOTFOUND;
  }
  char line[100];
  while (fgets(line, 100, fh_in) != NULL){
    current_line++;
    if (!is_empty_line(line)) {
      if(is_section_declaration(line)) {
        parse_section(line);
      } else if (is_symbol_declaration(line)) {
        parse_symbol(line);
      } else if (is_instruction(line)) {
        parse_instruction(line);
      } else {
        parse_data(line);
      }
    }
  }

  generate_mem_map();

  write_output(fh_out);

  release_mem();
  
  return 0;
}