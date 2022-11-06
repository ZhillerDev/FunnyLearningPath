---
title: WebSocket原理笔记
date: 2022-07-17 08:54:46
tags:

categories:
  - 前端
---

### 简要了解 websocket

- websocket 是基于 HTML5 的一个新协议，它的底层仍然使用 tcp
- 不同于 http/1.0 的短连接和 http/1.1 的长链接，websocket 完美实现全双工通信，无需频繁的断开重连
- websocket 是持久化协议
- 更好的二进制支持
- 较少的控制开销。websocket 客户端和服务器之间交换的数据报头部很小
- 支持扩展。可以实现自定义子协议
- 本质上是一种计算机网络应用层协议
- 没有同源限制，客户端可以与任意服务器通信

<br>

### websocket 协议定义

**一般的 http 协议**  
http 协议和 https 协议，访问的网址形式为 `http://xxx/`

**websocket 协议**  
他和 http 协议不同，使用 ws 或者 wss 替换掉原来的 http 和 https，表示使用 websocket 协议来进行客户端和服务端之间通信  
`ws` 不带安全套接字层 SSL 的 websocket 协议  
`wss` 带安全套接字层 SSL

> websocket 协议的网址格式为 `ws://xxx/` 或者 `wss://xxx/`

<br>

### websocket 链接流程

> 前提注意：
> websocket 双向通信，模拟 socket 协议，而 http 协议是单向的
> websocket 需要浏览器和服务器共同参与握手，而 http 协议只是浏览器发起的链接
> http/2 有服务器推送功能，但只能推送静态资源

<br>

完整 websocket 连接过程：

1. 浏览器发起 http 请求，3 次握手后建立 tcp 链接
2. 服务器收到浏览器请求，并以 http 协议反馈
3. 浏览器得到 ack 数据报后，即可用 tcp 信道和服务器全双工通信

<br>

### websocket 常见问题

**如何判断在线离线？** 第一次客户端发送带有时间戳的数据，服务端记下该时间后，在下一次客户端发送的数据中取出另一个时间戳，俩时间戳之差若大于设定的毫秒值，则表示离线
**解决离线问题** 一种时修改 nginx 配置信息，一种是发送心跳包

**心跳包**  
机制：隔一段时间客户端发送一个带包头的空包来告诉服务端自己还活着

1. TCP 中的选项 `SO_KEEPALIVE` 可以检测心跳包频率
2. 心跳包一般来说都是在逻辑层发送 `空的echo包` 来实现的

> **心跳包检测保活的步骤**
> 客户端每隔一个时间发送心跳包给服务端
> 客户端启动超时定时器
> 客户端理应在超时定时器范围内接收到服务端的响应包
> 若接收到，取消超时定时器；若接收不到，说明断开链接

<br>

### 简单实践案例

在新文件夹下使用 npm 预先安装如下插件：
`express` 简单构建本地服务器
`http` http 请求及相关服务构建
`ws` 封装集成易于使用的 websocket 插件

<br>

**服务端文件 server.js 代码清单**

1. 首先构建 express 服务器，并使用 http 插件指定服务器为 express 实例
2. WebSocket.Server 指定 websocket 服务器的端口号
3. 第一个 wss.on 中的 connection 表示这是链接阶段的事件，后面跟着一个函数表示对应事件的处理
4. ws.on 表示服务端开始接收客户端发来的信息
5. ws.send 表示服务端自主向客户端发送信息

```js
// require导入所有插件，并初始化express服务器
var app = require("express")();
var server = require("http").Server(app);
var WebSocket = require("ws");

// 构建websocket服务器，端口为8080
var wss = new WebSocket.Server({ port: 8080 });

// wss服务器打开后接受发送数据处理
wss.on("connection", function connection(ws) {
  console.log("服务器链接构建！");

  ws.on("message", function incoming(message) {
    console.log("服务器接收到客户端信息为： %s", message);
  });

  ws.send("my_server");
});

// 监听时延
app.listen(3000);
```

<br>

**客户端文件 client.html 代码清单**

1. new WebSocket 这是 JS 自带的构建 websocket 对象的方法，这里按照规定格式指定了 websocket 服务器 url
2. ws.onopen 表示客户端启动时需要做的事
3. ws.onmessage 表示客户端接收到服务端发来的数据时要做的事

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Document</title>
  </head>

  <body>
    <script>
      var ws = new WebSocket("ws://localhost:8080");
      ws.onopen = function () {
        console.log("客户端启动");
        ws.send("my_client");
      };
      ws.onmessage = function (e) {
        console.log("客户端收到的信息" + e.data);
      };
    </script>
  </body>
</html>
```

> 根目录下保存这两个文件，然后原地执行 `node server.js` 运行服务器，之后再双击打开 `client.html` 文件即可！

<br>

### websocket 数据帧传输

websocket 客户端和服务端之间是使用数据帧 frame 进行数据传输的！  
发送端将数据切分为多个数据帧，分别逐一发送给对方；  
接收端将接收到的帧逐一拼接后读取完整信息；

**其他传输易混易错事项**

1. 接收方只有接收到帧中包含 `FIN=1` 的标识后，才会逐一将前面的所有分块帧接合在一起
2. `opcode` 表示当前帧的状态，其中的参数含义可以查阅有关资料得出

<br>

### Sec-WebSocket-Key

他也是包含在帧首部的一个属性，下面主要介绍`Sec-WebSocket-Key`和`Sec-WebSocket-Accept` 两者的差别和作用

1. 避免服务端收到非法的 websocket 连接
2. 确保 tcp 握手后会执行 websocket 链接，而不是忽略它
3. 协助发送 ajax 请求
4. 可以防止反向代理返回错误的数据
