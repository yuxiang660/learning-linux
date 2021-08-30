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
      * 模式如果用引号引起，则告诉flex使用引号内的文本的原义，而不是把他们解释成正则表达式，如："+"代表加号
      * 在任意一个flex的动作中，变量`yytext`总是被设为指向本次匹配的输入文本
   * 会被拷贝到生成的词法分析器里面的C代码
      * 主程序，主要负责调用flex提供的词法分析例程`yylex()`

## 让Flex和Bison协同工作
* 词法分析器在动作中返回与不返回有什么区别？
   * 不返回：`"+" { printf("PLUS\n"); }`，参加[例子](./code/calc/fb1-3.l)
   * 返回：`"+"   { return ADD; }`，参加[例子](./code/calc/fb1-4.l)
      * 如果动作有返回，词法分析器会在下一次`yylex()`调用时继续
      * 如果动作没有返回，词法分析器将会立即继续进行

### 记号编号和记号值
* 当flex词法分析器返回一个记号流时，每个记号实际上有两个组成部分
   * 记号编号
   * 记号值


