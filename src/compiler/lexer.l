%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"

unsigned line = 1;
  
void yyerror (char *s) {
  fprintf (stderr, "%s at line %d :\n -> %s\n", s, line, yytext);
}

void my_echo() {
    printf("%s", yytext);
}

%}

MNEMO ("lfork"|"sti"|"fork"|"lld"|"ld"|"add"|"zjmp"|"sub"|"ldi"|"or"|"st"|"aff"|"live"|"xor"|"lldi"|"and")
PARAM_SEPARATOR (\,)
LABEL ([[:alpha:]][[:alpha:][:digit:]]*)
LABEL_CHAR (\:)
COMMENT_CHAR (\#)
DIRECT_CHAR (\%)
REGISTER (r[[:digit:]]+)

%x STRING

%%

<INITIAL>{
  {MNEMO} {yylval.string = strdup(yytext); return T_MNEMO;}
  {PARAM_SEPARATOR} {return T_SEPARATOR;}
  {REGISTER} {yylval.integer = atoi(yytext+1); return T_REGISTER;}
  {LABEL} {yylval.string = strdup(yytext); return T_LABEL;}
  {LABEL_CHAR} {return T_LABEL_CHAR;}
  {DIRECT_CHAR} {return T_DIRECT_CHAR;}
  ".name" {return T_NAME;}
  ".comment" {return T_COMMENT;}
  [[:digit:]]+ {yylval.integer = atoi(yytext); return T_INTEGER;}
  
  \" {BEGIN STRING;}

  \n {++line;}
  [[:space:]] ;
  {COMMENT_CHAR}.*\n ;
}

<STRING>{
  \" {
    BEGIN INITIAL; 
    yytext[yyleng-1]='\0'; 
    yylval.string = strdup(yytext); 
    return T_STRING;
  }
  . {yymore();}
}

%%
