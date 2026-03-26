#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <stddef.h>

/* 
 * ---------------
 *      Lines
 * ---------------
 */

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

#endif
