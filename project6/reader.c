#include "reader.h"

char *read_file(FILE *file) {
  char line_buff[2];
  char *buff = malloc(sizeof(char));
  buff[0] = '\0';

  while (fgets(line_buff, sizeof(line_buff), file) != NULL) {
    buff = append_string(buff, line_buff);
  }

  return buff;
}

void read_label_symbols(Symbol **head, char *buff) {
  int value = 0;
  bool take = false;
  char *label = malloc(1);
  label[0] = '\0';

  if (!label) {
    printf("Error allocating memory");
    exit(1);
  }

  while (*buff != '\0') {
    if (is_new_line(*buff))
      value++;

    if (!take && is_symbol_begin(*buff)) {
      take = true;
      buff++;
    }

    if (take && (is_symbol_end(*buff))) {
      take = false;
      Symbol *symbol = symbol_create(value, label);
      bool exists = symbol_exists(*head, symbol);
      if (!exists) {
        add_to_symbol_table(head, symbol);
      }

      free(label);
      label = malloc(1);
      label[0] = '\0';
    }

    if (take) {
      char *tmp = malloc(2);
      tmp[0] = *buff;
      tmp[1] = '\0';
      label = append_string(label, tmp);
      free(tmp);
    }
    buff++;
  }
}

void read_variables(Symbol **head, char *buff) {
  int value = 16; // vars begin from 16
  bool take = false;
  char *var = malloc(1);
  var[0] = '\0';

  while (*buff != '\0') {
    if (!take && is_variable_begin(*buff)) {
      take = true;
      buff++;
    }
    if (take && is_variable_end(*buff)) {
      take = false;
      Symbol *symbol = symbol_create(value, var);
      if (!symbol_exists(*head, symbol)) {
        symbol->value = value++;
        add_to_symbol_table(head, symbol);
      }

      free(var);
      var = malloc(1);
      var[0] = '\0';
    }

    if (take) {
      char *tmp = malloc(2);
      tmp[0] = *buff;
      tmp[1] = '\0';
      var = append_string(var, tmp);
      free(tmp);
    }
    buff++;
  }
}
