---
title: Vue3登陆弹窗解决方案
date: 2022-09-10 14:57:55
tags:

categories:
  - 前端实战
---

### 测试环境搭设

这里使用 vite3.0 进行环境配置（如果不习惯可以改为 vite2+环境配置）

随意找一个文件夹，输入
`npm create vite@latest`
如果你是首次使用 `vite` 框架，npm 会为你下载并全局安装 vite，之后按照步骤创建工程就好了；

打开创建好的工程，删去 `components` 文件夹，并删除 `App.vue` 中的所有内容

在 src 目录下新建文件夹 `views`，views 下新建 `Dialog.vue`；
这个文件将作为弹窗组件使用！

> 至此所有基本架构已经搭设完毕，不需要安装任何插件！
> 当然，如果感兴趣的话可以安装一个 `animate.css` 动画插件

<br>

### provide 和 inject

inject 与 provide
这是 vue3 提供的组件间数据交互的方式
A 组件中的 provide 设置事件名称，并暴露父组件内的属性或者方法；
B 组件中的 inject 根据 A 组件的事件名称获取到暴露的属性以及方法，并对其做出修改或者增删！

provide 与 inject 使用格式演示：

```js
// A组件设置事件evt，并暴露了属性attribute以及方法method
provide("evt", {
  attribute,
  method,
});

// B组件接收事件evt，并对获得到的参数进行取值操作
// 因为有两个暴露内容，所以这里需要解构
const { attribute, method } = inject("evt");
console.log(attribute.value);
```

<br>

### 弹窗思路

**整体思路**
弹窗组件 Dialog.vue 需要实现的功能是覆盖整个页面，并在页面正中央显示登陆窗口，其余背景为无法点击的透明淡灰色；
可以为组件施加一个绝对定位，以便脱离文档流，并让他直接覆盖整个页面，并通过 v-if 判断本组件的显示与隐藏，即可实现登陆弹窗的效果了！

<br>

**代码清单：template**
一个标准的登陆弹窗，请注意最外层 div 挂载属性 v-if，用来判断弹窗显示隐藏！

```html
<template>
  <div class="dialog" v-if="isShow">
    <div class="content">
      <h3>登陆/注册</h3>
      <div class="inputs">
        <input type="text" placeholder="您的账户名" />
        <input type="password" placeholder="******" />
      </div>
      <div class="btns">
        <button @click="close">登陆</button>
        <button @click="close">注册</button>
      </div>
    </div>
  </div>
</template>
```

<br>

**代码清单：style**
没什么可说的，除了对最外层 div 设置为绝对定位并脱离文档流外，基本都是正常 css

```css
<style scoped>
.dialog {
  // 脱离文档流，并设置背景为半透明的颜色
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: rgba(0, 0, 0, 0.3);

  // 让登录窗口垂直水平居中
  display: flex;
  justify-content: center;
  align-items: center;

  // 保持登录窗口一直在页面最顶层
  z-index: 999;
}

.content {
  width: 250px;
  height: 270px;
  background-color: white;
  border-radius: 4px;

  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
}

.inputs {
  width: 200px;
  height: 110px;
}

.inputs input {
  width: 90%;
  height: 30px;
  margin: 10px 0 0 0;
}

.btns {
  width: 200px;
  height: 40px;
}
.btns button {
  height: 30px;
  width: 40%;
  margin: 0 0 0 10px;
}
</style>
```

代码清单：script

```js
<script setup>
import { ref, reactive, inject } from "vue";

// 获取provide传递的指定信息并解构
const { isShow, setShow } = inject("dialog-show");

// 点击登陆或者注册按钮后关闭页面
const close = () => {
  isShow.value = false;
};
</script>
```

<br>

### 弹窗触发者

在我们了解了 provide 和 inject 的工作原理后，就不难理解以下代码了

```html
<template>
  <div>
    <button @click="setShow(true)">弹出弹窗</button>
    <dialog></dialog>
  </div>
</template>

<script setup>
  import { ref, reactive, provide } from "vue";
  import Dialog from "./views/Dialog.vue";

  const isShow = ref(false);

  const setShow = (val) => {
    isShow.value = val;
  };

  provide("dialog-show", {
    isShow,
    setShow,
  });
</script>

<style scoped></style>
```

> 以上就是登陆弹窗的完整实现流程以及原理解析，直接贴到 IDE 里面 run dev 就可以执行了！
