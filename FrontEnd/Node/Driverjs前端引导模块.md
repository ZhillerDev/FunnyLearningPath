### 简介

> 参考官方 github 仓库：https://github.com/kamranahmedse/driver.js  
> 参考官方 demo：https://kamranahmed.info/driver.js/

driverjs 用于快速构建前端引导页，实现高亮指定元素并在元素之间执行切换

<br>

安装库，使用 yarn 或者 npm

```sh
yarn add driver.js
npm install driver.js
```

或者直接使用 CDN

```html
<script src="https://unpkg.com/driver.js/dist/driver.min.js"></script>
<link rel="stylesheet" href="https://unpkg.com/driver.js/dist/driver.min.css" />
```

<br>

以上内容配置完毕后直接在 js 代码块中导入指定库即可

```js
import Driver from "driver.js";
import "driver.js/dist/driver.min.css";
```

<br>

### 整合至 vue

#### @click 处理

如果直接使用原生 js 开发，仅需通过监听一个按钮点击事件并高亮指定选择器的元素即可；

而在 vue 中，我们需要为 @click 添加 prevent.stop 后缀，driverjs 才可以正常显示（否则会一闪而过）

譬如这是一个最简单的基于 vue 的 driverjs 调用方法

```html
<template>
  <div>
    <div class="a">这是第一个元素</div>
    <div class="b">这是第二个元素</div>

    <!-- 注意这里使用的是@click.prevent.stop -->
    <div @click.prevent.stop="high">点击高亮</div>
  </div>
</template>

<script setup>
  // 必须导入这两个库
  import Driver from "driver.js";
  import "driver.js/dist/driver.min.css";

  // 实例化driverjs对象以及绑定对应的高亮元素
  const driver = new Driver();
  const high = function () {
    driver.highlight(".b");
  };
</script>
```

<br>

### 案例上手

#### 简单引导与 driver 封装

为便于统一管理，将 driver 驱动抽离为单个文件设置参数  
以下参数均为官方默认指定

新建文件 `api/driver.js`

```js
import Driver from "driver.js";

const driver = new Driver({
  className: "scoped-class", // className to wrap driver.js popover
  animate: true, // Animate while changing highlighted element
  opacity: 0.75, // Background opacity (0 means only popovers and without overlay)
  padding: 10, // Distance of element from around the edges
  allowClose: true, // Whether clicking on overlay should close or not
  overlayClickNext: false, // Should it move to next step on overlay click
  doneBtnText: "完成", // Text on the final button
  closeBtnText: "关闭", // Text on the close button for this step
  nextBtnText: "下一步", // Next button text for this step
  prevBtnText: "上一步", // Previous button text for this step
  showButtons: false, // Do not show control buttons in footer
  keyboardControl: true, // Allow controlling through keyboard (escape to close, arrow keys to move)
  scrollIntoViewOptions: {}, // We use `scrollIntoView()` when possible, pass here the options for it if you want any
  onHighlightStarted: (Element) => {}, // Called when element is about to be highlighted
  onHighlighted: (Element) => {}, // Called when element is fully highlighted
  onDeselected: (Element) => {}, // Called when element has been deselected
  onReset: (Element) => {}, // Called when overlay is about to be cleared
  onNext: (Element) => {}, // Called when moving to next step on any step
  onPrevious: (Element) => {}, // Called when moving to next step on any step
});

export default driver;
```

<br>

之后设置对应的 steps，即步骤

注意事项：  
若想要在每个步骤弹窗底部都显示“下一步”等按钮，就必须设置 `showButtons: true` ，因为它默认是 false  
若要在最后一步弹窗设置完成按钮，需要配置 `doneBtnText` 属性

新建文件 `constant/driver-setting.js`

```js
const steps = [
  {
    element: ".a",
    popover: {
      title: "查看",
      description: "第一步：查看第一个单元格",
      position: "bottom",
      showButtons: true, // 显式指定需要显示底部按钮
    },
  },
  {
    element: ".b",
    popover: {
      title: "设置",
      description: "第二步：进行相关参数的设定",
      position: "bottom",
      showButtons: true,
    },
  },
  {
    element: ".a",
    popover: {
      title: "查看",
      description: "第一步：查看第一个单元格",
      position: "bottom",
      doneBtnText: "完成！", // 完成按钮的文本设定
      showButtons: true,
    },
    onNext: () => {}, // 点击下一步后的回调
    onPrevious: () => {}, // 点击上一步的回调
  },
];

export default steps;
```

<br>

最后在 vue 文件内编写 html 以及导入 driver

```html
<template>
  <div>
    <div class="a">这是第一个元素</div>
    <div class="b">这是第二个元素</div>
    <div @click.prevent.stop="high">点击高亮</div>
  </div>
</template>

<script setup>
  // 导入driver样式表与外部定义模块
  import driver from "./api/driver";
  import driverSteps from "./constant/driver-setting";
  import "driver.js/dist/driver.min.css";

  // 先设置步骤，再start启动
  const high = function () {
    driver.defineSteps(driverSteps);
    driver.start();
  };
</script>
```

<br>

####
