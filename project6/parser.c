#include "parser.h"
#include "utils.h"
#include <ctype.h>

const Ins comp_0[] = {COMP_0_INS_MAP};
const Ins comp_1[] = {COMP_1_INS_MAP};
const Ins dest[] = {DEST_MAP};
const Ins jmp[] = {JMP_MAP};

bool is_a_ins(char *str) { return is_variable_begin(str[0]); }

bool is_c_ins(char *str) {
    return !is_label_begin(str[0]) && !is_variable_begin(str[0]);
}

bool is_symbol(char *str) { return is_a_ins(str) && !isdigit(*(++str)); }

void skip_line(char **buff) {
    char *tmp = *buff;
    while(*tmp != '\n') {
        tmp++;
    }
    *buff = tmp;
}

char *str_to_16_bits(char *str) {
    int i = 15;
    int val = atoi(str);

    char *bits = malloc(17);
    for (int i = 0; i <= 15; i++) {
        bits[i] = '0';
    }
    bits[16] = '\0';

    while(true) {
        bits[i] = (val % 2) + '0';
        val = val / 2;
        i--;

        if (val == 0) break;
    }

    return bits;
}

void parse_variable(char **buff, char **parse) {
    char *tmp = *buff;
    tmp++; //skip @
    char *var = malloc(sizeof(char));
    var[0] = '\0';

    while(*tmp != '\n') {
        var = append_char(var, *tmp);
        tmp++;
    }

    if (isdigit(var[0])) {
        char *bits = str_to_16_bits(var);
        *parse = append_string(*parse, bits);
        *parse = append_char(*parse, '\n');
        free(bits);
    } else {
        //TODO lookup in symbols and take that value
    }

    *buff = tmp;
    free(var);
}

Ins *find_comp_0_by_name(char *name) {
    size_t comp_len = sizeof(comp_0) / sizeof(Ins);
    for(int i = 0; i < comp_len; i++) {
        if(strcmp(name, comp_0[i].name) == 0) {
            return &comp_0[i];
        }
    }

    return NULL;
}

Ins *find_comp_1_by_name(char *name) {
    size_t comp_len = sizeof(comp_1) / sizeof(Ins);
    for(int i = 0; i < comp_len; i++) {
        if(strcmp(name, comp_1[i].name) == 0) {
            return &comp_1[i];
        }
    }

    return NULL;
}

Ins *find_dest_by_name(char *name) {
    size_t dest_len = sizeof(dest) / sizeof(Ins);
    for(int i = 0; i < dest_len; i++) {
        if(strcmp(name, dest[i].name) == 0) {
            return &dest[i];
        }
    }

    return NULL;
}

Ins *find_jmp_by_name(char *name) {
    size_t jmp_len = sizeof(jmp) / sizeof(Ins);
    for(int i = 0; i < jmp_len; i++) {
        if(strcmp(name, jmp[i].name) == 0) {
            return &jmp[i];
        }
    }

    return NULL;
}

void parse_c_ins(char **buff, char **parse) {
    *parse = append_string(*parse, "111");

    char *tmp = *buff;
    char *dest = malloc(sizeof(char));
    dest[0] = '\0';
    char *comp = malloc(sizeof(char));
    comp[0] = '\0';
    char *jmp = malloc(sizeof(char));
    jmp[0] = '\0';
    char *line = malloc(sizeof(char));
    line[0] = '\0';

    while(*tmp != '\n') {
        line = append_char(line, *tmp);
        tmp++;
    }

    bool has_dest = strstr(line, "=") != NULL;
    bool has_jmp = strstr(line, ";") != NULL;

    tmp = *buff;
    if(has_dest) {
        while(*tmp != '=') {
            if(!isspace(*tmp)) {
                dest = append_char(dest, *tmp);   
            }
            tmp++;
        }
        tmp++;
    } else {
        dest = append_string(dest, "null");
    }

    if(has_jmp) {
        while(*tmp != ';') {
            if(!isspace(*tmp)) {
                comp = append_char(comp, *tmp);
            }
            tmp++;
        }
        tmp++;

        while(*tmp != '\n') {
            if(!isspace(*tmp)) {
                jmp = append_char(jmp, *tmp);
            }
            tmp++;
        }
    } else {
        while(*tmp != '\n') {
            if(!isspace(*tmp)) {
                comp = append_char(comp, *tmp);
            }
            tmp++;
        }
        jmp = append_string(jmp, "null");
    }
   
    // append comp
    bool is_a = false;
    Ins *c = find_comp_0_by_name(comp);
    if(c == NULL) {
        c = find_comp_1_by_name(comp);
        if(c != NULL) {
            is_a = true;
        }
    }
    *parse = append_string(*parse, is_a ? "1" : "0");
    *parse = append_string(*parse, c->bin_value);

    // append dest 
    Ins *d = find_dest_by_name(dest);
    *parse = append_string(*parse, d->bin_value);

    //apend jmp
    Ins *j = find_jmp_by_name(jmp);
    *parse = append_string(*parse, j->bin_value);
    *parse = append_char(*parse, '\n');

    *buff = tmp;

    free(line);
    free(dest);
    free(comp);
    free(jmp);
}

void parse_label(char **buff) {
    char *tmp = *buff;
    while(*tmp != '\n') {
        tmp++;
    }
    *buff = tmp;
}

char *parse(char *buff) {
    int line_num = 0;
    char *parse = malloc(sizeof(char));
    parse[0] = '\0';

    while(*buff != '\0') {
        if(isspace(*buff)) {
            buff++;
            continue;
        }
        
        switch(*buff) {
            case '@':
                parse_variable(&buff, &parse);
                break;
            case '(':
                skip_line(&buff);
                break;
            default:
                parse_c_ins(&buff, &parse);
                break;
        }
        buff++;
    }
    printf("%s", parse);
    free(parse); //TODO return parse and free at caller
    return "test";
}
