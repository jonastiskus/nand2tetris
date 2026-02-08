#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void print_raw(char *buff) {
    int len = strlen(buff) + 1;
    for (int i = 0; i < len; i++) {
        switch (buff[i]) {
            case '\0':
                printf("%s\n", "\\0");
                break;
            case '\t':
                printf("%s\n", "\\t");
                break;
            case '\n':
                printf("%s\n", "\\n");
                break;
            default:
                printf("%c", buff[i]);
        }
    }
}

char *append_char(char *buff, char c) {
    size_t len = strlen(buff);
    char *tmp = realloc(buff, len + 2);
    tmp[len] = c;
    tmp[len+1] = '\0';

    return tmp;
}

char *append_string(char *buff, char *str) {
    char *tmp = realloc(buff, strlen(buff) + strlen(str) + 1);
    strcat(tmp, str);
    return tmp;
}

void remove_comments(char *buff) {
    for (int i = 0; buff[i] != '\0'; i++) {
        bool is_comment =
            buff[i] == '/' && buff[i + 1] == '/' && buff[i + 1] != '\0';
        if (is_comment) {
            int j = i;
            while (buff[j] != '\n' && buff[j] != '\0')
                j++;
            if (buff[j] != '\n')
                j++;

            int shift = 0;
            while (buff[j + shift] != '\0') {
                buff[i + shift] = buff[j + shift];
                shift++;
            }
            buff[i + shift] = '\0';

            i--;
        }
    }
}

bool is_new_line(char c) { return c == '\n'; }

bool is_label_begin(char c) { return c == '('; }

bool is_label_end(char c) { return c == ')'; }

bool is_variable_begin(char c) { return c == '@'; }

bool is_variable_end(char c) { return isspace(c); }
