---
title: WebGL原理与实战演示
date: 2022-08-11 09:22:45
tags:

categories:
  - CSS
---

### WebGL 理论术语

#### GLSL

`GLSL` 是 OpenGL 着色语言（OpenGL Shading Language）的简称；  
它运行在 GPU 内部，代替固定渲染管线的一部分；  
他有三个着色器：Vertex Shader（顶点着色器）和 Fragment（片断着色器），或 Geometry Shader（几何着色器）

<br>

#### WebGL 中的 GLSL

顶点着色器： 计算顶点的位置，即提供顶点在裁剪空间中的坐标值  
片段着色器： 计算图元的颜色值，我们可以将片段着色器大致理解成网页中的像素  
着色器获取数据的四种方式： 属性和缓冲、全局变量、纹理、可变量

<br>

#### WebGL 工作流程

示例：画一个三角形

1. 先调用三次顶点着色器计算出三角形的 3 个顶点在裁剪空间坐标系中的对应位置，并通过变量 gl_Position 保存在 GPU 中；
2. 后调用片元着色器完成每个顶点颜色值的计算，并通过变量 gl_FragColor 将对应的颜色值存储在 GPU 中；
3. 光栅化三角形；

> 枯燥的完整流程解释：  
> 顶点处理：转换顶点坐标，变成照相机下的坐标系
> 图元组装和裁剪：顶点注意链接形成图元，超出视窗之外的进行裁剪
> 光栅化：将几何图形变为二维图像，分配颜色值和深度到指定区域
> 片元处理：通过片元着色器来计算片元的最终颜色和深度值

<br>

#### SVG 与 Canvas2d 技术比较

物品数量越多，svg 渲染时间越久，而 canvas2d 时间增幅不明显  
图形面积越大，canvas2d 渲染时间越久，而 svg 时间增幅不明显

<br>

### WebGL 简单绘图

> WebGL 进行三维绘制仍旧十分繁复，而现成的 three.js 库能很好地代替原生的冗长代码，故目前不推荐初学者直接上手使用 webgl 原生代码编写

#### 基本示例

创建画布：

```html
<canvas id="webgl" width="500" height="300" style="border: 1px solid;"></canvas>
```

获取 canvas 对象

```js
const canvas = document.getElementById("webgl");
const gl = canvas.getContext("webgl");
```

编写顶点着色器和片元着色器

```js
// 这是一个模板代码片段
const vertexShaderSource = `
  // 声明一个属性变量 a
  attribute vec3 a;
  void main() {
    // 顶点在作色器处理后的位置信息
    gl_Position = vec4(a, 1.0);
  }
`;

const fragmentShaderSource = `
  void main() {
      // 片段颜色
      gl_FragColor = vec4(0.1, 0.7, 0.3, 1.0);
    }
`;
```

初始化并使用着色器

```js
function initShader(gl, vertexSource, fragmentSource) {
  const vertexShader = gl.createShader(gl.VERTEX_SHADER);
  const fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
  // 将着色器源码附加到着色器上
  gl.shaderSource(vertexShader, vertexSource);
  gl.shaderSource(fragmentShader, fragmentSource);

  // 编译着色器
  gl.compileShader(vertexShader);
  gl.compileShader(fragmentShader);

  // 创建一个程序对象
  const program = gl.createProgram();
  // 将编译好的着色器附加到程序对象上
  gl.attachShader(program, vertexShader);
  gl.attachShader(program, fragmentShader);
  // 链接程序对象
  gl.linkProgram(program);
  // WebGL引擎使用该程序对象
  gl.useProgram(program);

  return program;
}

const program = initShader(gl, vertexShaderSource, fragmentShaderSource);
```

将顶点数据传输到顶点着色器

```js
function sendDataToSharder(gl, data) {
  // 将顶点数据写入缓存区，并将数据传递给顶点着色器
  var vertexBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW);
  var vertexAttribLocation = gl.getAttribLocation(program, "a");
  gl.vertexAttribPointer(vertexAttribLocation, 3, gl.FLOAT, false, 0, 0);
  // 设置通过顶点着色器将缓冲的输入数据转换为一系列顶点数组
  gl.enableVertexAttribArray(vertexAttribLocation);
}

// 定义顶点数据，这里定义了三角形的三个顶点坐标，以中心点为坐标原点，z 轴为 0
var data = [0.0, 0.5, 0.0, -0.5, -0.5, 0.0, 0.5, -0.5, 0.0];
sendDataToSharder(gl, data);
```

最后一步：绘制图像！！！
`gl.drawArrays(gl.TRIANGLES, 0, 3);`

<br>
