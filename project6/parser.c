#include "parser.h"
#include "utils.h"
#include <ctype.h>

const Ins comp_0[] = {COMP_0_INS_MAP};
const Ins comp_1[] = {COMP_1_INS_MAP};
const Ins dest[] = {DEST_MAP};

bool is_a_ins(char *str) { return is_variable_begin(str[0]); }

bool is_c_ins(char *str) {
    return !is_label_begin(str[0]) && !is_variable_begin(str[0]);
}

bool is_symbol(char *str) { return is_a_ins(str) && !isdigit(*(++str)); }

void skip_comment(char **buff) {
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

void parse_variable(char *buff, char **parse) {
    buff++; //skip @
    char *var = malloc(sizeof(char));
    var[0] = '\0';

    while(*buff != '\n') {
        var = append_char(var, *buff);
        buff++;
    }

    if (isdigit(var[0])) {
        char *bits = str_to_16_bits(var);
        *parse = append_string(*parse, bits);
        *parse = append_char(*parse, '\n');
        free(bits);
    } else {
        //TODO lookup in symbols and take that value
    }

    free(var);
}

Ins *find_comp_0_by_name(char *name) {
    Ins *c = malloc(sizeof(Ins));
    size_t comp_len = sizeof(comp_0) / sizeof(Ins);
    for(int i = 0; i < comp_len; i++) {
        c = &comp_0[i];
        if(strcmp(name, c->name) == 0) {
            return c;
        }
    }

    return NULL;
}

Ins *find_comp_1_by_name(char *name) {
    Ins *c = malloc(sizeof(Ins));
    size_t comp_len = sizeof(comp_1) / sizeof(Ins);
    for(int i = 0; i < comp_len; i++) {
        c = &comp_1[i];
        if(strcmp(name, c->name) == 0) {
            return c;
        }
    }

    return NULL;
}

Ins *find_dest_by_name(char *name) {
    Ins *c = malloc(sizeof(Ins));
    size_t dest_len = sizeof(dest) / sizeof(Ins);
    for(int i = 0; i < dest_len; i++) {
        c = &dest[i];
        if(strcmp(name, c->name) == 0) {
            return c;
        }
    }

    return NULL;
}

void parse_c_ins(char *buff, char **parse) {
    char *dest = malloc(sizeof(char));
    dest[0] = '\0';
    char *comp = malloc(sizeof(char));
    comp[0] = '\0';

    *parse = append_string(*parse, "111");

    // rewind pointer to dest start
    while(!isspace(*buff)){
        buff--;
    }
    buff++;

    while(*buff != '=') {
        dest = append_char(dest, *buff);   
        buff++;
    }

    buff++;
    while(*buff != '\n') {
        if(!isspace(*buff)) {
            comp = append_char(comp, *buff);
        }
        buff++;
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
    *parse = append_char(*parse, '\n');


    //apend jmp
    //TODO

    free(dest);
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
        switch(*buff) {
            case '/':
                skip_comment(&buff);
                break;
            case '@':
                parse_variable(buff, &parse);
                break;
            case '=':
                parse_c_ins(buff, &parse);
                break;
            case '\n':
                line_num++;
                break;
            default:
                break;
        }
        buff++;
    }
    printf("%s", parse);
    return "test";
}
