### 基础使用

#### 标准通用模板

**src/router/index.ts**
先从 vuerouter 中导入三大件；  
设置对应 component 的路由地址；  
export 整个路由模块，createRouter 里面分别要写路由模式和路由信息；

> 请避免 router routes route 这些元素的混淆，他们各自代表不同的意思

```ts
import { createRouter, createWebHistory, RouteRecordRaw } from "vue-router";

const routes: Array<RouteRecordRaw> = [
  {
    path: "/",
    component: () => import("../components/xxx.vue"),
  },
];

export const router = createRouter({
  history: createWebHistory(),
  routes,
});
```

<br>

**注册 router**

针对 vue3，请使用 use 进行挂载

```js
...
import { router } from "./router/index.js";

const app = createApp(App);
app.use(router);
app.mount("#app");
```

<br>

**App.vue**

1. router-link 配合 router-view 使用，故一个 template 里面可以有多个 router-link，但只能有一个 router-view
2. router-link 和面的 to 表示需要连接到那个路由页面，路径就写我们在 path 里面定义的路径
3. 每次让 router-link 指定一个路径后，指定路径的页面就会显示到 router-view 插入的地方！！！

```html
<template>
  <div>
    <h1>点击下方按钮切换对应的页面</h1>
    <router-link to="/">login</router-link>
    <router-link style="margin-left: 20px;" to="/reg">reg</router-link>
    <hr />
  </div>

  <router-view></router-view>
</template>
```

<br>

#### 脚本式导航

导入模块 useRouter 后，通过使用 push 方法来跳转到对应的路由页面，而不是用 router-link 标签！！！  
下面展示了通过路由的 name 来跳转，当然也可以通过 path 来跳转

```html
<template>
  <div>
    <h1>点击下方按钮切换对应的页面</h1>
    <button @click="toPage('login')">sign in</button>
    <button @click="toPage('reg')">reg</button>
    <hr />
  </div>

  <router-view></router-view>
</template>

<script setup lang="ts">
  import { useRouter } from "vue-router";
  const router = useRouter(); // 导入路由使用模块

  const toPage = (url: string) => {
    router.push({
      name: url, // 根据路由名称打开对应页面
    });
  };
</script>
```

<br>

#### 历史记录

router-link 标签添加 replace 属性，即可不记录跳转历史

```html
<router-link replace to="/">login</router-link>
```

脚本跳转模式，把 push 替换成 replace 也可以达到不记录跳转历史的效果！

```ts
const toPage = (url: string) => {
  router.replace({
    name: url, // 根据路由名称打开对应页面
  });
};
```

<br>

使用 go()或者 back()来向前或者向后跳转一个网页

```ts
const toPage = (url: string) => {
  router.go(1); // 网页前进一步
  router.back(1); // 网页后退一步
};
```

<br>

#### 路由传参

**query 传参**
上层代码为发送参数的组件，下层代码为接收参数的组件；

- 传参的方式就是在 Push 中写入一个属性 query，之后 query 接受一个对象，该对象内写入你需要传递过去的属性；
- query 传参法可以使用 path 或者 name 来指定路由页面
- 接收组件需要导入 useRoute 并实例化它来接收参数
- 接收参数格式为{ {route.query.xxx} }

```html
<!-- 发出参数的组件 -->
<script setup lang="ts">
  import { useRouter } from "vue-router";
  const router = useRouter(); // 导入路由使用模块

  const toPage = (url: string) => {
    router.push({
      path: "/reg",
      query: {
        // query传入一个对象
        id: 1,
        job: "none",
      },
    });
  };
</script>

<!-- 接收参数的组件 -->
<template>
  <div>
    <div>{{ route.query.id }}</div>
    <div>{{ route.query.job }}</div>
  </div>
</template>

<script setup lang="ts">
  import { useRoute } from "vue-router";
  const route = useRoute();
</script>
```

<br>

**params 传参**
和`query`传参类似，但是他传入的参数是放在请求文件里面的，所以不会明明白白的显示在地址栏里面；  
在 push 中必须且只能使用 name 来配合 `params` 使用，不可以使用 path 指定路由页面；  
接收者组件只需要按照以下格式获取即可：`{{route.params.xxx}}`

<br>

#### 嵌套路由

需要定义定义路由中的 route；  
添加 children 属性，然后可以无限嵌套下去，但要注意最终 path 是拼接而来的，即一层层向上链接！！

```ts
const routes: Array<RouteRecordRaw> = [
  {
    path: "/",
    component: () => import("../components/footer.vue"),
    children: [
      {
        path: "",
        component: () => import("../components/login.vue"),
      },
      {
        path: "reg",
        component: () => import("../components/reg.vue"),
      },
    ],
  },
];
```

<br>

#### 组件命名

当一个 component 具有多个成员时，可以对 route-view 添加属性 name，来指定需要显示那个成员！

```ts
// 父组件定义导向的
const routes:Array<RouteRecordRaw> = [
    {
        path:'/',
        component:{
            login:import('../components/login.vue'),
            reg:import('../components/reg.vue')
        }
    }
]


// 接收者使用name来指定接收哪一个component
<router-view name="rv"></router-view>
```

<br>

#### 路由别名

在 routes 中加入参数 alias 即可指定别名；  
别名的作用：当使用 path 调用路由页面时，也可以直接使用别名代替；

```ts
const routes: Array<RouteRecordRaw> = [
  {
    path: "/",
    alias: ["/a", "/b"],
    component: {
      login: import("../components/login.vue"),
      reg: import("../components/reg.vue"),
    },
  },
];
```

<br>

#### 前置路由守卫

定义：路由守卫可以看做是一个筛选器，即根据访问者具有的权限来将其导航向不同的地址；  
例如某访问者想访问后台，但很明显其权限不够，路由守卫立刻阻挡并驳回请求（或者将其导向另一个网址）；

以下是前置路由守卫的主要格式：

- 单独使用 next()表示允许允许访问通过
- next('/')表示指向该访问到一个新的地址'/'
- 配合 if...else...对用户的权限进行判断，来指向性导航到某个网址

```ts
const whiteList = ["/"];

router.beforeEach((to, from, next) => {
  if (whiteList.includes("xxx")) {
    next();
  } else {
    next("/");
  }
});
```

<br>

#### 后置路由守卫

使用方式和前置路由守卫差不多，只是方法名变了一下而已，他表示在路由 next()完成后执行的响应

<br>

#### 路由元信息

即路由自带的一个数据包，里面可以存储变量等内容；  
使用 meta 定义，对象的形式！

```ts
export const router = createRouter({
  history: createWebHistory(),
  routes: [
    {
      path: "/",
      component: () => import("view/login.vue"),
      meta: {
        title: "这是一个标题",
      },
    },
  ],
});
```

<br>

### 进阶操作

#### 路由原理快速复习

> 重点重点重点！！！建议直接看完然后上手项目开发，不然会困惑很久（特别是对于我这种笨蛋来说）

众所周知，我们使用路由跳转必须要存在一个标签：`<router-view>`

一般的，我们会直接令 App.vue 的 template 标签内只保留一个该标签；  
此时我们无论在任何子组件中使用 push 或者 replace 切换路由，结果都会显示在 App.vue 的 router-view 标签内部（其实此刻就相当于全屏渲染新页面了，效果看起来和直接使用 a 标签进行页面跳转完全一致）；

这样做我们完全不用在意什么父子组件传参！

```html
<!-- 这是App.vue的全部内容 -->
<template>
  <!-- 只保留这一个标签！！！ -->
  <router-view></router-view>
</template>

<script setup></script>

<style lang="less" scoped></style>
```

<br>

#### 路由跳转动画

vuerouter4 建议我们不要使用旧版的 transition 里面套 routerview 了，改为使用插槽；

这里使用 `animate.css` 库制作动画
