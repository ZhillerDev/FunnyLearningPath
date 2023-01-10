### vite+gsap

#### 快速上手

vite 新增工程：`npm create vite@latest`

安装 gsap（或者跳过这一步直接使用 CDN）：`npm i gsap`

把 component 文件夹下的 helloworld 文件删干净，创建一个新的模板

写入以下代码  
很简单，只是要注意 gsap.to 方法务必写在生命周期钩子内部

```vue
<script setup>
import gsap from "gsap";
import { onMounted } from "vue";

// 必须写在生命周期函数内部gsap才可以正常获取DOM
onMounted(() => {
  // gsap.to(a,b) a表示操作对象的选择器，b表示操作内容
  // 我们直接标签选择h1，之后对其做出持续时间1s的向y轴增加100单位的简单动画
  gsap.to("h1", {
    duration: 1,
    y: 100,
  });
});
</script>

<template>
  <div>
    <h1>Hide this text.</h1>
  </div>
</template>

<style scoped></style>
```

<br>

####
