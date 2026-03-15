#ifndef SYMBOL_H
#define SYMBOL_H

#include <stddef.h>

typedef struct Symbol
{
    short value;
    char *name;
} Symbol;

typedef struct SymbolList
{
    Symbol *symbol;
    struct SymbolList *next;
} SymbolList;

extern Symbol *create_symbol(char *name, short value);
extern SymbolList *create_symbol_list(void);
extern Symbol *find_symbol_by_name(SymbolList *symbol_list, char *name);
extern void initialize_default_symbols(SymbolList *symbol_list);
extern void add_symbol(SymbolList *list, Symbol *symbol);
extern void print_symbol_list(SymbolList *list);
extern void free_symbol(Symbol *symbol);
extern void free_symbol_list(SymbolList *list);

#endif
