#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <err.h>

//#include "instruction.h"
//#include "line.h"
//#include "symbol.h"
//#include "utils.h"
//#include "parser.h"
#include "assembler.h"

int main(void)
{
    bool is_comment = false;
    short line_num = 0;
    char *line = alloc_string();

    LineList *line_list = create_line_list();
    
    int c;
    while((c = getc(stdin)) != EOF)
    {
        //TODO Implement and test multiline comments like so /* comment */!
        if(c == '/') is_comment = true;
        if(is_comment && c == '\n') is_comment = false;
        if(is_comment) continue;    

        if(!isspace(c))
        {
            line = append_char(line, c);
        }
        if(strlen(line) > 0 && c == '\n')
        {
            add_line(line_list, create_line(line_num, line));
            if(!is_line_label(line)) line_num++;

            free(line);
            line = alloc_string();
        }
    }

    if(strlen(line) > 0)
    {
        add_line(line_list, create_line(line_num, line));
        if(!is_line_label(line)) line_num++;
    }

    SymbolList *symbol_list = create_symbol_list();
    initialize_default_symbols(symbol_list);

    run_first_pass(line_list, symbol_list);
    run_second_pass(line_list, symbol_list);

    char *parse_buffer = parse(line_list, symbol_list);
    fputs(parse_buffer, stdout);

    free_line_list(line_list);
    free_symbol_list(symbol_list);
    free(parse_buffer);
    free(line);
    return 0;
}

