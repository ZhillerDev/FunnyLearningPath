### 整体项目思路

需求：实现一个在页面左上方的悬浮按钮，并且固定在一个位置；当路由导航到非 index 界面时，按钮可点击，点击后返回主页 index；当路由就在主页 index 界面时，按钮禁用不可点击

使用到的技术点：  
`elementplus` 中的按钮和固钉组件+导入并使用图标库；  
`vuerouter` 路由简单操作；  
`vue3` 中的计算属性 `computed` 的使用；

<br>

### 实现步骤

#### 项目简析

我们首先看一下当前的 App.vue 的主体界面，其实就是一个 routerview 标签而已；

考虑到到我们此次开发项目的特殊性，不再使用导航栏之类的固定内容，直接全屏作为一个路由，所以我们仅需在 `App.vue` 的 `template` 标签内添加一个 `router-view` 标签即可

其余 view 编写以及路由配置这里不详细介绍；

目前完整逻辑即：默认加载路由 `/` ，导航到 `index.vue`，点击该页面上的一个按钮后路由转到 `/demo`，同时切换页面至 `demo.vue`

<br>

#### 安装 ICON 库

> 这里建议大家移步至 elementplus 官网进行参考：https://element-plus.gitee.io/zh-CN/component/icon.html#基础用法

<br>

npm 下为项目添加库：`npm install @element-plus/icons-vue`

main.js 或 main.ts 添加如下代码全局注册图标库：

```js
import * as ElementPlusIconsVue from "@element-plus/icons-vue";

const app = createApp(App);
for (const [key, component] of Object.entries(ElementPlusIconsVue)) {
  app.component(key, component);
}
```

<br>

#### 悬浮按钮设计

悬浮按钮实现的方法我目前只想到一种：elementplus 的固钉 affix 组件+绝对定位脱离文档流

实现思路：

1. 按钮点击返回主页：获取路由实例，然后 push 一个 path 直接回到首页
2. 动态禁用按钮：使用计算属性，判断当前路由值是多少，如果等于 index 那么就禁用，其余状态就不禁用

<br>

下面的代码完整的展示了按钮的设计

固钉组件 `el-affix` 设计为紧贴顶部，且样式设置绝对定位，距离顶部和左侧分别 20px 距离，此时脱离文档流，不会干扰其余元素的渲染！

按钮组件 `el-button` 其中的属性 `disabled` 使用动态绑定，绑定一个计算方法，根据我们之前的思路来执行即可

```vue
<template>
  <el-affix position="top" class="mainaffix">
    <el-button
      :icon="House"
      type="primary"
      :disabled="currentPath"
      @click="backToIndex"
    ></el-button>
  </el-affix>
  ...
</template>

<script setup>
import { useRouter } from "vue-router";
import { House } from "@element-plus/icons-vue";
import { computed, ref, reactive } from "vue";

// 获取路由实例
const router = useRouter();
// 计算属性返回指定布尔值，判断是否应该禁用按钮
const currentPath = computed(() => {
  return router.currentRoute.value.path === "/";
});
// 返回主页的路由方法
const backToIndex = () => {
  router.push({
    path: "/",
  });
};
</script>

<style lang="less" scoped>
.mainaffix {
  position: absolute;
  left: 20px;
  top: 20px;
}
</style>
```

<br>

### 参考文档

https://element-plus.gitee.io/zh-CN/component/icon.html#基础用法
