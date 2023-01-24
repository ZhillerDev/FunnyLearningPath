### 简介

> 建议可参考 elementplus 官网介绍：https://element-plus.gitee.io/zh-CN/component/dialog.html#基础用法

el-dialog 组件可以实现在网页顶层弹出一个带遮罩的对话框，对话框的内容可以给我们自定义

官网上对于具体属性的介绍已经相当全面，这里不再过多赘述

<br>

### 套用外部组件

#### 思路简介

需求：点击按钮弹出添加用户窗口，对话框内点击确认后自动校验表单，若不符合则不关闭对话框，若符合就关闭对话框

实现方法：（我们将外部组件称为子组件，el-dialog 标签所在的文件称为父组件）

1. 编写外部组件 insert-user-comp.vue
2. el-dialog 中使用 component 组件引用外部组件
3. 子组件提交表单 validate 校验
4. 使用 mitt 插件向父组件传递“校验成功”讯号，关闭对话框

> 需要安装 mitt： npm i mitt

<br>

#### 父组件主体

在 template 中的任意地方都可以直接添加 `el-dialog` 标签，因为其弹出时会自动置于顶层，所以标签所在位置无影响；

下面展示了一个点击“添加用户”按钮后弹出对话框的案例

使用 component 标签以直接在 dialog 标签内渲染外部组件

```html
<template>
  <!-- 网页内容 -->
  <div class="iu-container">
    <div class="iu-main">
      <div class="iu-header">
        <el-button type="primary" @click="insertStatus=!insertStatus"
          >添加用户</el-button
        >
      </div>
      <div class="iu-body"></div>
    </div>
  </div>

  <!-- 对话框内容 -->
  <el-dialog
    v-model="insertStatus"
    style="width: 400px;"
    title="添加用户"
    draggable
  >
    <!-- component标签可以渲染外部组件 -->
    <component :is="insertVue" />
  </el-dialog>
</template>
```

script

```js
import { reactive, ref, watch } from "vue";
import insertVue from "../../components/modify/insert-user-comp.vue";
import emitter from "../../api/emitter.js";

// 设置对话框状态，若为true则弹出，false则关闭
const insertStatus = ref(false);

// 定义mitt接收器
const detectEmitter = () => {
  // 只要接收到true的信号，则关闭对话框
  emitter.on("insertResult", (data) => {
    if (data === true) insertStatus.value = !insertStatus;
  });
};
detectEmitter();
```

<br>

#### 子组件主体

子组件中表单和校验规则大家可以参考官网自己写，这里不是关键点就不一一写出来了呜呜呜

下面的就是最重要的校验方法了  
`fastMessage` 是我自己封装的一个弹出 ElMessage()  
`apiModify` 为封装的 axios 请求

<br>

特别注意：mitt 广播数据时，数据必须是非响应式的数据，若直接传入一个 ref 或者 reactive 数据，那么接收者接收到的就是一个 proxy 代理，没法直接拿取数据而造成困难

```js
// 校验方法
const submitForm = (formEl) => {
  if (!formEl) return;
  formEl.validate((valid) => {
    if (valid) {
      console.log();

      // 注意注意！这里需要手动定义key与value，不可以直接使用响应式数据formLabelAlign
      addUser({
        uname: formLabelAlign.username,
        pwd: formLabelAlign.password,
        role: formLabelAlign.role,
      });
    } else {
      console.log("error submit!");
      return false;
    }
  });
};

// axios请求方法
const addUser = async (insertData) => {
  await apiModify
    .insertUser("/mdb/insertuser", insertData)
    .then((res) => {
      fastMessage.success(res.data.msg);

      // 若后端放回成功信息，则向父组件传递true来关闭对话框
      emitter.emit("insertResult", true);
    })
    .catch((err) => {
      fastMessage.error(err.response.data.msg);
    });
};
```

<br>
