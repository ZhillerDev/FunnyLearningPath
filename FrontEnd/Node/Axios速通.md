### 封装 Axios

> 重头戏，认真看，别量子阅读！

此次演示使用 vite 框架，故配置环境变量 .env （具体流程请移步对应文章或查找资料）中的 axios 默认 url  
`VITE_AXIOS_URL = "http://localhost:10085"`

<br>

新建文件：`src/request/axios.js`  
编写源码

```js
// 引入axios
import axios from "axios";

// 设置axios默认请求URL
axios.defaults.baseURL = import.meta.env.VITE_AXIOS_URL;

// 请求拦截器
axios.interceptors.request.use(
  async (config) => {
    // 判定token是否过期，决定请求与否
    config.headers.token = sessionStorage.getItem("token");
    return config;
  },
  (error) => {
    return Promise.error(error);
  }
);

// 响应拦截器
axios.interceptors.response.use(
  // 针对成功200响应值以及其他情况做出promise判定
  (resp) => {
    if (resp.status === 200) {
      return Promise.resolve(resp);
    } else {
      return Promise.reject(resp);
    }
  },
  (error) => {
    if (error.response.status) {
      switch (error.response.status) {
        case 400:
          break;
        default:
      }
      return Promise.reject(error.response);
    }
  }
);

// get方法
const $get = (url, params) => {
  return new Promise((resolve, reject) => {
    axios
      .get(url, { params: params })
      .then((res) => {
        resolve(res.data);
      })
      .catch((err) => {
        reject(res.data);
      });
  });
};

// post方法
const $post = (url, params) => {
  return new Promise((resolve, reject) => {
    axios
      .post(url, QS.stringify(params))
      .then((res) => {
        resolve(res.data);
      })
      .catch((err) => {
        reject(err.data);
      });
  });
};

// 这不暴露的步骤只要是vue3及以上用户都必须添加该代码！作为全局注册使用！
export default {
  install: (app) => {
    app.config.globalProperties["$get"] = $get;
    app.config.globalProperties["$post"] = $post;
    app.config.globalProperties["$axios"] = axios;
  },
};
```

<br>

这还没完，要去 main.js 里面把封装好的 axios 挂载了！

```js
...
import Axios from "./request/axios.js";

const app = createApp(App);
app.config.globalProperties.$echarts = echarts;
app.use(Axios);
app.mount("#app");
```

<br>

### XMLHttpRequest(XHR)简介

以下案例展示了一个最经典的 XHR 异步请求示例，具体详细内容可以查看 WDN 官网：https://developer.mozilla.org/zh-CN/docs/Web/Guide/AJAX/Getting_Started

1. 首先获取 id 为 btn 的按钮的 DOM 节点，并为之添加点击监听事件；
2. 实现点击后回调函数 makeXHR 内部构造；
3. 实例化 XMLHttpRequest 对象，并检测实例化是否成功，如果失败则直接跳过所有步骤宣布失败；
4. 实例化完毕后立即 open 一个网站，注意你使用的请求方法，然后 send 发送请求；
5. onreadystatechange 检测并分析响应内容；  
   readyState 检测服务端连接状态，XMLHttpRequest.DONE 表示服务端成功响应  
   xhr.status 检测响应状态码，200 表示成功！
6. 成功收到响应后便可使用 responseText 将所有响应内容转化为文本形式显示

```js
// 使用直接执行函数
(function () {
  var xhr;
  document.getElementById("btn").addEventListener("click", makeXHR);

  // 按钮点击监听事件
  function makeXHR() {
    xhr = new XMLHttpRequest();

    // xhr实例创建失败后的判断
    if (!xhr) {
      alert("无法创建XHR实例");
      return false;
    }

    // 首先open打开需要请求的网站，切记不可忽略www，然后send发送数据！
    xhr.open("GET", "http://localhost:10086/serve");
    xhr.send();

    xhr.onreadystatechange = function () {
      // readyState检测链接完成状态，DONE表示服务器响应收到了并且无任何差错
      if (xhr.readyState === XMLHttpRequest.DONE) {
        // 接收状态码200表示ACK
        if (xhr.status === 200) {
          // 直接获取GET到的文件内容
          alert(xhr.responseText);
        } else {
          alert("错误！无法获取request内容");
        }
      }
    };
  }
})();
```

<br>

### 万金油 axios 模板

axios 是为了方便 ajax 请求而存在的；

1. 导入 axios 包
2. 直接使用 axios 实例，传入对应请求方法和请求地址，data 为传入数据（这可不是 params!!!）
3. then 内有回调函数 response，其存储着我们获取到的响应数据

```js
const axios = require("axios");

axios({
  method: "GET",
  url: "http://www.xxx.com/",
  data: {
    name: "xxx",
  },
}).then((response) => {
  console.log(response);
});
```

<br>

### 修改默认配置项

axios 自带了很多请求配置，具体请查看官方文档：https://www.axios-http.cn/docs/req_config  
直接使用 axios.defaults.xxx 的形式设置全局配置项  
或者如第二行代码，创建实例后直接在实例内部覆写配置项

```js
// 全局默认配置项
axios.defaults.baseURL = "http://www.zhiyiyi.com/";

// 实例内部配置项
const instance = axios.create({
  baseURL: "https://api.example.com",
});
```

<br>

### 拦截器

即针对请求和响应是的各个周期做出的反应；  
axios.interceptors.request.use 设置全局的，请求时的拦截器  
axios.interceptors.response.use 设置去哪聚的，响应时拦截器

```js
axios.interceptors.request.use(
  (config) => {
    // 发送请求前做些什么
    return config;
  },
  (error) => {
    // 请求出问题后做些什么
    return Promise.reject(error);
  }
);

// 响应拦截器
axios.interceptors.response.use(
  (response) => {
    // 当响应发出前做些什么
    return response;
  },
  (error) => {
    // 响应出错时做些什么
    return Promise.reject(error);
  }
);
```

<br>

当拦截器作为一个实例存在时，即可使用 eject 方法来移除指定的拦截器

```js
// 删除拦截器
const myInterceptor = axios.interceptors.request.use(function () {
  /*...*/
});
axios.interceptors.request.eject(myInterceptor);
```

<br>

为自定义 axios 实例挂载对应拦截器

```js
// 自定义实例挂载拦截器
const instance = axios.create();
instance.interceptors.request.use(function () {
  /*...*/
});
```

<br>

### 错误拦截

使用 catch 配合对应箭头函数（或 function 函数）即可捕获异常

```js
axios.get(url, params).catch((err) => {
  console.error(err);
});
```

<br>

### 取消请求

不多说废话，直接看代码你就懂了

```js
const controller = new AbortController();

axios
  .get("/foo/bar", {
    signal: controller.signal,
  })
  .then(function (response) {
    //...
  });
// 取消请求
controller.abort();
```
