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

bool is_line_C_instruction(char *line)
{
    return strlen(line) >= 2 && (strchr(line, '=') || strchr(line, ';'));
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
    const short ASCII_OFFSET = '0';
    int i = strlen(bits) - 1; // -1 because MSB defines instruction type
    while(i) {
        bits[i] = (value % 2) + ASCII_OFFSET;
        value = value / 2;
        i--;
    }
}

char *parse(LineList *line_list, SymbolList *symbol_list)
{
    const short ASCII_OFFSET = '0';
    char *buffer = alloc_string();

    for(;line_list; line_list = line_list->next)
    {
        char *line = line_list->line->text;
        char *destination = "null";
        char *computation = "";
        char *jump = "null";

        if(is_line_A_instruction(line))
        {
            char bits[17] = "0000000000000000";
            bool is_symbol = is_line_A_instruction(line) && !isdigit(line[1]);

            if(is_symbol)
            {
                Symbol *symbol = find_symbol_by_name(symbol_list, ++line);
                if(symbol)
                {
                    number_to_bits(bits, symbol->value);
                }
                else err(EXIT_FAILURE, "A instruction symbol not found");
            } else number_to_bits(bits, (short) atoi(++line));

            buffer = append_string(buffer, bits);
            buffer = append_char(buffer, '\n');
        } else if (is_line_C_instruction(line))
        {
            char bits[17] = "1110000000000000";
            char *has_destination = strchr(line, '=');
            char *has_jump = strchr(line, ';');
            char *iterator = line;

            if(has_destination)
            {
                char destination_substr[20];
                int i = 0;
                for(; *iterator != '='; i++)
                {
                    destination_substr[i] = *iterator;
                    iterator++;
                }
                destination_substr[i] = '\0';
                destination = destination_substr;
                iterator++; // Skip '=' for computation
            }

            char computation_substr[20];
            int i = 0;
            for(; *iterator != '\0' && *iterator != ';'; i++)
            {
                computation_substr[i] = *iterator;
                iterator++;
            }
            computation_substr[i] = '\0';
            computation = computation_substr;
         

            if(has_jump)
            {
                iterator++; //Skip ';'
                char jump_substr[20];
                int i = 0;
                while(*iterator != '\0') 
                {
                    jump_substr[i] = *iterator;
                    iterator++;
                    i++;
                }
                jump_substr[i] = '\0';
                jump = jump_substr;
            }

            Instruction *dest_instruction = find_instruction_by_name(
                    dest_instructions,
                    DEST_INSTRUCTIONS_SIZE,
                    destination);

            if(!dest_instruction) err(EXIT_FAILURE, "Destination instruction was not found");

            bool second_comp = false;
            Instruction *comp_instruction = find_instruction_by_name(
                    comp_0_instructions,
                    COMP_0_INSTRUCTIONS_SIZE,
                    computation);

            if(!comp_instruction)
            {
                second_comp = true;
                comp_instruction = find_instruction_by_name(
                        comp_1_instructions,
                        COMP_1_INSTRUCTIONS_SIZE,
                        computation);

                if(!comp_instruction) err(EXIT_FAILURE, "Computation instruction was not found");
            } 

            Instruction *jump_instruction = find_instruction_by_name(
                    jump_instructions,
                    JUMP_INSTRUCTIONS_SIZE,
                    jump);

            bits[3] = second_comp + ASCII_OFFSET;
            bits[4] = comp_instruction->binary[0];
            bits[5] = comp_instruction->binary[1];
            bits[6] = comp_instruction->binary[2];
            bits[7] = comp_instruction->binary[3];
            bits[8] = comp_instruction->binary[4];
            bits[9] = comp_instruction->binary[5];
            bits[10] = dest_instruction->binary[0];
            bits[11] = dest_instruction->binary[1];
            bits[12] = dest_instruction->binary[2];
            bits[13] = jump_instruction->binary[0];
            bits[14] = jump_instruction->binary[1];
            bits[15] = jump_instruction->binary[2];

            buffer = append_string(buffer, bits);
            buffer = append_char(buffer, '\n');
        } else 
        {
            fprintf(stderr, "Unknown instruction %s. Skipping\n", line);
            continue;
        }
    }
    return buffer;
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

