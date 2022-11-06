---
title: Vuejs设计与实现精读
date: 2022-09-06 20:29:27
tags:

categories:
  - 前端面试八股
---

### 一、框架设计概览

#### 权衡的艺术

Vue.js 的内部实现一定是`命令式`的，而暴露给用户的却更加`声明式`
声明式代码的性能不优于命令式代码的性能，因为声明式需要最终需要转换成命令式，整个步骤的时间总和大于单次使用命令式

框架设计者要做的就是：在保持可维护性的同时让性能损失最小化

声明式代码的更新性能消耗 = 找出差异的性能消耗 + 直接修改的性能消耗

<br>

**在创建新页面时**
虚拟 DOM 运作方式： 先创建 JS 对象，后遍历虚拟 DOM 树并创建真实 DOM
纯 JS 运作方式： 创建新的 DOM 后依次拼接
若创建新页面，俩方式性能差异不大；
**在更新指定 DOM 时**
虚拟 DOM 运作方式：重构虚拟 DOM 树，比较存在差异的 DOM 并修改它
纯 JS 运作方式：销毁所有旧的 DOM 元素，再全量创建新的 DOM 元素
显然，此情况下虚拟 DOM 更占优

> 总结：原生 DOM 操作，性能最优，最难维护；虚拟 DOM，声明式且可维护性强

<br>

**三种框架应用手段**
纯运行时：用户手写树形结构数据后渲染；
运行时+编译时：用户手写 HTML 标签结构后渲染；
编译时：用户手写 HTML 标签后编译为命令式代码，中途无需渲染

> svelte 使用纯编译时，而 vuejs 使用运行时+编译时

<br>

#### 框架设计的核心要素

vue3 源码中的 `initCustomFormatter` 方法可以协助解析 `ref` 并以 `formatter` 的形式输出到 console

以下是 vue3 源码中警告输出方法，if 中变量*DEV*表示目前是否为开发环境，如果是的话此值为 true，即输出警告信息；若为生产环境则不会输出

```js
if (_DEV_ && !res) {
  warn(`xxxxxxx`);
}
```

<br>

**Tree-Shaking**

1. 本术语解释：消除那些永远不被执行的代码；
2. `rollup.js` 以及 `webpack` 都支持 tree-shaking
3. 实现条件：模块必须为 ES Module 类型，因为它依赖 ESM 的静态结构
4. 若代码存在副作用（即代码可能修改全局变量等），那么 tree-shaking 就无法将其消除

<br>

**框架输出构建产物**
由于 vue.global.js 本就是一个自执行函数 IIFE，故于 script 中调用就会立即执行

如果 package.json 中存在 module 字段，那么会优先使用 module 字段指向的资源来代替 main 字段指向的资源
若你使用 webpack 构建工程，那么使用的 vuejs 资源就是后者

```json
// vuejs源码中的package.json
{
  "main": "index.js",
  "module": "dist/vue.runtime.esm-bundler.js"
}
```

通过修改 rollup.config.js 配置为 `format:'cjs'`，即可使用 required （NodeJS 中的导包方法）来导入 vuejs

> cjs 表示 CommonJS

<br>

**特性管理机制**
用户可以自定义开启或者关闭某些特性，关闭的特性将会被 tree-shaking 清除；
一般使用 `webpack.DefinePlugin` 插件加上 `__VUE_OPTIONS_API__` 属性实现特性开闭

```js
new webpack.DefinePlugin({
  __VUE_OPTIONS_API__: JSON.stringify(true), // 开启特性
});
```

<br>

**错误处理**
为了避免用户使用工具类时仍然需要 try...catch 处理异常，我们可以在工具类中设置一个注册错误句柄，用户可以调用高句柄统一处理可能返回异常的函数！

```js
let handlerError = null;
export default {
  foo(fn) {
    callErrorHandling(fn);
  },
  registerErrorHandler(fn) {
    handlerError = fn;
  },
};
function callErrorHandling(fn) {
  try {
    fn;
  } catch (e) {
    handlerError(e);
  }
}
```

<br>

#### Vue3 设计思路

**所谓声明式 UI**
vuejs 中的渲染函数 render 借助 h 函数以虚拟 DOM 的形式描述 UI
h 函数的存在就是方便我们使用虚拟 DOM

```js
import { h } from "vue";
export default {
  render() {
    return h(`xxx`);
  },
};
```

<br>

**渲染器**
渲染器的作用就是把虚拟 DOM 渲染成真实 DOM
以下是渲染器方法的介绍：
参数一为 `vnode`，即虚拟 DOM；参数二为欲挂载虚拟 DOM 的真实 DOM
`renderer(vnode, document.body)`

渲染器运行的整个步骤：通过 vnode 为标签创建 DOM，为虚拟 DOM 添加各种属性后挂载到真实 DOM 上面

<br>

**组建本质**
组件就是一组 DOM 元素的封装
组件依然需要使用 renderer 方法挂载到真实 DOM 上，只不过 vnode 表示的是一个函数（该函数包含构成组建的许多虚拟 DOM）而非字符串

组件使用 `mountComponent` 进行渲染

<br>

**模板原理**
一个 vue 文件就是一个组件， `编译器` 会把 template 标签内容编译后加入到 script 里面去

vuejs 渲染的总流程就是：模板编译成渲染函数，渲染函数返回的虚拟 DOM 被渲染器接收后渲染为真实 DOM

组件的实现依赖于 `渲染器` ，模板的编译依赖于 `编译器`

<br>

### 二、响应系统

#### 响应系统作用域实现

响应式数据在 vue3 中使用 proxy 代理实现，基本原理：
当读取操作发生时，将副作用函数收集到“桶”中，
当设置操作发生时，从“桶”中取出副作用函数并执行
使用 `proxy` 拦截欲改变的数据 data，分别对 get 和 set 方法进行处理

> 这是比较简单的处理方法，因为我们调用 effect 副作用函数的方法是硬编码的形式，这极其不方便

```js
// 存储副作用函数的桶
const bucket = new Set();

// 原始数据
const data = { text: "helloworld" };

// 对原始数据进行代理操作
const obj = new Proxy(data, {
  // 读取拦截
  get(target, key) {
    bucket.add(effect);
    return target[key];
  },
  // 设置拦截
  set(target, key, newVal) {
    target[key] = newVal;
    bucket.forEach((fn) => fn());
    return true;
  },
});

// 副作用函数
function effect() {
  document.body.innerHTML = data.text;
}
```

<br>

**完整响应式结构剖析**
这里省略了极为冗长且条理清晰内容详实但仍然看不懂的响应式处理源码

为避免 effect 副作用函数的无线嵌套，vuejs 引入副作用函数栈 effectStack，即当副作用函数执行时压入栈中，待执行完毕后弹出

为避免副作用函数同时执行，设置一个 trigger 触发器，当 trigger 执行指定副作用函数时发现其与目前正在执行的副作用函数相同，则停止触发运行

<br>

**调度执行**
为了保证在 vuejs 内连续调用多次响应式数据但仅触发一次更新，vuejs 内部实现了一个完善的调度器
