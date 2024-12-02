%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>

%}

%%

[ \t\n]+                   /* Ignorar espacios y saltos de línea */
"IF"                       { return IF; }
"THEN"                     { return THEN; }
"ELSE"                     { return ELSE; }
"END"                      { return END; }
"REPEAT"                   { return REPEAT; }
"UNTIL"                    { return UNTIL; }
"READ"                     { return READ; }
"WRITE"                    { return WRITE; }
":="                       { return ASSIGN; }
"<"                        { return LT; }
">"                        { return GT; }
"="                        { return EQ; }
"<>"                       { return NE; }
"<="                       { return LE; }
">="                       { return GE; }
"+"                        { return PLUS; }
"-"                        { return MINUS; }
"*"                        { return MUL; }
"/"                        { return DIV; }
";"                        { return SEMICOLON; }
"("                        { return LPAREN; }
")"                        { return RPAREN; }
[0-9]+                     { yylval.num = atoi(yytext); return NUMERO; }
[a-zA-Z_][a-zA-Z0-9_]*     { yylval.id = strdup(yytext); return IDENTIFICADOR; }
.                          { return yytext[0]; }

%%

int yywrap() {
    return 1;
}