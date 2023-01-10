> 本文主要讲解 TSX 如何安装与基本使用方法，并顺带介绍 defineComponent 在 VUE3 中的使用方法！

<br>

### 八股

<br>

**JSX 是什么？**  
JSX 是一个 JavaScript 的语法扩展，类似于模板语法，或者说是一个类似于 XML 的 ECMAScript 语法扩展，并且具备 JavaScript 的全部功能 ——摘自 react 官方解释

<br>

**JSX 运作原理**  
无论是 JSX 语法还是 vue 的 template，亦或是二者最终转换成的 createElement 方法，都少不了 babel 的作用；

<br>

### TSX 环境配置

<br>

本项目中使用如下代码安装 jsx 插件；  
`npm i -D @vitejs/plugin-vue-jsx`

事实上，这是一个封装好且专门用于 vite 的 jsx 插件，而他的底层实现是 `@vue/babel-plugin-jsx` 插件，这个我们不用管，编译器会帮我们处理这些的

**全局注册**  
来到 vite.config.ts，文件中添加如下代码，进行 jsx 插件的注册：

```ts
import vueJsx from "@vitejs/plugin-vue-jsx";

export default defineConfig({
  plugins: [vue(), vueJsx({})],
});
```

<br>

### 基本使用

<br>

**编写 tsx 的三种方式**  
首先是 `函数式编程`  
在一个 tsx 文件下输入以下代码，此方法适用于显示单个标签或者少量标签

```ts
// demo.tsx
export default () => <div>123456</div>;
```

<br>

或使用传统方法： `defineComponent+render`  
此方法会导致 this 指向问题，不推荐；

```ts
import { defineComponent } from "vue";

export default defineComponent({
  render() {
    <div>github</div>;
  },
});
```

<br>

最新推荐方法： `defineComponent+setup`  
直接在 setup 中编写 JSX，后续的变量或者其他属性就可以直接写在 setup 里面，而无需再次使用 this 调用！

```ts
import { defineComponent } from "vue";

export default defineComponent({
  setup() {
    return () => <div>give me a hand</div>;
  },
});
```

<br>

**语法细节**  
当只渲染一个标签时，可以直接写标签；  
渲染两个及以上标签，需要 `<></>` 包裹！

```ts
// 只渲染一个标签时
export default defineComponent({
  setup() {
    return () => <div>hello!</div>;
  },
});

// 渲染两个及以上标签时，需要<></>包裹！
export default defineComponent({
  setup() {
    return () => (
      <>
        <div>hellowrodl</div>
        <span>name</span>
        <button>click it</button>
      </>
    );
  },
});
```

<br>

**使用 tsx 文件**  
直接类似导入 vue 组件一样，在需要使用 tsx 的 vue 文件内 import 就好了；  
注意 import 的路径里面不能包含 `.tsx` 后缀名！

```html
<template>
  <plus />
</template>

<script setup lang="ts">
  import plus from "./components/plus/plus";
</script>
```

<br>

### 特殊语法比较

<br>

**花括号替换**  
v-model 和 v-show，在 vue 中使用双引号绑定的，现在全部替换为花括号：  
下面例子使用 v-model 来绑定 setup 中我们定义的一个变量

> 为什么会这么写？因为 jsx 是适配 react 语法的，所以你有 react 基础的话，看这些完全不惊讶

```ts
export default defineComponent({
  setup() {
    const count = ref(0);
    return () => (
      <>
        <input type="text" v-model={count.value} />
      </>
    );
  },
});
```

<br>

**v-bind 变动**  
直接省略变量名前的冒号，再将双引号改成花括号即可！

```ts
<demo title={"set your name here"}>...</demo>
```

<br>

**v-if 变动**  
目前在 jsx 里面没有 v-if 的实现，但使用三元运算符代替；

```ts
setup() {
    const count = ref(0)
    return () => (
      <>
        <div>{1 == 1 ? <span>123</span> : <span>456</span>}</div>
      </>
    )
  }
```

<br>

**v-for 变动**  
使用 map 函数替代之；  
jsx 中的插值语法改用一个花括号（代替 vue 中的两个花括号）

```ts
setup() {
    const list = ref<string[]>([])
    return () => {
      list.value.map((data, index) => <p key={index}>{data}</p>)    // 注意到单个花括号了吗
    }
  }
```

<br>

**emit**  
所有 jsx 语法中的 click 点击事件必须要以 `on` 开头，且后续单词 `首字母均大写`  
jsx 中使用 emit，首先要在 setup 的第二个参数注册 emit，这是编译器为了后续进行强类型检查的准备

```ts
export default defineComponent({
  emits: ["click"],
  setup(props, { emit }) {
    return () => (
      <button
        onClick={() => {
          emit("click");
        }}
      >
        立即触发emit
      </button>
    );
  },
});
```

<br>

### 重难点语法

<br>
