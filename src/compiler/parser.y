%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "lexer.h"
#include <machine.h>
#include <instructions.h>

struct header *h;
FILE *input, *output;

struct label {
    struct label *next;
    char *s;
    int32_t value;
};

struct label *labels = NULL;

void new_label(char *name){
    struct label *l = malloc (sizeof(*l));
    l->next = labels;
    l->s = name;
    l->value = h->prog_size;
    labels = l;
}

int32_t get_label_value(char *name){
    for(struct label *it = labels; it != NULL; it = it->next){
	if(!strcmp(it->s, name))
	    return it->value;
    }
    return -1;
}

union param_value{
    int32_t integer;
    int8_t reg;
};

struct param{
    int8_t type;
    union param_value value;
};

struct param *new_param(int8_t type){
    struct param *p = malloc(sizeof(*p));
    p->type = type;
    return p;
}

struct params{
    unsigned nb_params;
    int8_t types[3];
    union param_value values[3];
    int8_t code;
};

struct params *new_params1(struct param *p){
    struct params *ps = malloc(sizeof(*ps));
    ps->nb_params = 1;
    ps->types[0] = p->type;
    ps->values[0] = p->value;
    ps->code = encode_parameters(p->type, 0, 0);
    return ps;
}

struct params *new_params2(struct param *p1, struct param *p2){
    struct params *ps = malloc(sizeof(*ps));
    ps->nb_params = 2;
    ps->types[0] = p1->type;
    ps->values[0] = p1->value;
    ps->types[1] = p2->type;
    ps->values[1] = p2->value;
    ps->code = encode_parameters(p1->type, p2->type, 0);
    return ps;
}

struct params *new_params3(struct param *p1, struct param *p2, struct param *p3){
    struct params *ps = malloc(sizeof(*ps));
    ps->nb_params = 3;
    ps->types[0] = p1->type;
    ps->values[0] = p1->value;
    ps->types[1] = p2->type;
    ps->values[1] = p2->value;
    ps->types[2] = p3->type;
    ps->values[2] = p3->value;
    ps->code = encode_parameters(p1->type, p2->type, p3->type);
    return ps;
}

void write_instruction(int8_t mnemo, struct params *p){
    fwrite(&mnemo, sizeof(int8_t), 1, output);
    fwrite(&(p->code), sizeof(int8_t), 1, output);
    h->prog_size += 2;
    for(int i = 0; i < p->nb_params; ++i){
	if(p->types[i] == REG_PARAM){
	    fwrite(&(p->values[i].reg), sizeof(int8_t), 1, output);
	    h->prog_size += sizeof(int8_t);
	}
	else if(p->types[i] == DIR_PARAM || 
		p->types[i] == IND_PARAM){
		fwrite(&(p->values[i].integer), sizeof(int32_t), 1, output);
		h->prog_size += sizeof(int32_t);
	}
    }
}

struct mnemo {
    int8_t opcode;
    char *name;
};

struct mnemo mnemos [] = {
    {LFORK, "lfork"},
    {STI, "sti"},
    {FORK, "fork"},
    {LLD, "lld"},
    {LD, "ld"},
    {ADD, "add"},
    {ZJMP, "zjmp"},
    {SUB, "sub"},
    {LDI, "ldi"},
    {OR, "or"},
    {ST, "st"},
    {AFF, "aff"},
    {LIVE, "live"},
    {XOR, "xor"},
    {LLDI, "lldi"},
    {AND, "and"}
};

int8_t get_opcode(char *mnemo){
    for(int i = 0; i < 16; ++i)
	if(!strcmp(mnemo, mnemos[i].name))
	    return mnemos[i].opcode;
    return 0;
}

%}

%union {
  int32_t integer;
  char *string;
  struct param *parameter;
  struct params *parameters;
}

%token <integer> T_REGISTER T_INTEGER
%token <string> T_LABEL T_MNEMO T_STRING
%token T_SEPARATOR T_LABEL_CHAR T_DIRECT_CHAR T_NAME T_COMMENT

%type <integer> direct indirect
%type <parameter> param
%type <parameters> params

%%

s:	s instr {}
	| {}
	;

instr: 	T_NAME T_STRING {snprintf(h->prog_name, PROG_NAME_LENGTH, "%s", $2);}
	|T_COMMENT T_STRING {snprintf(h->comment, COMMENT_LENGTH, "%s", $2);}
	|T_MNEMO params {write_instruction(get_opcode($1), $2);}
	|T_LABEL T_LABEL_CHAR {new_label($1);}
	;

params: param {$$ = new_params1($1);}
	|param T_SEPARATOR param {$$ = new_params2($1, $3);}
	|param T_SEPARATOR param T_SEPARATOR param {$$ = new_params3($1, $3, $5);}
	;

param:	T_REGISTER {$$ = new_param(REG_PARAM); $$->value.reg = $1;}
	|direct {$$ = new_param(DIR_PARAM); $$->value.reg = $1;}
	|indirect {$$ = new_param(IND_PARAM); $$->value.reg = $1;}
	;

direct:	T_DIRECT_CHAR T_INTEGER {$$ = $2;}
	|T_DIRECT_CHAR T_LABEL_CHAR T_LABEL {$$ = get_label_value($3);}
	;

indirect: 	T_INTEGER {$$ = $1;}
	|	T_LABEL_CHAR T_LABEL {$$ = get_label_value($2);}
	;

%%

void usage(const char *name){
    fprintf(stderr, "Usage : %s file.s\n", name);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    char *output_name = NULL;
    size_t output_name_size;
    if(argc != 2)
	usage(argv[0]);
    
    if(!strstr(argv[1], ".s")){
	fprintf(stderr, "Error : invalid extension (expected .s)\n");
	exit(EXIT_FAILURE);
    }
    
    input = fopen(argv[1], "r");
    if(input == NULL){
	fprintf(stderr, "Error while opening %s\n", argv[1]);
	exit(EXIT_FAILURE);
    }
    
    output_name_size = (strlen(argv[1]) + 3) * sizeof(*output_name);
    output_name = malloc(output_name_size);
    snprintf(output_name, output_name_size, "%s", argv[1]);
    snprintf(output_name + output_name_size - 4, 4, "cor");
    output = fopen(output_name, "w");
    if(output == NULL){
	fclose(input);
	fprintf(stderr, "Error while opening %s\n", output_name);
	exit(EXIT_FAILURE);
    }
    h = malloc(sizeof(*h));
    h->prog_size = 0;
    sprintf(h->prog_name, "default");
    sprintf(h->comment, "default");
    dup2(fileno(input), fileno(stdin));
    fseek(output, sizeof(struct header), SEEK_SET);
    yyparse();
    fseek(output, 0, SEEK_SET);
    fwrite(h, sizeof(struct header), 1, output);
    free(h);
    fclose(input);
    fclose(output);
    return EXIT_SUCCESS;
}
