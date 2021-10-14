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
* 目的
   * policies可以在型别安全的前提下扩增host class的功能
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
   * Host Class
      * 定义了policy class后，需要一个host class用以使用policy。采用template+继承的方式：
      ```cpp
      // 也可以是多继承
      template <class CreationPolicy>
      class WidgetManager: public CreationPolicy
      {
      };
      typedef WidgetManager<OpNewCreator<Widget>> MyWidgetMgr;

      // 使用"template template参数"优化 WidgetManager
      template <template <class> class CreationPolicy>
      class WidgetManager : public CreationPolicy<Widget>
      {
      };
      typedef WidgetManager<OpNewCreator> MyWidgetMgr;
      ```
   * [完整例子](./code/creator/main.cpp)

## Policy Classes的析构函数
* 如果为policy定义一个虚析构函数，会妨碍policy的静态连结特性，会影响执行效率(虚表的额外开销)
* 折中的办法是：将policy的析构函数放到protected层级，只允许派生类销毁此policy对象，阻止外界采用delete方式销毁

## 将一个Class分解为一堆Polices
* 关键问题
   * 如何将class正确分解为policies
* 准则
   * 将参与class行为的设计鉴别出来并命名之
      * 任何事情只要能以一种以上的方法解决，都应该被分析出来，并从class中移出来成为policy
      * 淹没于class设计中的constraints(约束条件)就像淹没于代码中的魔术常数一样不好
   * 找出正交的policies

## 总结
* Policies机制
   * 由templates和多重继承组成
* host class
   * 一个class如果使用了policies，我们称其为host class
   * 拥有多个template参数(通常是"template template参数")的class template
   * 运作起来像是一个聚合了数个policies的容器
* 由于采用"public"继承
   * policy得以通过host class提供追加机能
   * host classes也能运用"policy提供的选择性机能"作出更丰富的功能
      * 如果某个选择性机能不存在，host class还是可以成功编译，前提是该选择性机能未被真正使用

# 技术

## 编译期(Compile-Time) Assertions
* 编译期的Assertion的方法是：
   * 当出现不期望的情况时，利用宏和模板产生非法的代码，导致编译出错
* 这多少有点不利于维护，个人不太喜欢这样的做法
* 可参见，C++标准库中的`static_assert`

## 模板偏特化(Partial Template Specialization)
* 偏特化只能用于模板类，不能用于函数(无论是成员函数或非成员函数)
   * 可以全特化成员函数，但不能偏特化
   * 不能偏特化普通函数，但可以重载overloading，例如：
   ```cpp
   template<class T, class U> T Fun(U obj); // primary template
   // template<class U> void Fun<void, U>(U obj); // illegal partial specialization
   template <class T> T Fun(Window obj); // legal overloading
   ```
* [例子](./code/template_partial_spec/main.cpp)

## 局部类(Local Classes)
* 定义在函数中的类，有不能使用static的限制，感觉也有点奇怪
* 可以用函数外的“template class”实现同样的功能

## 型别选择(Type Selection)
* 使用"traits class template"可以实现编译期间的型别选择，例如
   ```cpp
   template<typename T, bool isPolymorphic>
   struct NiftyContainerValueTraits
   {
      typedef T* ValueType;
   };

   template<typename T>
   struct NiftyContainerValueTraits<T, false>
   {
      typedef T ValueType;
   };

   // then, we can use isPolymorphic to select the value type in NiftyContainer
   template <typename T, bool isPolymorphic>
   class NiftyContainer
   {
      typedef NiftyContainerValueTraits<T, isPolymorphic> Traits;
      typedef typename Traits::ValueType ValueType;
   };
   ```
* 也可以直接设计一个通用类型选择器，用户可自行指定可选择的类型
   ```cpp
   template <bool flag, typename T, typename U>
   struct Select
   {
      typedef T Result;
   };
   template <typename T, typename U>
   struct Select<false, T, U>
   {
      typedef U Result;
   };

   template <typename T, bool isPolymorphic>
   class NiftyContainer
   {
      typedef Select<isPolymorphic, T*, T>::Result ValueType;
   };
   ```

## Type Traits

