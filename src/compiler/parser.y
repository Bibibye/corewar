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
int32_t size = 0;

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
    l->value = size;
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

%}

%union {
  int32_t integer;
  char *string;
  struct param *parameter;
}

%token <integer> T_REGISTER T_INTEGER
%token <string> T_LABEL T_MNEMO T_STRING
%token T_SEPARATOR T_LABEL_CHAR T_DIRECT_CHAR T_NAME T_COMMENT

%type <integer> direct indirect
%type <parameter> param

%%

s:	s instr {}
	| {}
	;

instr: 	T_NAME T_STRING {snprintf(h->prog_name, PROG_NAME_LENGTH, "%s", $2);}
	|T_COMMENT T_STRING {snprintf(h->comment, COMMENT_LENGTH, "%s", $2);}
	|T_MNEMO params {}
	|T_LABEL T_LABEL_CHAR {new_label($1);}
	;

params: param {}
	|param T_SEPARATOR param {}
	|param T_SEPARATOR param T_SEPARATOR param {}
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
    yyparse();
    free(h);
    fclose(input);
    fclose(output);
    return EXIT_SUCCESS;
}
