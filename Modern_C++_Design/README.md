# 序
《Modern C++ Design》的读书笔记

# 基于Policy的Class设计
## 软件设计的多样性
* 如何在程序中包装既富有弹性又有优良设计的组件？
* 如何让使用者自行装配这些组件？
* 如何在合理大小的代码中对抗邪恶的多样性？

## 全功能型(Do-It-All)接口的失败
* 一个良好的设计应该在编译期强制表现出大部分constraints(约束条件、规范)
   * 帮助使用者精巧完成设计，以实现使用者自己的constraints，而不是实现预先定义好的constraints

## 多重继承(Multiple Inheritance)是救世主？
* 产生的问题
   * 技术(Mechanics)
      * 需要小心协调，才能工作
   * 型别信息(Type information)
      * Based classes没有足够的型别信息来完成其工作
   * 状态处理
      * Based classes需要操作相同的state
* 虽然本质上是组合，但是多重继承无法单独解决设计时的**多样性选择**

## Templates带来曙光
* template是一种很适合“组合各种行为”的机制，主要因为他们是“依赖使用者提供的型别信息”并且“在编译期才产生”的代码
* 两大特点
   * 编译期特性
   * “可互相结合”特性(偏特化)
* Template的限制
   * 对于普通Class，只能特化其成员函数，不能特化其数据成员
   * 对于类的成员函数，不支持partial specialization，参加[例子](./code/template_partial_spec/main.cpp)
   * 只能提供一份template的缺省值，不能提供多份缺省值
* 多重继承和Template对比
   * template拥有了多重继承缺少的技术，拥有更多型别信息
   * 多重继承更容易扩展，可以写无限的base class，但是template成员函数只能有一份缺省版本

## Policies和Policy Classes
* Policy用来定义一个class或class template的接口，有下列项目之一或全部组成：
   * 内隐型别定义(inner type definition)
   * 成员函数
   * 成员变量
* 例子
   * Creator Policy
      * 定义了一个policy用以生成对象
      * 是一个带有型别T的class template
      * 三个[policy classes](./code/creator/main.cpp)
         * 虽然他们的接口有所不同，但都定义了`Create()`，所以符合"Creator policy"





