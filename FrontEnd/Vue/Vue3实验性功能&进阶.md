### $ref 响应性语法糖

> 官方文档 https://cn.vuejs.org/guide/extras/reactivity-transform.html#explicit-opt-in

#### 启用$ref

由于这是一个实验性功能，故需要显式启用，下面是 vite 中开启的方式

```js
// vite.config.js
export default {
  plugins: [
    vue({
      reactivityTransform: true,
    }),
  ],
};
```

<br>

#### 快速上手

`$ref` 的作用简单说就是：免去了使用 `.value` 取值的过程；  
这一过程将会在编译阶段自动插入，提升开发者效率；

并非只有 ref 才可以加$，与他相关联的所有类型都可以加；  
譬如 `$shallowRef`

`$ref` 不需要使用 import 导入，当然你要这么做也没问题

```js
let count = $ref(0);

console.log(count);

function increment() {
  count++;
}
```

<br>

#### 其他不常用机制

使用$()将某个量变成响应式的

```js
function myCreateRef() {
  return ref(0);
}
let count = $(myCreateRef());
```

<br>

`$$()` 即可对其转义，让编译后的代码可以正常运行

如下代码，使用转义后编译的代码就直接返回 ref 对象而非 value 了

```js
let count = $ref(123);

console.log(count); // 编译为count.value
console.log($$(count)); // 编译为ref对象
```

<br>

### ref 属性

DOM 上的 ref 属性可以帮助我们在挂载时获取该 DOM

假设目前的 DOM 存在 ref 属性，属性值为 demo

```html
<template>
  <div>
    <div ref="demo">123</div>
  </div>
</template>
```

之后通过响应式 ref 的形式获取到该 DOM

```js
import { ref } from "vue";

// 特别特别注意！这里的变量名一定要和我们设定的ref属性值完全一致
// 首先赋null初始化，到onmounted再获取实例
const demo = ref(null);

onMounted(() => {
  // .value获取当前指向的DOM
  // 针对多种需要操作实际DOM的js库来说，推荐使用此方法
  console.log(demo.value);
});
```

<br>
