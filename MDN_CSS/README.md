# CSS -- 设计Web
[MDN Learn CSS](https://developer.mozilla.org/zh-CN/docs/Learn/CSS)的读书笔记。

# CSS排版

## [正常布局流](https://developer.mozilla.org/zh-CN/docs/Learn/CSS/CSS_layout/Normal_Flow)

### 默认情况下，元素时如何布局的？
* 块级元素
    * 每个块级元素会在上一个元素下面另起一行，它们会被设置好的margin分隔(重叠情况下，用更大的margin)
* 内联元素
    * 不会另起一行；只要在其父级块级元素的宽度内有足够的空间，它们与其他内联元素、相邻的文本内容（或者被包裹的）被安排在同一行。如果空间不够，溢出的文本或元素将移到新的一行

## [弹性盒子](https://developer.mozilla.org/zh-CN/docs/Learn/CSS/CSS_layout/Flexbox)
弹性盒子是一种用于按行或按列布局元素的一维布局方法，可以改变正常布局流。元素可以膨胀以填充额外的空间, 收缩以适应更小的空间。通过[一个简单的例子](./code/flexbox/flexbox0.html)可以观察弹性盒子的效果。

![flex_layout](./pictures/flex_layout.png)

### 行还是列
```css
flex-direction: column;
```
* 默认值时row，可以将主轴方向变成column

### 换行
```css
flex-wrap: wrap
```

### 水平和垂直对齐
```css
div {
  display: flex;
  align-items: center;
  justify-content: space-around;
}
```
* align-items
    * 控制flex item在交叉轴上的位置。
* justify-content
    * 控制flex item在主轴上的位置。