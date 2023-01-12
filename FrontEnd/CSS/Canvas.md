### 基础中的基础

#### 最简单的 canvas

canvas 必须要有长宽  
获取其 DOM 后获取上下文，并通过上下文在指定的画布内作画

```html
<body>
  <canvas
    id="cav"
    width="600"
    height="400"
    style="background-color: ghostwhite"
  ></canvas>

  <script>
    var cav = document.getElementById("cav");
    var ctx = cav.getContext("2d");
    ctx.fillRect(50, 50, 100, 200);
  </script>
</body>
```

<br>

宽高区别：

canvas 标签内部自带两个属性 width 和 height，这是定义画布大小的，单位为 px 但是可以省略；  
而使用 css 控制 canvas 的宽高是不改变画布大小的！所以可能会导致图像拉伸而失真，且这里的宽高单位 px 不可省略；

<br>

#### 浏览器支持

如果浏览器支持 canvas，就会直接渲染画布内容，否则就会渲染 canvas 标签内部包裹的内容

所以你不能把 canvas 当成一个自闭合标签来用！

```html
<canvas id="canvas" height="600" width="700">
  你的浏览器太烂了，快点升级去！
</canvas>
```

<br>

#### 绘制三角形

moveto 先移动画笔到指定位置，在使用 lineto 画线段，从而组成一个完整的三角形；  
画完后记得闭合路径，否则路径是开的

```js
function draw() {
  var canvas = document.getElementById("tutorial");
  if (!canvas.getContext) return;
  var ctx = canvas.getContext("2d");
  ctx.beginPath();
  ctx.moveTo(50, 50);
  ctx.lineTo(200, 50);
  ctx.lineTo(200, 200);
  ctx.closePath(); //虽然我们只绘制了两条线段，但是closePath会closePath，仍然是一个3角形
  ctx.stroke(); //描边。stroke不会自动closePath()
}
draw();
```

<br>

####
