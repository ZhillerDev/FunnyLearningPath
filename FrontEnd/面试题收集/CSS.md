### CSS 盒模型

标准盒模型：content  
IE 盒模型（怪异盒模型）：content+padding+border

<br>

### BFC

> 块格式化上下文（Block Formatting Context，BFC）针对的是 CSS 渲染过程中的块级盒子

BFC 区域不会与 float box 重叠  
BFC 是页面上的一个独立容器，子元素不会影响到外面  
计算 BFC 的高度时，浮动元素也会参与计算

会生成 BFC 的元素

1. float 不为 none 的元素
2. position 为 fixed 和 absolute 的元素
3. display 为 inline-block、table-cell、table-caption，flex，inline-flex 的元素
4. overflow 不为 visible 的元素

<br>

### 块级元素与行内元素

块元素：独占一行，并且有自动填满父元素，可以设置 margin 和 pading 以及高度和宽度
行元素：不会独占一行，width 和 height 会失效，并且在垂直方向的 padding 和 margin 失效

<br>

### visibility opacity display

opacity=0 元素透明度为 0，隐藏，仍占据布局空间，可以响应点击事件  
visibiliy=hidden 隐藏元素，仍占据布局空间，但无法响应点击事件  
display=none 完全隐藏，不占据任何空间，可视为元素被删除

<br>

### 外边距折叠

多个相邻 `（兄弟或者父子关系）` 普通流的块元素垂直方向 `marigin` 会重叠

折叠结果：

1. 两个相邻的外边距都是正数时，折叠结果是它们两者之间较大的值。
2. 两个相邻的外边距都是负数时，折叠结果是两者绝对值的较大值。
3. 两个外边距一正一负时，折叠结果是两者的相加的和

<br>

### 清除浮动

方法一：使用带 clear 属性的空元素  
`<div class="clear"></div>`，在 CSS 中赋予 `.clear{clear:both;}`

方法二：使用 CSS 的 overflow 属性  
给浮动元素的容器添加 `overflow:hidden;` 或 `overflow:auto;` 可以清除浮动

方法三：给浮动的元素的容器添加浮动

方法四：使用邻接元素处理  
给浮动元素后面的元素添加 clear 属性

<br>

### CSS 选择器优先级

> 参考文献：https://juejin.cn/post/6844903709772611592#comment

`!important > id 选择器 > 类选择器 > 伪元素选择器 > 伪类选择器`

内联样式 > 内部样式 > 外部样式 > 浏览器用户 > 自定义样式 > 浏览器默认样式

同一元素引用了多个样式时，排在后面的样式属性的优先级高  
标签之间存在层级包含关系时，后代元素会继承祖先元素的样式，继承的样式的优先级比较低

<br>

###
