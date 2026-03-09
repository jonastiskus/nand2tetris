#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stddef.h>

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

extern Instruction find_instruction_by_name(Instruction instructions[], size_t length, char *name);

#endif

