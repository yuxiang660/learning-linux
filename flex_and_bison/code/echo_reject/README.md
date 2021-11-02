# Intro
Explain the usage of `ECHO` and `REJECT` of flex. For details, refer to https://codedost.com/flex/flex-programs/flex-program-check-use-echo-reject-statements/

# Description
* `REJECT`
   * directs  the  scanner  to proceed on to the "second best" rule which matched the  input  (or a prefix  of  the  input).
* 例子
   * 输出：
      ```
      yytext = 0x0x7fffd76588de, yyleng = 4, contains both uppercase and lowercase letters = Just
      yytext = 0x0x7fffd76588de, yyleng = 3, contains both uppercase and lowercase letters = Jus
      yytext = 0x0x7fffd76588de, yyleng = 2, contains both uppercase and lowercase letters = Ju
      yytext = 0x0x7fffd76588de, yyleng = 1, contains both uppercase and lowercase letters = J
      yytext = 0x0x7fffd76588de, yyleng = 1, contains mixed letters = J
      yytext = 0x0x7fffd76588df, yyleng = 3, contains only lowercase letters = ust
      yytext = 0x0x7fffd76588e2, yyleng = 1, contains mixed letters =  
      yytext = 0x0x7fffd76588e3, yyleng = 4, contains only lowercase letters = code
      ```
   * 在这个例子中，`[a-zA-Z]+`是`Just`的最长匹配项，所以先进入`contains both uppercase and lowercase letters`
   * `Just`输入被REJECT后，`yytext`和`yyleng`被重新设置，`Jus`被重新输入开始匹配

