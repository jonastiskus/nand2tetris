#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <err.h>
#include <ctype.h>
#include "assembler.h"

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
            // TODO Refactor to assembly function
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
