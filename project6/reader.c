#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "assembler.h"

void reader_read_into_lines(LineList *line_list)
{
    bool is_comment = false;
    short line_num = 0;
    char *line = alloc_string();

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

    free(line);
}
