#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol.h"

Symbol *create_symbol(char *name, short value)
{
    Symbol *symbol = malloc(sizeof(Symbol));
    if(!symbol) err(EXIT_FAILURE, "Error allocating Symbol");

    symbol->name = strdup(name);
    symbol->value = value;

    return symbol;
}

SymbolList *create_symbol_list(void)
{
    SymbolList *symbol_list = malloc(sizeof(SymbolList));    
    if(!symbol_list) err(EXIT_FAILURE, "Error allocating SymbolList");

    symbol_list->symbol = NULL;
    symbol_list->next = NULL;

    return symbol_list;
}

void add_symbol(SymbolList *list, Symbol *symbol)
{
   if(!list) err(EXIT_FAILURE, "Head is null"); 
   
   if(!list->symbol)
   {
       list->symbol = symbol;
       return;
   }

   while(list->next)
   {
       list = list->next;
   }

   SymbolList *new_list = create_symbol_list();
   new_list->symbol = symbol;

   list->next = new_list;
}

void initialize_default_symbols(SymbolList *symbol_list)
{
    for(int i = 0; i < 16; i++)
    {
        char s[4];
        sprintf(s, "R%d", i);
        add_symbol(symbol_list, create_symbol(s, i));
    }

    add_symbol(symbol_list, create_symbol("SCREEN", 16384));
    add_symbol(symbol_list, create_symbol("KBD", 24576));
    add_symbol(symbol_list, create_symbol("SP", 0));
    add_symbol(symbol_list, create_symbol("LCL", 1));
    add_symbol(symbol_list, create_symbol("ARG", 2));
    add_symbol(symbol_list, create_symbol("THIS", 3));
    add_symbol(symbol_list, create_symbol("THAT", 4));
}

Symbol *find_symbol_by_name(SymbolList *symbol_list, char *name)
{
    Symbol *symbol = NULL;
    while(symbol_list)
    {
        if(strcmp(symbol_list->symbol->name, name) == 0)
        {
            symbol = symbol_list->symbol;
        }

        symbol_list = symbol_list->next;
    }

    return symbol;
}

void print_symbol_list(SymbolList *list)
{
    SymbolList *iterable = list;
    while(iterable)
    {
        Symbol *symbol = iterable->symbol;
        fprintf(stderr, "name: %s, value: %d\n", symbol->name, symbol->value);
        iterable = iterable->next;
    }
}

void free_symbol(Symbol *symbol)
{
    free(symbol->name);
    free(symbol);
}

void free_symbol_list(SymbolList *list)
{
    SymbolList *next;
    while(list)
    {
        next = list->next;
        free_symbol(list->symbol);
        free(list);
        list = next;
    }
}
