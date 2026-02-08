#include "parser.h"
#include "utils.h"
#include <ctype.h>

const Comp comp_0[] = {COMP_0_INS_MAP};
const Comp comp_1[] = {COMP_1_INS_MAP};

bool is_a_ins(char *str) { return is_variable_begin(str[0]); }

bool is_c_ins(char *str) {
  return !is_label_begin(str[0]) && !is_variable_begin(str[0]);
}

bool is_symbol(char *str) { return is_a_ins(str) && !isdigit(*(++str)); }

char *parse(char *buff, Symbol *head) {
  char *line = malloc(sizeof(char));
  line[0] = '\0';

  for (int i = 0; buff[i] != '\0'; i++) {
    if (!isspace(buff[i])) {
      char *tmp = malloc(2);
      tmp[0] = buff[i];
      tmp[1] = '\0';
      line = append_string(line, tmp);
      free(tmp);
    } else {
      //print_raw(line);
      printf("line %s\n", line);
      free(line);
      line = malloc(sizeof(char));
      line[0] = '\0';
    }
  }
  return "";
}
