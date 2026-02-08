#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stdint.h>

typedef struct Symbol {
  char *label;
  int value;
  struct Symbol *next;
} Symbol;

Symbol *symbol_create(int value, char *label);
Symbol *create_symbol_table(void);
bool symbol_exists(Symbol *head, Symbol *symbol);
void print_symbols(Symbol *head);
void add_to_symbol_table(Symbol **head, Symbol *symbol);
void free_symbol_table(Symbol *head);

#endif
