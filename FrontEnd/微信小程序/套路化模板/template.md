### template 模板页面

> 对于需要频繁复用的简单页面，或者小组件，我们可以将其抽离出来作为单个 template 进行动态渲染；  
> 动态渲染所需要的数据，通过调用者传递

<br>

假设目前需要一个模板文件 temp.wxml，主文件为 index.wxml

首先，根目录下新建 templates 文件夹，同时创建模板文件 temp.wxml

其中

1. name="temp" 表示设置当前模板的名称为 temp，后续调用会用到
2. {{demo}} 这里就取出传过来的参数中的属性 demo

```html
<template name="temp">
  <view>
    <text>{{demo}}</text>
  </view>
</template>
```

<br>

之后在主页面的 js 文件添加欲传入的参数

```js
data: {
    res:[
      {
        demo:"jack"
      },
      {
        demo:"tom"
      },
      {
        demo:"lily"
      }
    ]
  },
```

<br>

这是在主页面内使用模板的方式：

1. 首先使用 import 标签导入模板 wxml，否则识别不到
2. block 标签定义一个块，并使用列表渲染的方式渲染模板，block 本身不占用任何空间且没有实际意义
3. template 标签内的 is 属性即我们之前定义的 name，选择要渲染的模板；  
   data 即传递给模板的数据，因为我们实验 res 进行列表渲染，那么这里的 `解构形式{{...item}}` 也就是将 res 作为一个数组发送过去

> 模板文件接收到传递过来的数据后，一次获取属性即可实现所谓的动态渲染了

```html
<view>
  <import src="../../templates/temp" />

  <block wx:for="{{res}}" wx:key="index">
    <template is="temp" data="{{...item}}"></template>
  </block>
</view>
```

<br>
