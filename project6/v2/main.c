#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <err.h>

typedef struct Instruction
{
    char *insturction;
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
   
   if(list->line == NULL)
   {
       list->line = new_line;
       return;
   }

   while(list->next != NULL)
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
   
   if(list->symbol == NULL)
   {
       list->symbol = symbol;
       return;
   }

   while(list->next != NULL)
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

char *parse(LineList *list)
{
    //TODO Implement
    for(LineList *i = list; i; i = i->next)
    {
        printf("looping %d\n", i->line->number);
    }
}

void run_first_pass(LineList *line_list, SymbolList *symbol_list)
{
    while(line_list)
    {
        char *text = line_list->line->text;
        short number = line_list->line->number;
        bool is_line_label = (text[0] == '(') && (text[strlen(text) - 1] == ')');

        if(is_line_label) {
            char *trimmed = ++text;
            size_t length = strlen(trimmed);
            trimmed[length - 1] = trimmed[length];

            if(!find_symbol_by_name(symbol_list, trimmed))
            {
                // TODO passing plain number might be wrong here! Need to address pointing
                // Maybe count how many labels we found already and subtract.
                add_symbol(symbol_list, create_symbol(trimmed, number));
            }
        }

        line_list = line_list->next;
    }
}

void run_second_pass(LineList *line_list, SymbolList *symbol_list)
{
    short value = 16;
    while(line_list)
    {
        char *text = line_list->line->text;
        short number = line_list->line->number;
        bool is_line_variable = text[0] == '@';

        if(is_line_variable)
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
            add_line(line_list, create_line(line_num++, line));
            free(line);
            line = alloc_string();
        }
    }

    SymbolList *symbol_list = create_symbol_list();
    initialize_default_symbols(symbol_list);

    run_first_pass(line_list, symbol_list);
    run_second_pass(line_list, symbol_list);
    print_symbol_list(symbol_list);

    free_line_list(line_list);
    free_symbol_list(symbol_list);
    return 0;
}

