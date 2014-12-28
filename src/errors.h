#ifndef _ERRORS_
#define _ERRORS_

#define ERR_FILENOTFOUND 1

void lpasm_error_bad_section(int current_line);
void lpasm_error_outside_section(int current_line);
void lpasm_error_not_recognized(int current_line);

#endif
