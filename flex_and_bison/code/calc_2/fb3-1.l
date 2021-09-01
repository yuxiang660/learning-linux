%option noyywrap yylineno
%{
#include "fb3-1.h"
#include "fb3-1.tab.h"
%}

EXP ([Ee][-+]?[0-9]+)

%%
"+" |
"-" |
"*" |
"/" |
"|" |
"(" |
")" { return yytext[0]; }
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { yylval.d = atof(yytext); return NUMBER; } // yylval现在时联合类型
\n  { return EOL; }
.   {printf("bad input character '%s' at line %d\n", yytext, yylineno);}
