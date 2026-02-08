#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print_raw(char *buff);
void remove_comments(char *buff);

char *append_string(char *buff, char *str);

bool is_new_line(char c);
bool is_label_begin(char c);
bool is_label_end(char c);
bool is_variable_begin(char c);
bool is_variable_end(char c);

#endif
