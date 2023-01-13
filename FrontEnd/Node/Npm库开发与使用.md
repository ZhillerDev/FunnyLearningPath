### 开发并上传第一个库

#### 账号注册与登陆

官网注册一个自己的账户，务必记住用户名，之后链接到 npm 需要使用

安装 nodejs，他会自动帮我们把 npm 添加到系统变量，打开命令行直接就可以用

开启命令行，输入 `npm login`  
之后会让你依次输入：用户名、密码、邮箱（若是第一次登入还可能需要进行邮箱验证）  
等待几秒就可以了

<br>

**登陆报错解决**

使用 `npm login` 并正确输入所有信息后往往还会发生以下几种报错：

最常见的是这个报错信息：`[FORBIDDEN] Public registration is not allowed` ，他存在两种原因

1. 邮箱还没有验证，这个自己去官网验证
2. 镜像源不是官方的，比如我们会使用更快的淘宝源，此时输入以下代码切换回原版镜像，之后就可以正常登录了 `npm config set registry https://registry.npmjs.org/`

> 如果日常使用的话我们依然可以切换回淘宝源：  
> `npm config set registry https://registry.npm.taobao.org`

<br>

#### 新库创建

假设我们的新插件就叫做 zr-use，则新建同名文件夹；  
进入该文件夹，命令行下执行 `npm init -y`，自动初始化 `package.json`

这是 `package.json` 中的几个关键参数

1. name 插件名
2. version 插件版本
3. main 入口点，相对路径起始点为文件根目录
4. anthor 作者名字

> 由于是首次开发，version 直接写 1.0.0 即可，再填入作者名以及其他小参数即可

<br>

根目录下（和 package.json 同级）新建入口文件 index.js

简单写一下我们的插件，赋予其两个功能

```js
// 弹窗函数
function zr_alert(msg) {
  alert(msg);
}

// 这里为了展示，不单独抽离函数，直接把zr_add的功能以箭头函数的形式导出了
module.exports = {
  zr_alert,
  zr_add: (a, b) => {
    console.log(a + b);
  },
};
```

<br>

新建 README.md 文件（根目录下），这个文件的内容会展示在 npm 官网插件资料页上，一般会写插件的用法以及安装等等

<br>

#### 上传

上传前使用 `npm whoami` 验证当前用户是自己，否则请跳到第一步再次登录

根目录下打开命令行，输入以下命令上传： `npm publish`

没报错就代表成功了，此时你就可以通过 npm 官网搜到自己新上传插件了！

<br>

#### 插件更新

> 每次更新插件代码后，都必须赋予一个新的版本号才能正常上传！！！

这是一个标准的插件版本号： `1.0.0`  
请最好依据 npm 标准来指定每次更新后应当变动的版号

版号首位：里程碑式更新或者大面积重做  
版号中位：新功能较大幅度添加  
版号末位：小补丁更新

<br>

有两种方式更新版本号：

方法一：  
直接修改 `package.json` 里面的 `version` 字段，改成新版号；  
然后 `npm publish` 上传

方法二：  
使用命令自动更新版号（即修改 version 字段，为末位数字+1）：`npm version patch`  
然后上传 `npm publish`

<br>

#### 试试你的第一个插件吧！

手动搭设 vue 脚手架，这里我们就不浪费笔墨描述了

不需要在 `main.js` 全局引用（因为插件体量很小，没必要浪费代码行）

在任意 vue 文件的 script 调用即可！

```html
<script setup>
  import { zr_alert } from "zr-use";

  zr_alert("helloplugin");
</script>
```

> 打开网页，立即可见弹窗“helloplugin”

<br>
