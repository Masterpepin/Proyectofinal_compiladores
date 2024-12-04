%{
#include "syntax_tree.h"
#include "parser.tab.h"
extern int yylineno;
%}

%%
\n              { yylineno++; }  
"IF"            { return IF; }
"THEN"          { return THEN; }
"ELSE"          { return ELSE; }
"END"           { return END; }
"REPEAT"        { return REPEAT; }
"UNTIL"         { return UNTIL; }
"READ"          { return READ; }
"WRITE"         { return WRITE; }

[0-9]+          { yylval.num = atoi(yytext); return NUMERO; }
[a-zA-Z][a-zA-Z0-9]* { yylval.id = strdup(yytext); return IDENTIFICADOR; }

":="            { return ASSIGN; }
"<"             { return LT; }
">"             { return GT; }
"="             { return EQ; }
"<>"            { return NE; }
"<="            { return LE; }
">="            { return GE; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MUL; }
"/"             { return DIV; }

"("             { return LPAREN; }
")"             { return RPAREN; }

[ \t\n]+        ;  // Ignore whitespace
.               { return yytext[0]; }  

%%

int yywrap(void) {
    return 1;
}