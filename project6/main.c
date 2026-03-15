#include <stdlib.h>
#include <stdio.h>
#include "assembler.h"

int main(void)
{
    LineList *line_list = create_line_list();
    reader_read_into_lines(line_list);

    SymbolList *symbol_list = create_symbol_list();
    initialize_default_symbols(symbol_list);

    run_first_pass(line_list, symbol_list);
    run_second_pass(line_list, symbol_list);

    char *binary = parse(line_list, symbol_list);
    fputs(binary, stdout);

    free_line_list(line_list);
    free_symbol_list(symbol_list);
    free(binary);

    return 0;
}

