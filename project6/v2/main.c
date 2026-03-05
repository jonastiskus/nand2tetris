#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <err.h>

typedef struct Instruction
{
    char *name;
    char *binary;
} Instruction;

typedef struct Line
{
    short number;
    char *text;
} Line;

typedef struct LineList
{
    Line *line;
    struct LineList *next;
} LineList;

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

Instruction comp_0_instructions[] =
{
    {"0", "101010"}, {"1", "111111"}, {"-1", "111010"},
    {"D", "001100"}, {"A", "110000"}, {"!D", "001101"},
    {"!A", "110001"}, {"-D", "001111"}, {"-A", "110011"},
    {"D+1", "011111"}, {"A+1", "110111"}, {"D-1", "001110"},
    {"A-1", "110010"}, {"D+A", "000010"}, {"D-A", "010011"},
    {"A-D", "000111"}, {"D&A", "000000"}, {"D|A", "010101"}
};

Instruction comp_1_instructions[] =
{
    {"M", "110000"}, {"!M", "110001"}, {"-M", "110011"},
    {"M+1", "110111"}, {"M-1", "110010"}, {"D+M", "000010"},
    {"D-M", "010011"}, {"M-D", "000111"}, {"D&M", "000000"},
    {"D|M", "010101"}
};

Instruction dest_instructions[] =
{
    {"null", "000"}, {"A", "100"}, {"M", "001"}, {"D", "010"},
    {"MD", "011"}, {"AM", "101"}, {"AD", "110"}, {"AMD", "111"}
};

Instruction jump_instructions[] =
{
    {"null", "000"}, {"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"},
    {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"} 
};

//------------------- Line begin ---------------------

Line *create_line(short number, char *text)
{
    Line *line = malloc(sizeof(Line));
    if(!line) err(EXIT_FAILURE, "Error allocating Line");

    line->number = number;
    line->text = strdup(text);

    return line;
}

LineList *create_line_list(void)
{
    LineList *line_list = malloc(sizeof(LineList));    
    if(!line_list) err(EXIT_FAILURE, "Error allocating LineList");

    line_list->line = NULL;
    line_list->next = NULL;

    return line_list;
}

void add_line(LineList *list, Line *new_line)
{
   if(!list) err(EXIT_FAILURE, "Head is null"); 
   
   if(!list->line)
   {
       list->line = new_line;
       return;
   }

   while(list->next)
   {
       list = list->next;
   }

   LineList *new_list = create_line_list();
   new_list->line = new_line;

   list->next = new_list;
}

void print_line_list(LineList *list)
{
    LineList *iterable = list;
    while(iterable)
    {
        Line *line = iterable->line;
        printf("Line: %d, text: %s\n", line->number, line->text);
        iterable = iterable->next;
    }
}

void free_line(Line *line)
{
    free(line->text);
    free(line);
}

void free_line_list(LineList *list)
{
    LineList *next;
    while(list)
    {
        next = list->next;
        free_line(list->line);
        free(list);
        list = next;
    }
}

//--------------- Line end TODO move to its own header ------------------


//-------------------------- Symbol begin -------------------------------
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
        printf("name: %s, value: %d\n", symbol->name, symbol->value);
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
//-------------- Symbol end TODO move to its own header ----------------------

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
    new_string[len] = c;
    new_string[len+1] = '\0';

    return new_string;
}

bool is_line_label(char *line)
{
    return strlen(line) >= 2 &&
        line[0] == '(' &&
        line[strlen(line) - 1] == ')';
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

bool is_line_A_instruction(char *line)
{
    return strlen(line) >= 2 && line[0] == '@';
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

Instruction find_instruction_by_name(Instruction instructions[], size_t length, char *name)
{
    for(int i = 0; i < length; i++) {
        if(strcmp(name, instructions[i].name) == 0) {
            return instructions[i];
        }
    }
}

void *parse(char *buffer, LineList *line_list)
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
                printf("bits: %s = %d\n", bits, value);
                // TODO append bits to buffer
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
                        sizeof(dest_instructions),
                        destination);
                
                bits[10] = dest_instruction.binary[0];
                bits[11] = dest_instruction.binary[1];
                bits[12] = dest_instruction.binary[2];
                printf("bits: %s %s was added\n", bits, dest_instruction.binary);
            }
        }
        
        //printf("looping %d\n", line_list->line->number);
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

    puts("Testing");

    free_line_list(line_list);
    free_symbol_list(symbol_list);
    free(parse_buffer);
    return 0;
}

