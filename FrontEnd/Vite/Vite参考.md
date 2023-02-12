### 数值问题

#### .env 环境变量配置

项目根目录下（不是 src 文件夹下）创建环境变量文件 `.env` 以及正式环境文件 `.env.pro`

对于使用 vite 框架，必须要在所有环境变量前加上 `VITE` 前缀，否则 vite 无法识别！！！  
譬如这是一个正确的变量：`VITE_DEMO = "demo"`

之后就可以在 js 文件直接使用该代码调用环境变量：  
`import.meta.env.VITE_DEMO`

<br>

### 开发技巧

#### 按钮点击后保持样式

> 此效果可以使用 element 通过修改属性轻松实现，现在介绍一个 vue3 的写法

首先准备样式表（务必注意层级关系！）  
这里使用 less 语法

1. `.btns` 为使用 vfor 渲染的按钮组的原版样式
2. `&:hover` 当鼠标经过按钮后的样式
3. `.btns-focus` 按钮获得焦点后保持的样式

```less
.btns {
  width: @btn-width;
  height: @btn-height;
  margin-top: 12px;
  padding: 8px;
  border-radius: 4px;

  transition: 0.2s ease;

  &:hover {
    background-color: aliceblue;
  }
}

.btns-focus {
  background-color: aliceblue;
}
```

<br>

template 部分  
`leftbar-btn-vue` 是我事先写好的一个按钮组件

1. `(item,id)` 其中的 item 为值，而我们需要的是 id
2. `class="btns"` 原版样式直接写上去，不执行绑定！
3. `:class="currentMode===id ? 'btns-focus':''"` 动态绑定样式，表示检测变量 currentMode 的值如果等于当前渲染的按钮的 id 值，那么就启用样式 btns-focus
4. `@click="changeMode(id)"` 点击按钮后传入当前按钮 id，然后修改 currentMode 的值，由于动态绑定，故会依据上一条所述启用对应的 class

```html
<div
  v-for="(item,id) in svgPaths"
  :key="id"
  class="btns"
  :class="currentMode===id ? 'btns-focus':''"
  @click="changeMode(id)"
>
  <leftbar-btn-vue :svgNames="item.path"></leftbar-btn-vue>
</div>
```

<br>

js 部分

完成上一部分讲到的，获取当前鼠标点击的（即获得焦点的）按钮组件的 id，并修改 `currentMode` 的值为该 id  
与此同时因 `:class` 所以会自动启用样式 `btns-focus`

```js
let currentMode = ref("");
let changeMode = (id) => {
  currentMode.value = id;
};
```

<br>
