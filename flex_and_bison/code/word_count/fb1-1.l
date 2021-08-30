%{
int chars = 0;
int words = 0;
int lines = 0;
%}

%%

[a-zA-Z]+   { words++; chars += strlen(yytext); }
\n          { chars++; lines++; }
.           { chars++; }

%%

int main (int argc, char **argv)
{
   printf("before yylex()\n");
   yylex();
   printf("end yylex()\n");
   printf("%8d%8d%8d\n", lines, words, chars);
   return 0;
}
