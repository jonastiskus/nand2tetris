#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"

Symbol *symbol_table_create(void) {
    Symbol *tmp = malloc(sizeof(Symbol));
    if (tmp == NULL) {
        printf("Error reallocating buffer memory");
        exit(0);
    }

    tmp->next = NULL;
    return tmp;
}

void add_to_symbol_table(Symbol **head, Symbol *symbol) {
    Symbol *tmp = *head;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = symbol;
}

void print_symbols(Symbol *head) {
    Symbol *curr = head;
    while (curr != NULL) {
        printf("label: %s, line: %d\n", curr->label, curr->value);
        curr = curr->next;
    }
}

Symbol *symbol_create(int value, char *label) {
    Symbol *symbol = malloc(sizeof(Symbol));
    if (symbol == NULL) {
        printf("Error allocating buffer memory");
        exit(0);
    }

    symbol->value = value;
    symbol->label = strdup(label);
    symbol->next = NULL;
    return symbol;
}

bool symbol_exists(Symbol *head, Symbol *symbol) {
    while (head != NULL) {
        if (strcmp(head->label, symbol->label) == 0) {
            return true;
        }
        head = head->next;
    }
    return false;
}

Symbol *create_symbol_table(void) {
    Symbol *head = symbol_create(0, "R0");
    for (int i = 1; i < 16; i++) {
        char s[4];
        sprintf(s, "R%d", i);
        Symbol *symbol = symbol_create(i, s);
        add_to_symbol_table(&head, symbol);
    }
    add_to_symbol_table(&head, symbol_create(16384, "SCREEN"));
    add_to_symbol_table(&head, symbol_create(24576, "KBD"));
    add_to_symbol_table(&head, symbol_create(0, "SP"));
    add_to_symbol_table(&head, symbol_create(1, "LCL"));
    add_to_symbol_table(&head, symbol_create(2, "ARG"));
    add_to_symbol_table(&head, symbol_create(3, "THIS"));
    add_to_symbol_table(&head, symbol_create(4, "THAT"));

    return head;
}

void free_symbol_table(Symbol *head) {
    while (head->next != NULL) {
        Symbol *next = head->next;
        free(head->label);
        free(head);
        head = next;
    }
}
