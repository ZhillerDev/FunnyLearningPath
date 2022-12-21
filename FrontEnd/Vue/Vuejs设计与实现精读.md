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

<br>

#### 非原始值的响应式方案

**代理 proxy**

代理仅能拦截对对象的基本操作（getter、setter）  
但是 obj.fn()属于复合操作，因为它 get 了两次

下方使用 proxy+apply 拦截，最终输出结果为 `let me think`

```js
const fn = (name) => {
  console.log("let me " + name);
};

const pp = new Proxy(fn, {
  apply(target, thisArg, arg) {
    target.call(thisArg, ...arg);
  },
});

pp("think");
```

<br>

**反射 reflect**

反射还可接收第三个参数，来表示 this，用来指示当前对象

```js
const obj = { foo: 1 };
// 直接读取
console.log(obj.foo); // 1
// 使用 Reflect.get 读取
console.log(Reflect.get(obj, "foo")); // 1
```

<br>

#### 合理的响应触发

一个简单的响应式 proxy 编写如下：

```js
const obj = { foo: 1 };

const p = new Proxy(obj, {
  // target对象
  // key欲修改的键
  // newval修改后的对应值
  // receiver
  set(target, key, newval, receiver) {
    // 获取旧值
    const oldval = target[key];
    // 判断当前类型
    const type = Object.prototype.hasOwnProperty.call(target, key)
      ? "SET"
      : "ADD";
    const res = Reflect.set(target, key, newval, receiver);
    // 当新旧值不全等且二者均不为NaN时，触发响应！
    if (oldval !== newval && (oldval === oldval || newval === newval)) {
      trigger(target, key, type);
    }
    return res;
  },
});
```

<br>

**深浅响应**

浅响应只会对首层的对象做响应式处理，而当我们改变深层属性时，并不会触发副作用产生函数更新！  
深响应即每次属性更改都将结果包装成响应式进行返回

shallowreactive 浅响应代码：

```js
function shallowReactive(obj) {
  return new Proxy(obj, {
    get(target, key, receiver) {
      if (key === "raw") return target;
      const res = Reflect.get(target, key, receiver);
      track(target, key);

      // 因为是浅响应，直接返回原值即可！
      return res;
    },
  });
}
```

<br>

可以添加一个“只读”readonly 判断，当视图修改一只读属性时，报错！

```js
if (isReadonly) {
  console.warn("该属性只读！");
  return true;
}
```

<br>

#### 代理数组

reactive 代理数组时主要有以下两个场景：

1. 判断索引值小于数组长度，表示不新增数据，则设置类型为 SET
2. 判断索引值大于数组长度，则新增数据，设置类型为 ADD

<br>

JS 迭代协议：即一个对象能否迭代，取决于该对象或者该对象的原型是否实现了 `@@iterator` 方法  
如果一个对象实现了 `Symbol.iterator` 方法，那么这个对象就是可以迭代的

数组内建了 `Symbol.iterator` 方法

> 一旦对象可迭代，就可使用 for...of...，而 for...in...无论迭代与否均可使用

```js
// 将一个对象变成可迭代的
const obj = {
  val: 0,
  // 设置标识，使其可迭代！
  [Symbol.iterator]() {
    return {
      next() {
        return {
          value: obj.val++,
          done: obj.val > 10 ? true : false,
        };
      },
    };
  },
};

// 调用可迭代对象
for (const values of obj) {
  console.log(values); // 0 1 2 3 4 5 6 7 8 9
}
```

<br>

#### 代理 set 与 map

**size 属性**

集合的 size 属性是一个访问器属性，故不能从代理对象中获取，只能从原始对象中取出！

以下表示当判断入参为 size 时，reflect 闭包作用域选择原始 set 对象，防止报错

```js
const s = new Set([1, 2, 3]);
const p = new Proxy(s, {
  get(target, key, receiver) {
    // 当入参为size时，指定反射第三个参数为原始set对象
    if (key === "size") {
      return Reflect.get(target, key, target);
    }
    // 非size参数就不多管了
    return Reflect.get(target, key, receiver);
  },
});

console.log(s.size); // 3
```

<br>

**delete 删除操作**

delete 是一个方法，他需要被绑定在原始对象而非代理对象上才可发挥作用！

使用 bind，改变作用域到原始对象上

```js
const s = new Set([1, 2, 3]);
const p = new Proxy(s, {
  get(target, key, receiver) {
    // 当入参为size时，指定反射第三个参数为原始set对象
    if (key === "size") {
      return Reflect.get(target, key, target);
    }

    // 绑定原始数据对象
    return target[key].bind(target);
  },
});
```

<br>

**foreach 难题**

如果 foreach 回调函数中的 value 形参不是一个响应式对象，那么将无法建立响应联系；

在 foreach 前对 value 进行检测，如果非响应式，则将其包装为响应式并返回使用

```js
forEach(callback) {
  const wrap = (val) => (typeof val === "object" ? reactive(val) : val);
  ...
},
```

<br>

**两大响应式操作**

get 读取数据时：使用 track 函数追踪依赖关系建立响应联系；  
set 设置数据时：使用 trigger 函数触发响应；

> 数据污染：把响应式数据设置到原始数据上的行为

<br>

**迭代器协议与可迭代协议**

可迭代协议指的是一个对象实现了 Symbol.iterator 方法  
迭代器协议指的是一个对象实现了 next 方法

一个对象可同时实现二者：

```js
const obj = {
  // 迭代器协议
  next() {},
  // 可迭代协议
  [Symbol.iterator]() {
    return this;
  },
};
```

<br>

#### 第二章快速总结

vuejs3 的响应式数据是基于 proxy 实现的

访问器属性 this 指向问题可以使用 reflect 进行重定向

合理触发 ITERATE_KEY 相关联的副作用函数重新执行

数组是一个异质对象（异质对象相对的概念是常规对象）

在代理对象中查找不到元素时，再去原始对象里面找

修改数组长度的原型方法需要使用标记变量 shouldTrack 来代表是否允许进行追踪，以免多个副作用函数相互调用导致栈溢出

集合以及映射的 size 属性是一个访问器属性！

注意数据污染问题

可迭代协议以及迭代器协议区分

<br>

### 三、原始值响应式

> 所谓原始值即针对六大基本类型的值的响应式

<br>

#### ref

由于 proxy 无法阻止原始值的修改，故此方法在这里不管用

我们可以创建一个包裹对象把原始值包起来；  
然后在使用 reactive 将该包裹对象变成响应式的即可

> \_\_v_isRef 属性是用来判断是否为 ref 包裹对象的判据

```js
function ref(val) {
  // 包裹对象
  const wrapper = {
    value: val,
  };
  // 是否为ref的判据
  Object.defineProperty(wrapper, "__v_isRef", {
    value: true,
  });
  // 返回响应式数据
  return reactive(wrapper);
}
```

<br>

#### 响应丢失

响应丢失问题：使用 reactive 创建响应式对象后，使用多参的形式 return，则实际上返回的是普通对象而非响应式对象！

```js
export default {
  setup() {
    const obj = reactive({ a: 1, b: 2 });
    return {
      ...obj,
    };
  },
};
```

<br>

**响应关系建立**

obj 为原响应对象

newobj 内拥有与 obj 同名的所有属性值，且均一一编写访问器属性 value  
value 执行后返回原响应对象中的值

这样一来，修改 obj 属性后，便会自动触发副作用函数执行！

```js
const obj = reactive({ a: 1, b: 2 });

const newobj = {
  a: {
    get value() {
      return obj.a;
    },
  },
  b: {
    get value() {
      return obj.b;
    },
  },
};

console.log(newobj.a.value);
```

<br>

**toRef 函数的实现**

使用包裹 wrapper，内部加上 getter、setter 方法实现响应式

```js
function toRef(obj, key) {
  const wrapper = {
    get value() {
      return obj[key];
    },
    set value(val) {
      obj[key] = val;
    },
  };

  Object.defineProperty(wrapper, "__v_isRef", {
    value: true,
  });

  return wrapper;
}
```

<br>

#### 自动脱 ref
