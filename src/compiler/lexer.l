%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"

void yyerror (char *s) {
    fprintf (stderr, "%s\n", s);
}

void my_echo() {
    printf("%s", yytext);
}

%}

%%

\#.*\n {}
. my_echo();

%%
