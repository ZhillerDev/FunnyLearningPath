### 快速上手

#### 第一张表

废话少说，vite 搭设开发环境，`npm i echarts`安装对应库

`main.js` 全局挂载 echarts

```js
...

import * as echarts from "echarts";

const app = createApp(App);

app.config.globalProperties.$echarts = echarts;

...
```

`App.vue` 创建一张简单图表

简单来说就是获取指定 DOM，在该区域使用函数 `echarts.init` 画一张图表，图表内容依据自定义对象 `options` 来进行配置

```html
<template>
  <div>
    <!-- 设置DOM的ref值为demo -->
    <div ref="demo" :style="{ width: '100vw', height: '100vh' }"></div>
  </div>
</template>

<script setup>
  import { onMounted, getCurrentInstance, ref } from "vue";

  // 获取app实例对象中的proxy
  const { proxy } = getCurrentInstance();
  // 老套路，设置一个名字和ref属性值一样的变量，首先为null初始化
  const demo = ref(null);

  onMounted(() => {
    console.log(proxy);

    // demo.value直接获取到DOM，若直接使用document是没效果的！
    var chart = proxy.$echarts.init(demo.value);
    // 配置图表属性，这里完全照搬了echarts官网的配置
    let option = {
      title: {
        text: "ECharts 入门示例",
      },
      tooltip: {},
      legend: {
        data: ["销量"],
      },
      xAxis: {
        data: ["衬衫", "羊毛衫", "雪纺衫", "裤子", "高跟鞋", "袜子"],
      },
      yAxis: {},
      series: [
        {
          name: "销量",
          type: "bar",
          data: [5, 20, 36, 10, 10, 20],
        },
      ],
    };

    // 别忘了最后把自定义的options挂载到图表上
    chart.setOption(option);
  });
</script>
```

<br>

或者使用 vue2 的写法，更加简便

```html
<template>
  <div class="container">
    <div class="left">
      <div class="echarts-div" id="demo"></div>
    </div>
  </div>
</template>

<script>
  import * as echarts from "echarts";

  export default {
    name: "App",
    mounted() {
      this.showBar();
    },
    methods: {
      showBar() {
        var chart = echarts.init(document.getElementById("demo"));

        let option = {
          title: {
            text: "ECharts 入门示例",
          },
          tooltip: {},
          legend: {
            data: ["销量"],
          },
          xAxis: {
            data: ["衬衫", "羊毛衫", "雪纺衫", "裤子", "高跟鞋", "袜子"],
          },
          yAxis: {},
          series: [
            {
              name: "销量",
              type: "bar",
              data: [5, 20, 36, 10, 10, 20],
            },
          ],
        };

        chart.setOption(option);
      },
    },
  };
</script>
```

####
