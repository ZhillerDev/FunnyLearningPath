### 滚动加载动画

> 实现效果：滚到到指定元素后，该元素才执行对应动画

<br>

#### 依赖配置

wow 依赖于 animate 动画库，所以

安装 `animate.css` 以及 `wow.js`  
`npm install animate.css --save`  
`npm install wow.js --save`

<br>

#### 配置动画

首先写一个简单的页面

提供两个元素，第一个为占据全屏（是为了提供滚动空间）的 div，第二个为模拟需要执行动画的卡片

```html
<template>
  <div class="container">
    <!-- 占据全屏 -->
    <div class="before">index</div>

    <!-- 欲执行动画的div -->
    <div class="after">after</div>
  </div>
</template>

<script setup></script>

<style scoped lang="less">
  .container {
    width: 100%;

    .before {
      width: 100%;
      height: 100vh;
      background-color: antiquewhite;
    }

    .after {
      width: 40%;
      height: 40vh;
      background-color: aquamarine;
    }
  }
</style>
```

<br>

修改第二个元素，填入你想要执行的动画 class，类名可以直接在 `animate.css` 官网找到

除此之外，还有一个 `scroll-item` 类，这是为了让 wowjs 获取该 DOM 的标记

> 注意！这里先不要为元素添加 `animate__animated` 类名，这个在后面留给 wowjs 动态添加！！！

```html
<template>
  <div class="container">
    <div class="before">index</div>
    <div class="after scroll-item animate__fadeInRight">after</div>
  </div>
</template>
```

<br>

初始化 wowjs 代码，由于使用 vue3，对于 DOM 操作必须放在 onmounted 钩子执行

wowjs 的实现原理是：在滚动到指定元素位置时，动态的添加一个 class，使其具有动画切换效果，比如下方我们动态添加了类名 `animate__animated` ，这就使得 `animate.css` 得以启动并执行动画

```js
<script setup>
import "animate.css";
import Wow from "wow.js";
import { onMounted } from "vue";

onMounted(() => {
  const wow = new Wow({
    // 对所有拥有该class的元素挂载动画
    boxClass: "scroll-item",
    // 需要挂载的动画元素
    animateClass: "animate__animated",
    // 偏移值（即偏移一定距离后再执行动画，推荐设置！）
    offset: 100,
    // 移动设备是否生效
    mobile: true,
    // 是否支持异步
    live: true,
  });
  wow.init();
});
</script>
```

<br>
