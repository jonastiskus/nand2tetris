#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"
#include "reader.h"
#include "utils.h"
#include "parser.h"

int main(int argc, char const *argv[]) {
  FILE *file;
  file = fopen(argv[1], "r");

  if (file == NULL) {
    printf("%s\n", "The file is not opened.");
    return 0;
  }

  char *buff = read_file(file);

  print_raw(buff);
  printf("-------------------------------\n");
  remove_comments(buff);

  //print_raw(buff);

  Symbol *head = create_symbol_table();
  //print_symbols(head);

  printf("-------------------------------\n");
  read_label_symbols(&head, buff);
  read_variables(&head, buff);
  //print_symbols(head);

  printf("-------------------------------\n");
  parse(buff, head);

  free(buff);
  free_symbol_table(head);

  return 0;
}
