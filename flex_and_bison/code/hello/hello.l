%option noyywrap

%%

%%

void main()
{
   char tstr[] = "Hello";
   yy_scan_buffer(tstr, sizeof(tstr)+1);
   yylex();
   printf("\n");
}
