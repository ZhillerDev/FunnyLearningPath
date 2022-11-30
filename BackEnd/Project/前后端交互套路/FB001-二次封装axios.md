### 基本封装

> 基于 vue2  
> 使用模块：axios elementui

<br>

#### 原理与结构

项目结构：
在前端根目录（src 文件夹）下新建 plugin 文件夹，新增俩文件

1. service.js 封装 axios 请求
2. setToken.js 操作 token 相关

<br>

前端请求：注入用户注册的信息什么的

后端反馈：一个 status 表示状态码，一个 msg 表示响应文本

<br>

#### 设置 token

setToken.js 代码清单

```js
export function setToken(toKey, token) {
  return localStorage.setItem(toKey, token);
}

export function getToken(toKey) {
  return localStorage.getItem(toKey);
}

export function removeToken(toKey) {
  return localStorage.removeItem(toKey);
}
```

<br>

#### 封装 axios

`baseURL` 表示 axios 实例请求的默认 url，他会自动连接到你请求的 url 之前（后面有展示）

```js
import axios from "axios";
import { getToken } from "@/plugin/setToken";
import { Message } from "element-ui";

// 获取axios实例
const service = axios.create({
  baseURL: "http://localhost:10086/api",
  timeout: 3000,
});

// 请求拦截器
service.interceptors.request.use(
  (config) => {
    // 设置请求头附带token
    config.headers["token"] = getToken("userinfo");
    return config;
  },
  (error) => {
    return Promise.reject(error);
  }
);

// 响应拦截器
service.interceptors.response.use(
  (resp) => {
    // 若收到的响应状态非200，则返回错误提示
    // 这里用了elementui的message提示框
    let { status, msg } = resp;
    if (status !== 200) {
      Message({
        message: msg,
        type: "error",
        duration: 1200,
      });
    }
  },
  (error) => {
    return Promise.reject(error);
  }
);

// 别忘了最后的暴露
export default service;
```

<br>

#### 全局配置 axios

```js
...

// 导入封装好的service
import service from "@/plugin/service";

// 全局挂载
Vue.prototype.service = service;


new Vue({
  // 全局注册路由
  router,
  render: (h) => h(App),
}).$mount("#app");
```

<br>

#### 使用 axios

任意组件内直接 this 调用就好了

```js
this.service
  .post("/login", this.form)
  .then((res) => {})
  .catch((err) => {});
```

<br>

### 进阶封装
