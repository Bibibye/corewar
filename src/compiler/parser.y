%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    
#include "lexer.h"
#include <machine.h>

struct header *h;
FILE *input, *output;

%}

%union {
  int integer;
  char *string;
}

%token <integer> T_REGISTER T_INTEGER
%token <string> T_LABEL T_MNEMO T_STRING
%token T_SEPARATOR T_LABEL_CHAR T_DIRECT_CHAR T_NAME T_COMMENT

%%

s:	instr {}
	|instr s {}
	;

instr: 	T_NAME T_STRING {}
	|T_COMMENT T_STRING {}
	|label T_MNEMO params {}
	|label {}
	;

label:	T_LABEL T_LABEL_CHAR {}
	;

params: param {}
	|param T_SEPARATOR param {}
	|param T_SEPARATOR param T_SEPARATOR param {}
	;

param:	T_REGISTER {}
	|direct {}
	|indirect {}
	;

direct:	T_DIRECT_CHAR T_INTEGER {}
	|T_DIRECT_CHAR T_LABEL_CHAR T_LABEL {}
	;

indirect: 	T_INTEGER {}
	|T_LABEL_CHAR T_LABEL {}
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
    // yyparse();
    fclose(input);
    fclose(output);
    return EXIT_SUCCESS;
}
