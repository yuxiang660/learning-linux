/* Disable */
%option noyywrap

%{
#include <stdio.h>
%}

%%

[a-z]+ {
   printf("yytext = 0x%p, yyleng = %d, contains only lowercase letters = ", yytext, yyleng);
   ECHO;
   printf("\n");
   }

[a-zA-Z]+ {
   printf("yytext = 0x%p, yyleng = %d, contains both uppercase and lowercase letters = ", yytext, yyleng);
   ECHO;
   printf("\n");
   REJECT;
   }

. {
   printf("yytext = 0x%p, yyleng = %d, contains mixed letters = ", yytext, yyleng);
   ECHO;
   printf("\n");
   }

%%

void main()
{
   char tstr[] = "Just code";
   yy_scan_buffer(tstr, sizeof(tstr)+1);
   yylex();
   printf("\n");
}
