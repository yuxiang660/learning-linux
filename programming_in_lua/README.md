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

# 数值
* Lua5.3开始为数值模式提供了两种选择(以前版本的数值都以双精度浮点格式表示)
   * integer：64位整型
   * float：双精度浮点类型

## 数值常量
* 具有十进制小数或者指数的数值会被当作浮点型值，否则会被当作整型值。
* 由于整型值和浮点型值的类型都是`number`，所以它们是可以互相转换的：`1 == 1.0 --> true`
   * 如需要区分整型值和浮点型值：`math.type(3) --> integer`，`math.type(3.0) --> float`

### Lua数值的精度限制
* 用十进制表示的有限小数再用二进制表示时可能是无限小数
   * `12.7 - 20 + 7.3`用双精度表示也不是0，由于12.7和7.3的二进制表示不是有限小数
* 由于整型值和浮点型值的表示范围不同，因此当超过它们的表示范围时，整型值和浮点型值的算术运算会产生不同的结果
   ```lua
   math.maxinteger + 2   --> -9223372036854775807
   math.maxinteger + 2.0 --> 9.2233720368548e+18
   math.maxinteger + 0.0 --> 9.2233720368548e+18
   math.maxinteger + 2.0 == math.maxinteger + 1.0 --> true
   ```
* 浮点型能够表示的整数范围被精确地限制在[-2^53, 2^53]之间，超出这个范围后，我们则应该谨慎地思考所使用的表示方式
   * 小于2^53的所有整型值的表示与双精度浮点型值的表示一样，对于绝对值超过了这个值的整型值而言，在将其强制转换位浮点型值时可能导致精度的损失
   ```lua
   9007199254740991 + 0.0 == 9007199254740991   -> true
   9007199254740992 + 0.0 == 9007199254740992   -> true
   9007199254740993 + 0.0 == 9007199254740993   -> false
   ```
   * 整型转浮点型
      * `-3 + 0.0`
   * 浮点型整数转整型
      * `2^53 | 0`
      * `math.tointeger`函数会在输入参数无法转换位整型值时返回nil
         * `math.tointeger(-258.0) --> -258`
         * `math.tointeger(5.01) --> nil`

# 字符串
Lua语言中的字符串是不可变值(immutable value)，我们可以通过创建一个新字符串的方式来达到修改的目的。字符串的常见操作有：
```lua
a = "one string"
b - string.gsub(a, "one", "another") -- 改变字符串中的某些部分
print(b)    --> another string
print(#b)   --> 获取字符串b的长度，#操作符返回字符串占用的字节数，在某些编码中，这个值可能与字符串中的字符个数不同
print("hello ".."world")   --> 使用..来进行字符串的连接
```

## 字符串常量
* 长\多行字符串
   * 使用一对双方括号来声明长字符串常量
   ```lua
   data = [[
      hello
      world
   ]]
   print(data) --> hello
               --> world
   ```
   * 使用转义序列`\z`跳过其后的所有空白字符
   ```lua
   print("hello\z
    world")          --> helloworld
   ```

## 强制类型转换
* Lua不仅在算术操作时将字符串转换为数值，也会在需要字符串的地方将数值转换为字符串
   * `print(10 .. 20)   -> 1020`
* 建议通过函数`tonumber`显式地将字符串转换为有效数字
* 建议通过函数`tostring`显式地将数字转换成字符串

## 字符串标准库
* `string.len(s)`       - 返回字符串s的长度
* `string.rep(s,n)`     - 返回将字符串s重复n次的结果
* `string.reverse`      - 用于字符串翻转
* `string.lower(s)`     - 返回一份s的副本，将大写字母转成小写
* `string.sub(s, i, j)` - 从字符串s中提取第i个到第j个字符(包括第i个和第j个字符，字符串的第一个字符索引为1), -1代表最后一个字符
* `string.format`       - 用于进行字符串格式化
   ```lua
   string.format("x = %d  y = %d", 10, 20)      --> x = 10  y = 20
   string.format("x = %x", 200)                 --> x = c8
   string.format("x = 0x%X", 200)               --> x = 0xC8
   string.format("x = %f", 200)                 --> x = 200.000000
   tag, title = "h1", "a title"
   string.format("<%s>%s</%s>", tag, title, tag) --> <h1>a title</h1>
   string.format("pi = %.4f", math.pi)           --> pi = 3.1416
   d = 5; m = 11; y = 1990
   string.format("%02d/%02d/%04d", d, m , y)     --> 05/11/1990
   ```