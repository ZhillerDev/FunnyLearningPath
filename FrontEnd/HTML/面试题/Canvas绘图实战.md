---
title: Canvas绘图实战
date: 2022-07-25 07:55:20
tags:

categories:
  - 前端实战
---

> 迅速掌握 canvas 绘图方法，光速出图，全文无废话，精简至上

<br>

### 点线面基础绘图

<br>

**canvas 基础架构**  
canvas 首先必须通过 js 获取其 dom，然后才能执行绘图操作；  
下面的模板为获取 canvas 对象，并以 2d 上下文的形式进行绘图；  
接下来的内容都依据该模板来进行；

```html
<body>
  <canvas id="cav" width="500" height="500"></canvas>

  <script>
    var cav = document.getElementById("cav");
    if (cav.getContext) {
      var context = cav.getContext("2d");
    }
  </script>
</body>
```

<br>

**基本形状**  
moveTo(x, y) 设置绘画起始点  
lineTo(x, y) 画一条从上一点到指定点的直线

strokeRect(x, y, width, height) 绘制一个矩形的边框  
fillRect(x, y, width, height) 绘制一个填充的矩形  
clearRect(x, y, width, height) 清除指定矩形区域，让清除部分完全透明

<br>

**多次画图**  
`beginPath()` 开始一次画图；
`stroke()` 把图像渲染出来
`closePath()` 闭合图形，结束一次绘画

若我们只想一笔画，直接在最后 stroke()就完事；  
如果不是，那么必须要使用 beginPath()和 closePath()包裹每一次绘画；

```js
var cav = document.getElementById("cav");
if (cav.getContext) {
  var context = cav.getContext("2d");

  // 这是一次绘画的开始
  context.beginPath();
  ctx.moveTo(50, 50);
  ctx.lineTo(200, 200);
  context.stroke();
}
```

<br>

**圆弧与圆**  
绘制圆弧的代码：arc(x, y, radius, startAngle, endAngle, anticlockwise)  
x 和 Y 为圆心的坐标，radius 为半径，startAngle 为圆弧或圆的开始位置，endAngle 为圆弧或圆的结束位置  
anticlockwise 是绘制的方向（不写默认为 false，从顺时针方向）

> arc 中角度使用的是弧度制，转换公式：弧度=(Math.PI/180)\*角度

<br>

### 进阶图形

<br>
