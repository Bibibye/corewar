%{

#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

%}

%%

s:		
	;
%%

int main(void) {
  printf("Hello, world!\n");
  //yyparse();
  return EXIT_SUCCESS;
}
