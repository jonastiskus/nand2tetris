#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <err.h>


typedef struct Line
{
    int number;
    char *text;
} Line;

typedef struct LineList
{
    Line *line;
    struct LineList *next;
} LineList;

Line *create_line(int number, char *text)
{
    Line *line = malloc(sizeof(Line));
    if(!line) err(EXIT_FAILURE, "Error allocating Line");

    line->number = number;
    line->text = strdup(text);

    return line;
}

void free_line(Line *line)
{
    free(line->text);
    free(line);
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

int main(void)
{
    bool is_comment = false;
    // TODO Maybe we dont need int here. Shorter size may suffice.
    int line_num = 0;
    char *line = alloc_string();

    LineList *list = create_line_list();
    
    int c;
    while((c = getc(stdin)) != EOF)
    {
        if(c == '/') is_comment = true;
        //TODO Implement and test multiline comments like so /* comment */!
        if(is_comment && c == '\n') is_comment = false;
        if(is_comment) continue;    
        //{
        //    if(c == '\n')
        //    {
        //        is_comment = false;
        //    } else {
        //        continue;
        //    }
        //}     
        if(!isspace(c))
        {
            line = append_char(line, c);
        }
        if(strlen(line) > 0 && c == '\n')
        {
            add_line(list, create_line(line_num++, line));
            free(line);
            line = alloc_string();
        }

        //printf("%c is comment %d\n", c, is_comment);
    }

    //printf("%s\n", line);
    print_line_list(list);

    free_line_list(list);
    return 0;
}

