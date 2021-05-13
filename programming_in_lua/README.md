# 序
《Lua程序设计》的阅读笔记

# Lua语言入门
## 一些词法规范
* Lua语言的保留字<br>
   ![lua_reserved_word](./pictures/lua_reserved_word.png)

* Lua语言中，连续语句之间的分隔符并不是必需的，可以用分号来进行分隔，如：
   * `a = 1 b = a * 2`和`a = 1; b = a * 2`是一样的

## 全局变量
* 全局变量无须声明即可使用(结果是nil)

## 类型和值
* 八种基本类型<br>
   ![lua_type](./pictures/lua_type.png)

## 独立解释器
独立解释器(Stand-alone interpreter, 由于源文件名为lua.c，所以也被称为lua.c；又由于可执行文件为lua，所以也被称为lua)是一个可以直接使用Lua语言的小程序。
* lua命令的完整参数形如下：`lua [options] [script [args]]`
   * -e参数允许我们直接在命令行中输入代码
   * -l参数用于加载库
   * -i参数用于在运行完其他命令行参数后进入交互式模式
