# 序
《计算机程序的构造和解释》读书笔记

* 本书中三类需要关注的对象：
    * 人的大脑
    * 计算机程序的集合
    * 计算机本身

## 环境搭建：VSCode + Racket
* 参考文档
    * https://zhuanlan.zhihu.com/p/34313034
    * https://chee5e.space/sicp-vscode-setup/
    * https://docs.racket-lang.org/sicp-manual/Installation.html
* 安装Racket
    ```sh
    sudo add-apt-repository ppa:plt/racket -y
    sudo apt-get install racket -y
    ```
    * 安装过程需要一定时间，完成后可打开如下图形界面：
    ![drRacket](./pictures/drRacket.png)
    * 安装完成后检查`racket`和`raco`命令是否能在命令行中正确执行
* 在VSCode中安装Magic Racket插件，并安装racket-langserver
    * `raco pkg install racket-langserver`
* 执行[hello程序](./code/hello/Makefile)
* 通过drRacket图形界面，安装 SICP package，以支持`#lang sicp`
    * 参考：https://docs.racket-lang.org/sicp-manual/Installation.html


# 构造过程抽象
* 心智活动的三个方面
    * 组合
        * 简单认识组合为一个复合认识
    * 类比
        * 将两个认识放在一起对照，得到相互关系的认识
    * 抽象
        * 将有关认识与其他认识隔离

## 程序设计的基本元素
为了将简单的认识组合起来形成更复杂的认识，每种语言都提供了三种机制：
* 基本表达式
    * 用于表示语言所关心的最简单的个体
* 组合的方法
    * 通过它们可以从较简单的东西出发构造出复合的元素
* 抽象的方法
    * 通过它们可以为复合对象命名，并将它们当作单元去操作

程序设计中，需要处理两类要素：
* 数据
    * 是一种我们希望去操作的“东西”
* 过程
    * 是有关操作这些数据的规则的描述

### 表达式
前缀表达式的有点：
* 适用于可能带有任意个实参的过程
* 可以直接扩充，允许出现组合式嵌套的情况

### 命名和环境
命名提供了一种通过名字去使用计算对象的方式，名字标识符称为变量。



