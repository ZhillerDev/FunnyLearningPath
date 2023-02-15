### Pug 模板引擎

#### 安装与基本语法

热门的一款模板渲染引擎，下面将展示基本语法

首先你要安装： `npm i pug`

下面是 `index.pug` 文件即含义解析
doctype html head body 这四个玩意是固定的，cv 过来就好；  
#{title} 表示引用变量 title，前提是你赋予了 title 这个变量值！

> 变量赋值将在后面的 express 结合中有写出

```pug
doctype
html(lang='en')
    head
        meta(charset='utf-8')
        title title
    body
        h1 #{title}
```

使用 extends 继承其他模板，使用 block 指定一个区域，并在子模板中进行填充

```pug
//- 这是父模板 index.pug
doctype
html(lang='en')
    head
        meta(charset='utf-8')
        title title
    body
        block content

//- 这是继承的模板 error.pug
extends index

block content
    h1 message
    p helloworld
    p #{title}
```

> end 语法大概就这么多，具体的自己去官网看：https://www.npmjs.com/package/pug

<br>

#### 作为 express 渲染引擎

在 app.js 文件添加如下图所示代码（具体含义看注释）
之后就可以在根目录下的 view 文件夹内添加模板文件 pug 来进行模板渲染了！

```js
var express = require("express");
var app = express();

// 类似键值对，views是固定值，表示存储模板的文件夹别名，后面的就是路径
app.set("views", path.join(__dirname, "view"));

// view engine也是固定值，表示设置渲染引擎，pug表示使用它作为模板渲染引擎
app.set("view engine", "pug");
```

路由内或者其他状况下，直接使用 `res.render()` 指定欲渲染的模板名字即可  
参数一为模板名称，比如有文件 view/index.pug，那么参数一就写 index  
参数二为附加属性，代码表示设置模板中的变量 title 的值为 helloworld

```js
res.render("index", { title: "helloworld" });
```

<br>

### mysql2 数据库访问

下载 `npm i mysql2`  
npm 包解释官网 https://www.npmjs.com/package/mysql2

#### 使用 pool 方法链接数据库

普通方法在链接并查询一次后会自动关闭数据库，造成频繁开闭损耗，而创建池子 pool 的方法可以保持数据库开启的状态，并且只在需要查询数据库的时候进行调用！
`createPool` 创建数据库池，里面的五个经典参数不解释。。。

```js
// 代码清单 database.js

const mysql = require("mysql2");

const pool = mysql.createPool({
  host: "localhost",
  port: 3306,
  user: "root",
  password: "123456",
  database: "test",
});
```

接着在以上代码下方继续添加以下 query 方法，以后即可使用此封装好的方法进行数据查询  
query 是一个方法声明，箭头函数内有三个参数，分别是：  
`sql` 语句，CRUD 语句都可以  
`params` 附加的语句  
`callback` 回调函数，接收查询得到的数据或者插入得到的反馈结果

```js
// 续 database.js 代码清单

let query = (sql, params, callback) => {
  // 将池子连接到数据库
  pool.getConnection((err, connection) => {
    // 链接失败直接return，返回错误细心
    if (err) {
      return callback(err);
    }
    // 链接成功就开始查询
    connection.query(sql, params, (err, res) => {
      // 查询到解雇后释放连接
      connection.release();
      console.log(sql, err, res);
      if (err) {
        return callback(err);
      }
      // 如果很完美，啥问题都没有，那就返回res结果
      return callback(err, res);
    });
  });
};

// 最后别忘了暴露变量
module.exports = query;
```

<br>

### http-errors

快速返回一个 status

```js
// 引入插件
var createError = require("http-errors");

router.get("/api", (req, res) => {
  // 检测到对应query后抛出错误
  if (req.query.username === "zhiyiyi") {
    createError(404, "message error");
  } else {
    res.send({
      id: 1,
      username: "zhiyiyi",
      password: "12345",
    });
  }
});
```

<br>

### nodemon

一般的，我们执行 node xxx.js 后，只会执行一次；  
但是我们搭建 express 服务器后需要频繁修改文件，所以 nodemon 作用就是使得 node 自动化

安装： `npm i nodemon`

修改 package.json 中的启动参数（不改也行）

```json
"scripts": {
    "start": "nodemon ./xxx.js"
  },
```

> 立即运行，立即看到效果

<br>
