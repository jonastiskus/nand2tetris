#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <err.h>

#include "instruction.h"
#include "line.h"
#include "symbol.h"

char *alloc_string(void)
{
   char *string = malloc(sizeof(char)); 
   if (!string) err(EXIT_FAILURE, "Error allocating string");

   string[0] = '\0';

   return string;
}

char *append_char(char *str, char c)
{
    size_t len = strlen(str);
    char *new_string = realloc(str, len + 2);
    if(!new_string) err(EXIT_FAILURE, "Error reallocating string");

    new_string[len] = c;
    new_string[len+1] = '\0';

    return new_string;
}

char *append_string(char *location, char *str) {
    size_t size = strlen(location) + strlen(str) + 1;
    char *new_string = realloc(location, size);
    if(!new_string) err(EXIT_FAILURE, "Error reallocating string");

    strcat(new_string, str);
    return new_string;
}

bool is_line_label(char *line)
{
    return strlen(line) >= 2 &&
        line[0] == '(' &&
        line[strlen(line) - 1] == ')';
}

bool is_line_A_instruction(char *line)
{
    return strlen(line) >= 2 && line[0] == '@';
}

void run_first_pass(LineList *line_list, SymbolList *symbol_list)
{
    while(line_list)
    {
        char *copy = strdup(line_list->line->text);
        short number = line_list->line->number;

        if(is_line_label(copy)) {
            size_t length = strlen(copy);
            copy[length - 1] = '\0';
            char *trimmed = copy + 1;

            if(!find_symbol_by_name(symbol_list, trimmed))
            {
                add_symbol(symbol_list, create_symbol(trimmed, number));
            }
        }
        line_list = line_list->next;
        free(copy);
    }
}

void run_second_pass(LineList *line_list, SymbolList *symbol_list)
{
    short value = 16;
    while(line_list)
    {
        char *text = line_list->line->text;
        short number = line_list->line->number;
        bool is_symbol = is_line_A_instruction(text) && !isdigit(text[1]);

        if(is_symbol)
        {
            char *trimmed = ++text;

            if(!find_symbol_by_name(symbol_list, trimmed))
            {
                add_symbol(symbol_list, create_symbol(trimmed, value++));
            }
        }
        line_list = line_list->next;
    }
}

void number_to_bits(char *bits, short value)
{
    int i = strlen(bits) - 1; // -1 because MSB defines instruction type
    while(i) {
        bits[i] = (value % 2) + '0';
        value = value / 2;
        i--;
    }
}

void parse(char *buffer, LineList *line_list)
{
    for(;line_list; line_list = line_list->next)
    {
        char *line = line_list->line->text;
        char *destination = "null";
        char *computation = "";
        char *jump = "null";

        if(is_line_A_instruction(line))
        {
            bool is_symbol = is_line_A_instruction(line) && !isdigit(line[1]);
            if(is_symbol)
            {
                            
                // TODO find symbol from symbol list and get its value into binary
            } else 
            {
                short value = atoi(++line);
                char bits[17] = "0000000000000000";
                number_to_bits(bits, value);
                append_string(buffer, bits);
                append_char(buffer, '\n');
            }
        } else
        {
            char bits[17] = "1000000000000000";
            char *destination = strchr(line, '=');

            if(destination)
            {
                size_t length = destination - line;
                char substring[20];

                strncpy(substring, line, length);
                substring[length] = '\0';
                destination = substring;

                Instruction dest_instruction = find_instruction_by_name(
                        dest_instructions,
                        DEST_INSTRUCTIONS_SIZE,
                        destination);
                
                bits[10] = dest_instruction.binary[0];
                bits[11] = dest_instruction.binary[1];
                bits[12] = dest_instruction.binary[2];
                append_string(buffer, bits);
                append_char(buffer, '\n');
            }
        }
    }
}

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

    SymbolList *symbol_list = create_symbol_list();
    initialize_default_symbols(symbol_list);

    run_first_pass(line_list, symbol_list);
    run_second_pass(line_list, symbol_list);

    char *parse_buffer = alloc_string(); 
    parse(parse_buffer, line_list);

    fputs(parse_buffer, stdout);

    free_line_list(line_list);
    free_symbol_list(symbol_list);
    free(parse_buffer);
    free(line);
    return 0;
}

