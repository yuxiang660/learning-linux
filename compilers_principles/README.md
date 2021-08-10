# 简介
《编译原理》(龙书）的读书笔记

# 引论

## 一个编译器的结构
* 分析analysis - 前端
   * 把源程序分解成多个组成要素
   * 使用此结构创建该源程序的一个中间表示
   * 把源程序存放在一个符号表(symbol table)的数据结构中
   * 符号表将和中间表示形式一起传送给综合部分
* 综合synthesis - 后端
   * 根据中间表示和符号表中的信息来构造用户期待的目标程序
* 详细步骤如下<br>
   ![compile_phase](./pictures/compile_phase.png)

### 词法分析
* 对于每个词素，词法分析器产生`<token-name, attribute-value>`词法单元(token)作为输出
   * token-name是一个由语法分析步骤使用的抽象符号
   * attribute-value指向符号表中关于这个词法单元的条目
* `position = initial + rate * 60`语句的词法分析过程
   * `position`词素
      * 被映射成词法单元<id, 1>
         * id表示标识符
         * 1指向符号表中position对应的条目，一个标识符对应的符号表条目存放该标识符有关的信息，比如名字和类型等
   * `=`词素
      * 被映射成<=>
      * 这个词法单元不需要属性，忽略第二个分量
   * `initial`词素
      * 被映射成<id, 2>
   * `+`词素
      * 被映射成<+>
   * `rate`词素
      * 被映射成<id, 3>
   * `*`词素
      * 被映射成<*>
   * `60`词素
      * 被映射成<60>
   * 因此，最终上述赋值语句被表示成：`<id, 1> <=> <id, 2> <+> <id, 3> <*> <60>`<br>
     ![compile_phase_example](./pictures/compile_phase_example.png)