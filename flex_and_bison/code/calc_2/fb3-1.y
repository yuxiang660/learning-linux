%{
#include <stdio.h>
#include <stdlib.h>
#include "fb3-1.h"
%}

// %union可以用来为符号值创建一个C语言的union联合类型
// 当声明中存在%union时，如果试图使用一个没有被赋予类型的符号值，bison将会报错
%union {
struct ast *a;
double d;
}

// 通过放置在尖括号（<>）中的联合类型的相应成员名字，来确定语法符号的值类型
%token <d> NUMBER
// 默认情况下所有的符号值都是整数
%token EOL

// 新的声明%type把值<a>赋给exp, factor和term
%type <a> exp factor term

%%
calclist: /* 空 */
   | calclist exp EOL {
      printf("= %4.4g\n", eval($2));
      treefree($2);
      printf("> ");
   }
   | calclist EOL { printf("> "); }
   ;

exp: factor
   | exp '+' factor { $$ = newast('+', $1, $3); }
   | exp '-' factor { $$ = newast('-', $1, $3); }
   ;

factor: term
   | factor '*' term { $$ = newast('*', $1, $3); }
   | factor '/' term { $$ = newast('/', $1, $3); }
   ;

term: NUMBER { $$ = newnum($1); }
   | '(' exp ')' { $$ = $2; }
   | '-' term { $$ = newast('M', $2, NULL); }
   ;

%%
