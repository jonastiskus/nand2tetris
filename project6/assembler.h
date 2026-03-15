#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stddef.h>
#include <stdbool.h>

/*-------------------Instructions------------------- */
typedef struct Instruction
{
    char *name;
    char *binary;
} Instruction;

extern Instruction comp_0_instructions[];
extern Instruction comp_1_instructions[];
extern Instruction dest_instructions[];
extern Instruction jump_instructions[];

extern const size_t COMP_0_INSTRUCTIONS_SIZE;
extern const size_t COMP_1_INSTRUCTIONS_SIZE;
extern const size_t DEST_INSTRUCTIONS_SIZE;
extern const size_t JUMP_INSTRUCTIONS_SIZE;

extern Instruction *find_instruction_by_name(Instruction instructions[], size_t length, char *name);

/*-------------------Lines------------------- */
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

extern Line *create_line(short number, char *text);
extern LineList *create_line_list(void);
extern void add_line(LineList *list, Line *new_line);
extern void print_line_list(LineList *list);
extern void free_line(Line *line);
extern void free_line_list(LineList *list);

/*-------------------Symbols------------------- */
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

extern Symbol *create_symbol(char *name, short value);
extern SymbolList *create_symbol_list(void);
extern Symbol *find_symbol_by_name(SymbolList *symbol_list, char *name);
extern void initialize_default_symbols(SymbolList *symbol_list);
extern void add_symbol(SymbolList *list, Symbol *symbol);
extern void print_symbol_list(SymbolList *list);
extern void free_symbol(Symbol *symbol);
extern void free_symbol_list(SymbolList *list);

/*-------------------Parser------------------- */
bool is_line_label(char *line);
bool is_line_A_instruction(char *line);
bool is_line_C_instruction(char *line);
void run_first_pass(LineList *line_list, SymbolList *symbol_list);
void run_second_pass(LineList *line_list, SymbolList *symbol_list);
char *parse(LineList *line_list, SymbolList *symbol_list);

/*-------------------Utils------------------- */
char *alloc_string(void);
char *append_char(char *str, char c);
char *append_string(char *location, char *str);
void number_to_bits(char *bits, short value);

#endif
