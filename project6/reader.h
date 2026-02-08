#ifndef READER_H
#define READER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "symbols.h"

char *read_file(FILE *file);

void read_label_symbols(Symbol **head, char *buff);
void read_variables(Symbol **head, char *buff);

#endif
