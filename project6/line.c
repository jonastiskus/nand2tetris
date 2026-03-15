#include "line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

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

extern void add_line(LineList *list, Line *new_line)
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
        fprintf(stderr, "Line: %d, text: %s\n", line->number, line->text);
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
