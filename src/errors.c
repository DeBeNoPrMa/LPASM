#include "errors.h"

void lpasm_current_line(int current_line) {
  printf("Error: Line %i: ", current_line);
}

void lpasm_error_bad_section(int current_line) {
  lpasm_current_line(current_line);
  printf("Instruction in data section or data in text section\n");
}

void lpasm_error_outside_section(int current_line) {
  lpasm_current_line(current_line);
  printf("Instruction, data or label declared before declaration of section\n");
}

void lpasm_error_not_recognized(int current_line) {
  lpasm_current_line(current_line);
  printf("Symbol not recognized\n");
}
