# 序
《精通CSS：高级Web标准解决方案》读书笔记

# 基础知识

## 设计代码的结构
### 标记简史
* HTML有意义的元素
    * h1,h2
    * ul, ol, dl
    * strong, em
    * blockquote, cite
    * abbr, acronym, code
    * fieldset, legend, label
    * caption, thread, tbody, tfoot
* ID和类名
    * ID用于标识页面上的特定元素(比如站点导航)，而且必须是唯一的
    * 一个ID名只能应用于页面上的一个元素，而同一个类名可以应用于页面上任意多个元素
* 为元素命名
    * 在分配ID和类名时，一定要尽可能保持名称与表现方式无关
    * 应该根据“它们是什么”来为元素命名，而不应该根据“它们的外观如何”来命名。例如不要给通知分配类名red，而是应该分配更有意义的名称，比如.warning或.notification
* ID还是类
    * 类应该应用于概念上相似的元素，这些元素可以出现在同一页面上的多个位置，而ID应该应用于不同的唯一的元素
* div和span
    * 为了将不必要的标记减到最少，应该只在没有现有元素能够实现区域分割的情况下使用div元素。例如，如果使用主导航列表，就不需要将它包围在div中：
    ```html
    <div class="nav">
        <ul>
            <li><a href="/css/">CSS</a></li>
            <li><a href="/js/">JavaScript</a></li>
        </ul>
    </div>
    <!-- 可以完全删除div，直接在列表上应用类 -->
    <ul class="nav">
        <li><a href="/css/">CSS</a></li>
        <li><a href="/js/">JavaScript</a></li>
    </ul>
    ```
    * div可以用来对块级元素进行分组，而span可以用来对行内元素进行分组或标识


