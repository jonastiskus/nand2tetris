#include <string.h>
#include "assembler.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof *x)

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

const size_t COMP_0_INSTRUCTIONS_SIZE = ARRAY_SIZE(comp_0_instructions);
const size_t COMP_1_INSTRUCTIONS_SIZE = ARRAY_SIZE(comp_1_instructions);
const size_t DEST_INSTRUCTIONS_SIZE = ARRAY_SIZE(dest_instructions);
const size_t JUMP_INSTRUCTIONS_SIZE = ARRAY_SIZE(jump_instructions);

Instruction *find_instruction_by_name(Instruction instructions[], size_t length, char *name)
{
    for(int i = 0; i < length; i++) {
        if(strcmp(name, instructions[i].name) == 0) {
            return &instructions[i];
        }
    }

    return NULL;
}

