### pinia 优点罗列

取消了 getter 和 action 等冗杂的方法；  
ts 原生编写，速度更快；  
只有一个 store，并且添加内容自动部署，提升效率；

<br>

### 安装与注册

安装：npm install pinia -S  
在 main.ts 中引入 pinia 并注册（标准三步走，不做赘述）

```ts
import { createPinia } from "pinia";
const store = createPinia();
const app = createApp(App);
app.use(store);
```

<br>

### 简单 store 使用

在 src 文件夹下新建 store 文件夹，里面又新建 index.ts 和 store-name.ts，以下为源码解释

1. index.ts 是存储 store 对象的仓库，store-name.ts 是存储 store 名称的一个文件
2. 使用 defineStore 新增一个 store，参一为名称，参二为一对象；
3. state 返回一个函数，它的作用类似于 data，存储属性
4. getters 类似于计算属性，而 actions 类似于 methods

> 因为后续可能因需要使用多个 store，故新增 store-name.ts 文件来管理这些 store 的名称！

```ts
// index.ts文件内容
import { defineStore } from "pinia";
import { Names } from "./store-name";

export const testStore = defineStore(Names.TEST, {
  state: () => {
    return {
      current: 1,
      name: "hello",
    };
  },
  // 类似computed，用于修饰一些值
  actions: {},
  // 类似methods，可同步异步调用方法
  getters: {},
});

// store-name文件内容
export const enum Names {
  TEST = "TEST",
}
```

<br>

之后使用某个组件把 store 导入（导入时不用写完整路径，如下方代码所示）；  
再实例化这个仓库，就可以调用 store 中 state 内定义的属性了！！！（使用插值语法配合点运算符）

```html
<template>
  <div>pinia中提取的值：{{ data.current }}--{{ data.name }}</div>
</template>

<script setup lang="ts">
  import { testStore } from "./store";
  const data = testStore();
</script>
```

<br>

### state 定义值修改

在组件内部可以直接使用 store 实例来对 state 中定义的属性值进行修改；

- $patch 直接对 store 内的 state 进行修改
- $patch 接收一个函数，函数形参为 store 的 state，所以我们直接使用 state.xxx 即可修改对应的值！！！
- 当然还有单个变量修改的方法，就是直接 data.current=xxx 来修改，只不过结构不太好

```ts
<script setup lang='ts'>
import { testStore } from './store';
const data = testStore()

const changeVal = ()=>{
  data.$patch((state)=>{
    state.current=100,
    state.name='goodbye world!!!'
  })
}
</script>
```

<br>

或者直接对 state 下手，免去了$patch 代理的过程；  
但注意！使用此方式就必须要对 state 中的所有属性重新定义一次，否则会报错！

> 此方法为对象的书写方式，写对属性的赋值用冒号而不是等号，细心细心在细心，别一昧追求速度

```ts
const changeAgain = () => {
  data.$state = {
    current: 1000,
    name: "none",
  };
};
```

> TIPS：还有最后一种办法，直接在 actions 内定义一个修改 state 中属性的方法，然后于组件内部调用该方法即可

<br>

### store 解构

对 store 使用解构赋值的办法直接获得 state 中的所有属性；  
因为默认解构获得的属性不具有响应式，故需要使用 storeToRefs 方法来让这些属性具有响应式；  
一旦具有响应式就可以通过 xxx.value 修改他们的值了！！！

```html
<script setup lang="ts">
  import { storeToRefs } from "pinia";
  import { testStore } from "./store";
  const data = testStore();

  const { current, name } = storeToRefs(data); // 解构赋值
  const changeCurrent = () => {
    current.value = 100;
  };
</script>
```

<br>

### getters 解析

上文讲过，本对象类似于计算属性 computed，故对象内部属性都是一个方法且使用 return 返回值；  
组件调用时直接使用插值语法即可（假设组件内 store 实例名称为 Store）: `{{Store.getCurrent}}`

```ts
export const testStore = defineStore(Names.TEST, {
  state: () => {
    return {
      current: 1,
      name: "hello",
    };
  },

  getters: {
    getCurrent(): string {
      return `the current is ${this.current}`;
    },
  },
});
```

<br>

### actions 解析

本方法类似于 methods，可以支持同步和异步请求，同步不多说，下面代码展示异步的写法;  
async 定义异步变量或者方法来进行请求操作

```ts
type User = {
  name: string;
  age: number;
};

const login = (): Promise<User> => {
  return new Promise((reslove) => {
    setTimeout(() => {
      reslove({
        name: "helloworld",
        age: 12,
      });
    }, 2000);
  });
};

export const testStore = defineStore(Names.TEST, {
  state: () => {
    return {
      user: <User>{},
    };
  },
  actions: {
    async setUser() {
      const result = await login();
      this.user = result;
    },
  },
});
```

<br>

### 其他重要 API

**$reset**
重置 state 内所有属性（即恢复到 store 中设置的最初值）

```ts
const data = testStore(); // store实例
const reset = () => {
  data.$reset();
};
```

<br>

**$subscribe**
返回一个函数，函数默认有两个形参；  
检测 store 对象中任何一个属性的变动，变动转化成对象存储在形参 args 里，state 顾名思义就是存储变化后的 state

```ts
data.$subscribe((args, state) => {
  console.log(args);
  console.log(state);
});
```

<br>

### pinia 插件

为了解决页面刷新后数据丢失的情况，pinia 可以使用新增插件的方式存储缓存解决问题
