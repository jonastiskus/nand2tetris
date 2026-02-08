#ifndef PARSER_H
#define PARSER_H

#include "symbols.h"
#include "utils.h"

typedef struct {
    const char *name;
    const char *bin_value;
} Comp;

#define COMP_0_INS_MAP                                                         \
{"0", "101010"}, {"1", "111111"}, {"-1", "111010"}, {"D", "001100"},         \
{"A", "110000"}, {"!D", "001101"}, {"!A", "110001"}, {"-D", "001111"},   \
{"-A", "110011"}, {"D+1", "011111"}, {"A+1", "110111"},                  \
{"D-1", "001110"}, {"A-1", "110010"}, {"D+A", "000010"},                 \
{"D-A", "010011"}, {"A-D", "000111"}, {"D&A", "000000"},                 \
{"D|A", "010101"}

#define COMP_1_INS_MAP                                                         \
{"M", "110000"}, {"!M", "110001"}, {"-M", "110011"}, {"M+1", "110111"},      \
{"M-1", "110010"}, {"D+M", "000010"}, {"D-M", "010011"},                 \
{"M-D", "000111"}, {"D&M", "000000"}, {"D|M", "010101"}

extern const Comp comp_0[];
extern const Comp comp_1[];

bool is_a_ins(char *str);
bool is_c_ins(char *str);
bool is_symbol(char *str);

char *parse(char *buff, Symbol *head);

#endif
