---
title: SVG入门到进阶
date: 2022-07-25 11:03:00
tags:

categories:
  - 前端
---

> 主要介绍 svg 相关基础知识，其余技能提升请前往”前端实战”板块进一步了解

<br>

### 基础语法

<br>

在使用之前，请安装以下 vscode 插件来加速我们的开发过程：  
`SVG` `svg preview`

**svg 框架**  
使用 svg 标签创建一个域，在里面执行绘制；  
可以设置域的宽高且默认单位为 px，还可以使用内联样式

```html
<svg width="300" height="300" style="border: 1px solid grey"></svg>
```

<br>

**基本图形绘制**  
矩形：`width` 设置长度 `height` 设置宽度  
圆形：`cx` 圆心 x 坐标，`cy` 圆形 y 坐标，r 半径长；  
椭圆形：`cx,cy` 对应圆心坐标，`rx,r`y 对应轴长度；  
直线：分别表示两个点的坐标；  
折线：`points` 里面设置每个落点的绝对坐标，之间使用逗号隔离开

`stroke` 设置线条描边颜色；  
`fill` 设置闭合图形填充颜色

```html
<body>
  <svg width="300" height="300" style="border: 1px solid red">
    <!-- 矩形 -->
    <rect x="0" y="0" width="20" height="20"></rect>

    <!-- 圆形 -->
    <circle cx="10" cy="40" r="10"></circle>

    <!-- 椭圆形 -->
    <ellipse cx="30" cy="80" rx="30" ry="20"></ellipse>

    <!-- 直线 -->
    <line x1="00" y1="150" x2="20" y2="120" stroke="blue"></line>

    <!-- 折线 -->
    <polyline
      points="0 200, 100 250, 100 200"
      stroke="red"
      fill="red"
    ></polyline>
  </svg>
</body>
```

![](https://cdn.jsdelivr.net/gh/zhiyiyi/zhipic/blog/imgs/css/svg-basic.png)

<br>

**path 路径绘制**  
可以借助 path 中的 d 属性，来绘制一串连贯的图线；  
d 中的常用属性：

1. M 起始点坐标，任何一个 d 属性开头都必须是 M，但一个 d 中可以有多个 M
2. L 轮廓坐标，即连接到下一点的位置
3. H 水平距离
4. V 垂直距离
5. Z/z 闭合路径

> 所有代号都必须有空格隔开，且不能包含任何符号  
> 大写代号表示绝对坐标，小写表示相对坐标；如 h 表示相对上一点的水平位置；

```html
<svg width="300" height="300" style="border: 1px solid grey">
  <path
    d="M 10 10 L 40 40 l 30 -30 M 10 40 l 100 0 "
    stroke="blue"
    fill="none"
  ></path>
</svg>
```

<br>

**更多属性介绍**  
stroke-opacity 设置描边透明度  
stroke-width 描边轮廓粗细  
stroke-dasharray 设置虚线样式，如 stroke-dasharray="20 10"表示 20px 实现后 10px 虚线，以此类推

stroke-linecap 设置线条守卫圆角样式，butt 不圆角，round 圆角；  
stroke-linejoin 设置线条拐角平滑，miter 尖角、round 圆角、bevel 平角

<br>

**text 标签**  
因为 svg 中的文本是以基线（即文本左下角）为定位基准的，故我们需要提前设置文本的 x 或者 y 值来让他正确的显示在 svg 画布中；

可以在 text 标签里面任意使用文本配置的 css 而不需要专门写在 style 中！

```html
<text y="24" x="200" font-size="24px" font-weight="bold">get it!</text>
```

text-anchor 设置水平对齐，有参数可选：start middle end；  
dominant-baseline 设置垂直对齐，有参数可选：text-after-edge middle text-before-edge

<br>

### 进阶语法

<br>

**defs**  
此标签内存放一些经常复用的元素，后续可以通过 url 调用

```html
<defs>
  <linearGradient id="Gradient01">
    <stop offset="20%" stop-color="#39F" />
    <stop offset="90%" stop-color="#F3F" />
  </linearGradient>
</defs>

<rect x="10" y="10" width="60" height="10" fill="url(#Gradient01)" />
```

<br>

**g**  
一个图案组，对 g 施加的属性将全部应用到他包裹的元素上；

```html
<g stroke="green" fill="white" stroke-width="5">
  <circle cx="25" cy="25" r="15" />
  <circle cx="40" cy="25" r="15" />
</g>
```

<br>

**use**  
在 SVG 文档内取得目标节点，并在别的地方复制它们；  
但可能被某些浏览器同源策略阻挡，且无法保证 CSS 克隆完全属性，不建议使用；

```html
<defs>
  <g id="Port">
    <circle style="fill:inherit" r="10" />
  </g>
</defs>
<use x="50" y="50" xlink:href="#Port" style="fill:blue" />
```

<br>

### 单文件 svg 与导入

<br>

**网页导入 svg 文件**  
可以直接使用 img 标签进行导入；  
`<img src="./1.svg" alt="">`

或者使用 embed，且 HTML5 以下版本不支持；  
注意 type 里面是固定的写法，这样才能导入 svg 格式文件  
`<embed src="./1.svg" type="image/svg+xml">`

object 导入和 embed 一致，但他无法使用脚本；  
`<object data="./1.svg" type="image/svg+xml"></object>`

> 最后一张办法就是直接在 html 的 body 里面使用 svg 标签编写，不推荐这种写法

<br>

**单 svg 文件**  
直接按照 svg 文件的格式来编写，用 svg 标签套住所需内容后将文件保存文.svg 类型的就可以了！  
其中配置的 title 和 desc 只是为了方便码农阅读，其实没什么用；

> 对于外部 svg 文件，需要设置 xmlns 头部，即以下代码所展示的固定格式书写！

```html
<svg
  xmlns="http://www.w3.org/2000/svg"
  width="200"
  height="200"
  style="border: 1px solid grey"
>
  <title>new svg</title>
  <desc>a description of svg</desc>
  <path d="M 10 10 h 60 v 60 l -60 -60 z" stroke="black" stroke-width="2" />
</svg>
```
