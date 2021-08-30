# 序
《flex与bison》读书笔记

# Flex和Bison简介
## 我们的第一个Flex程序
```
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

main (int argc, char **argv)
{
   yylex();
   printf("%8d%8d%8d\n", lines, words, chars);
}
```
* flex程序包含三个部分(通过`%%`分割)：
   * 声明和选项设置
      * `%{`和`}%`之间的代码会被原样照抄到生成的C文件的开头部分
   * 一系列的模式和动作
      * 模式(前面不能有空格) + {需要执行的C代码}
      * 在任意一个flex的动作中，变量`yytext`总是被设为指向本次匹配的输入文本
   * 会被拷贝到生成的词法分析器里面的C代码
      * 主程序，主要负责调用flex提供的词法分析例程`yylex()`

