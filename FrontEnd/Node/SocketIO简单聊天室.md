> 前注
> 开发基于 express 和 socket.io 库，如有描述不清地方请前往以下两个网址自行查看 API 文档
> [express](https://www.expressjs.com.cn/) [socketio](https://socket.io/get-started/)

<br>

### 文件结构与项目初始化

新建主文件夹 socket_demo，在文件夹下新建两个文件
`index.html` -> 客户端网页
`index.js` -> 用 socketio 搭建服务器

初始化项目：`npm init -y`  
安装 express 和 socketio：`npm i express@4 socket.io`

<br>

### index.js 文件

首先导入我们之后所用到的所有库，请注意以下几层关系：

1. app 是一个 express 服务器，我们对其进行基本的属性设置
2. server 是一个基于 http 搭建的本地服务器，在这里配置服务器运行时的事件监听
3. io 通过 socketio 内部封装好的服务器模块代理 server，相当于一个 websocket 服务器

```js
const express = require("express");
const app = express();
const http = require("http");
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);
```

<br>

紧接着为服务器注册主页，该主页即根目录下的 index.html 文件；  
`__dirname` 隶属 path 库，其返回当前项目的根目录路径

```js
app.get("/", (req, res) => {
  res.sendFile(__dirname + "/index.html");
});
```

<br>

我们让当前的 server 服务器监听 3000 端口，并且在服务器开启的一刻在后台输出对应信息

```js
server.listen(3000, () => {
  console.log("本服务器的端口打开在3000");
});
```

<br>

**核心 socketio 服务器事件响应代码解析**  
为便于理解下方大幅代码，故以下是几个关键 API 的通俗解释

1. io.on(event,method) 即当 io 服务器接收到请求后对该请求做出的回调函数，event 表示目前接收到的事件，method 表示后续的回到函数，该函数默认包含一个形参 socket，表示接收到的 socket 请求
2. io.emit(event,data) 根据聊天室原理，我们必然希望服务端接收到某个客户传来的信息后立刻向全部客户群发该消息，而该方法的作用就是抛出事件 event，并且附带数据 data 给所有的客户端，而对应的客户端只需要监听该 event 名称即可获得数据 data
3. socket.on 针对 socket 服务器的接收事件的响应以及回调函数

>

> 说人话就是，emit 即发出一个带特定标签的广播，而 on 在提供特定标签的名字并和 emit 发出广播比对成功后，即可取回 emit 传输的数据等内容

```js
// connection管理新客户端连入该websocket服务器后的所有事件
io.on("connection", (socket) => {
  console.log("新用户加入了！");

  // 服务端群发新用户加入房间的信息
  io.emit(
    "usercfg",
    JSON.stringify({
      type: 0,
      content: "用户加入房间！",
    })
  );

  // 服务端接收到客户发出的信息后做出的响应
  socket.on("chat message", (msg) => {
    io.emit("chat message", msg);
    console.log("客户说：" + msg);
  });

  // 客户端下线后服务端做出的响应
  socket.on("disconnect", () => {
    console.log("客户端下线了！");
    io.emit(
      "usercfg",
      JSON.stringify({
        type: 1,
        content: "用户退出房间！",
      })
    );
  });
});
```

JSON.stringify 来打包发送的数据后，在客户端使用 JSON.parse 进行解码，这是对于对象类型的数据才进行的操作，一般的数据类型直接传入即可使用！

<br>

**大体思路就是：**
新客户上线以及客户下线都会通过服务端的广播系统 io.eimt 通知给所有客户端；  
客户端接收 usercfg 事件后，判断传过来的 data 中的 type 数值为多少，从而确定做出对应设置

<br>

### index.html

这是客户端的主文件夹
首先我们要搭建好整个单页面系统，

```html
<!DOCTYPE html>
<html>
  <head>
    <title>Socket.IO chat</title>
    <style>
      body {
        margin: 0;
        padding-bottom: 3rem;
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto,
          Helvetica, Arial, sans-serif;
      }

      #form {
        background: rgba(0, 0, 0, 0.15);
        padding: 0.25rem;
        position: fixed;
        bottom: 0;
        left: 0;
        right: 0;
        display: flex;
        height: 3rem;
        box-sizing: border-box;
        backdrop-filter: blur(10px);
      }
      #input {
        border: none;
        padding: 0 1rem;
        flex-grow: 1;
        border-radius: 2rem;
        margin: 0.25rem;
      }
      #input:focus {
        outline: none;
      }
      #form > button {
        background: #333;
        border: none;
        padding: 0 1rem;
        margin: 0.25rem;
        border-radius: 3px;
        outline: none;
        color: #fff;
      }

      #messages {
        list-style-type: none;
        margin: 0;
        padding: 0;
      }
      #messages > li {
        padding: 0.5rem 1rem;
      }
      #messages > li:nth-child(odd) {
        background: #efefef;
      }
    </style>
  </head>
  <body>
    <ul id="messages"></ul>
    <form id="form" action="">
      <input id="input" autocomplete="off" /><button>Send</button>
    </form>
  </body>
</html>
```

<br>

导入 socketio 代码包，请按照以下代码直接复制粘贴到 index.html 中的 body 标签尾部，他这里的导入类似于导入根目录下的
`<script src="/socket.io/socket.io.js"></script>`

<br>

**客户端核心 js 代码**

1. 直接通过 io()构建一个 websocket 服务器
2. 此时给 form 添加 submit 事件，事件内容就是讲一个名称为 chat message 的时间包含数据 data 发送给 websocket 服务器
3. 接收事件时无论是 chat message 事件还是 usercfg 事件，都运用了向源 DOM 中添加新 DOM 的方法来模拟新消息的插入；
4. 因为 body 中有一个 ul 标签，那么只需要在该标签下面插入 li 标签即可实现均衡的垂直排列消息的效果了！

````js
<script>
      var socket = io();

      // 获取两个组件的DOM
      var form = document.getElementById("form");
      var input = document.getElementById("input");
      var messages = document.getElementById("messages");

      form.addEventListener("submit", (e) => {
        e.preventDefault();
        if (input.value) {
          socket.emit("chat message", input.value);
          input.value = "";
        }
      });

      // 接收正常消息时
      socket.on("chat message", (msg) => {
        console.log(msg);
        var item = document.createElement("li");
        item.textContent = msg;
        messages.appendChild(item);
        window.scrollTo(0, document.body.scrollHeight);
      });

      // 接收到名为usercfg消息的动作
      socket.on("usercfg", (msg) => {
        // 首先需要进行JSON解码
        var data = JSON.parse(msg);
        var item = document.createElement("li");
        item.textContent = data.content;

        // 判断得到的数据中type值是多少，以此来决定li标签中字体的颜色
        // 譬如绿色代表用户进入房间，而红色代表离开房间，这里的三目运算符判断type===0时为进入，更改为绿色
        item.style.color = data.type === 0 ? "green" : "red";
        messages.appendChild(item);
        window.scrollTo(0, document.body.scrollHeight);
      });
    </script>
    ```
````
