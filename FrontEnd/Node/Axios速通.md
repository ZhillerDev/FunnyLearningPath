### Axios 基础

#### XMLHttpRequest(XHR)简介

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

#### 万金油 axios 模板

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

#### 修改默认配置项

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

#### 拦截器

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

#### 错误拦截

使用 catch 配合对应箭头函数（或 function 函数）即可捕获异常

```js
axios.get(url, params).catch((err) => {
  console.error(err);
});
```

<br>

#### 取消请求

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

### 高级封装 Axios

> 参考文献 https://juejin.cn/post/7143488626955911181

<br>

#### 请求思路

1. 自定义 getData 方法，他通过 axios 实例进行 get 请求并返回一个 promise 实例
2. useRequest 方法，用来处理 getData 等其余返回 promise 实例的方法，然后把 ref 绑定好的数据返回给调用者
3. 自己注册好 axios 实例以及拦截器

<br>

#### .env

环境变量配置文件，没什么可说的，直接在根目录（不是 src 文件夹）下添加文件 `.env`

随便添加一条为后续 axios 实例构造时用到的 `BASE_URL`

```
VITE_BASE_URL='http://localhost:10086/'
```

<br>

#### request.js

src 文件夹下新建 api 文件夹，创建 request.js 作为封装 axios 代码所用

首先创建一个 axios 实例，下面是几个经典的配置参数

```js
export const server = axios.create({
  baseURL: import.meta.env.VITE_BASE_URL,
  timeout: 300 * 1000,
  withCredentials: true,
  headers: {
    "Content-Type": "application/x-www-form-urlencoded",
  },
});
```

其次就是请求与响应拦截器，这里大家可以自定义

```js
// 请求拦截器
server.interceptors.request.use(
  (config) => {
    // 如果请求方法为GET，则字符串化params
    if (config.method === "get") {
      config.paramsSerializer = (params) => {
        return qs.stringify(params, {
          arrayFormat: "comma",
        });
      };
    }

    // 获取用户token并附带在headers里传递给后端
    const token = localStorage.token;
    if (token) {
      config.headers["token"] = token;
    }
    return config;
  },
  (err) => {
    Promise.reject(err);
  }
);

// 响应拦截器
server.interceptors.response.use(
  (config) => {
    return config;
  },
  (err) => {
    // 如果错误的响应体存在的话
    if (err.response) {
      // 获取响应值message
      const msg =
        err.response.data === null
          ? "服务端出错，无法获取响应内容"
          : err.response.data.message;

      // 根据响应状态码来返回对应信息
      switch (err.response.status) {
        case STATUS.success.id:
          ElMessage(STATUS.success.msg);
          break;
        default:
          if (msg === "invalid token") {
            ElMessage("登录状态过期，请重新登陆");
          } else {
            ElMessage(msg);
          }
          break;
      }
    }
    return Promise.reject(err);
  }
);
```

<br>

到了最关键的 `useRequest` 方法部分了

这里的 `service` 形参接收一个返回 promise 实例的 API 方法，比如我们之后要定义的 `getData`

这里使用异步请求并绑定 ref，可以使得数据在获取完毕后立刻触发响应式更新 UI

options 形参用于标识是否开始以及深度参数（不重要，可以在调用的时候忽略）

```js
export const useRequest = (service, options) => {
  const res = ref();
  const run = async () => {
    res.value = await service();
  };
  if (options.ready) {
    run();
  }
  watch(options.deps, () => {
    run();
  });
  return res;
};
```

<br>

#### api.js

在 api 文件夹下直接新建 `api.js`

分别为 `GET` 和 `POST` 请求定义对应的方法

```js
export function getData(url, params) {
  return new Promise((resolve, reject) => {
    server
      .get(url, params)
      .then((res) => {
        resolve(res?.data?.data);
      })
      .catch((err) => {
        reject(err);
      });
  });
}

export function postData(url, formData) {
  return new Promise((resolve, reject) => {
    server
      .post(url, formData)
      .then((res) => {
        resolve(res?.data?.data);
      })
      .catch((err) => {
        reject(err);
      });
  });
}
```

<br>

#### 调用

因为 ref 可以接收一个 ref 数据，故我的做法是，现在外部定义一个 ref，并于 `onMounted` 钩子获取后端数据

由于 `useRequest` 直接返回的就是一个 ref 属性，所以不用担心匹配问题

```js
let result = ref();

onMounted(() => {
  result = useRequest(() => getData("bk/admin/getnote"), {
    ready: true,
    deps: [],
  });
});
```

> 由于异步获取后端数据需要一段时间，故 ref 绑定可以在获取正确数据后立刻刷新 UI，很方便！

<br>

### 项目实战

#### 推荐封装结构

> 鉴于最先写的 Axios 封装有些过于简单了，现在介绍一下我比较喜欢使用的 axios 封装方式

创建文件夹 api，该文件夹存放 axios 封装的所有内容

主要文件解释：  
`http.js` 封装 axios 的 get 和 post 请求；  
`interceptor.js` 封装 axios 的拦截器以及服务器初始化  
`status.js` 封装常见的 code 与响应 message  
`api-xxx.js` 封装对应模块的 axios 请求

<br>

代码清单：interceptor.js

```js
import axios from "axios";
import qs from "qs";
import { ElMessage } from "element-plus";
import { STATUS } from "./status";

// 初始化后端服务器数据
const server = axios.create({
  baseURL: "http://localhost:10001",
  timeout: 30 * 1000,
});

// 请求拦截器
server.interceptors.request.use(
  (config) => {
    // 如果请求方法为GET，则字符串化params
    if (config.method === "get") {
      config.paramsSerializer = (params) => {
        return qs.stringify(params, {
          arrayFormat: "comma",
        });
      };
    }

    // 获取用户token并附带在headers里传递给后端
    const token = localStorage.token;
    if (token) {
      config.headers["token"] = token;
    }
    return config;
  },
  (err) => {
    Promise.reject(err);
  }
);

// 响应拦截器
server.interceptors.response.use(
  // 如果没错误就直接通过不拦截
  (config) => {
    return config;
  },
  (err) => {
    // 如果错误的响应体存在的话
    if (err.response) {
      // 获取响应值message
      const msg =
        err.response.data === null
          ? "服务端出错，无法获取响应内容"
          : err.response.data.message;

      // 根据响应状态码来返回对应信息
      switch (err.response.status) {
        case STATUS.success.id:
          ElMessage(STATUS.success.msg);
          break;
        default:
          // 后端验证token无效，写到响应体里返回给前端
          // 前端在此判定为无效，提示对应信息
          if (msg === "invalid_token") {
            ElMessage("token无效，请重新登陆");
          } else {
            ElMessage(msg);
          }
          break;
      }
    }
    return Promise.reject(err);
  }
);

// 导出拦截器供后续使用
export default server;
```

<br>

代码清单：http.js  
主要对 get 和 post 方法进行封装，设置 config，并判断有无 params 以及 formdata 来决定是否添加对应参数到请求体中

特别的，对于 post 请求的 config，必须要设置 `header` 中的 `content-type`，否则后端无法识别传递过来的 formdata（比如 GIN 框架下就是无法识别的！）

```js
import qs from "qs";
import intcp from "./interceptor";

const http = {
  // axios执行get请求
  get(url, params) {
    const config = {
      method: "get",
      url: url,
    };
    // 当存在params时，才会将其添加到config并发送给后端
    if (params) config.params = params;
    return intcp(config);
  },

  // axios执行post请求
  post(url, formdata) {
    const config = {
      method: "post",
      url: url,
      // 这一条必须要加，否则后端无法识别formdata
      headers: {
        "Content-type": "application/x-www-form-urlencoded",
      },
    };
    if (formdata) config.data = formdata;
    console.log(config);
    return intcp(config);
  },
};

export default http;
```

<br>

代码清单 status.js

```js
// 在这里存储所有状态码及其对应的所有信息
export const STATUS = {
  success: {
    id: 200,
    msg: "请求成功",
  },
  failed: {
    id: 400,
    msg: "请求失败",
  },
};
```

<br>

代码清单 api-login.js

因为每次请求都需要使用 then 与 catch 处理 promise  
故这段代码即展示我们在编写不同模块时使用到的 axios 请求抽离出来的样子

```js
import http from "./http.js";
import { fastMessage } from "../constants/message.js";

export default {
  // 抽离出来的登录校验请求（使用token配合jwt验证）
  // 传入三个参数：后端url、用户名、密码
  validateLogin: (url, uname, pwd) => {
    // 使用post请求，请求体data即传入两个必要值
    http
      .post(url, {
        username: uname,
        password: pwd,
      })
      // 在then中处理请求成功的方法
      .then((res) => {
        console.log(res);
        let token = res.data.msg;
        localStorage.setItem("token", token);
        fastMessage.success("成功登录！");
      })
      // 在catch中处理请求失败的方法
      .catch((err) => {
        fastMessage.error("用户名或密码错误！");
        return false;
      });

    // 返回值有无取决于你的项目需求
    return true;
  },
};
```

<br>

#### axios 请求中异步与否问题

众所周知，axios 请求有些请求需要配合异步函数来实现，我大概总结了两种适配场景：

不使用异步：后端响应值对后续操作无影响，我们仅需要判断这个请求是否成功的情况下  
使用异步：我们需要存储后端响应值并使用它进行某些渲染操作

<br>

**不使用异步的例子**

参考我们上一节的最后一块，我们抽离了登录校验的 axios 请求，这是具体代码

可见，我们只需要把本地 token 传递给后端，经过后端 jwt 验证并返回一个成功状态码 200，既然成功了，自然就会执行 then 内的方法，此时我们无需对响应体做出任何判断都可以明确的知道此次登录验证是成功的，所以直接返回一个布尔值 true 即可

这里就不需要使用到异步函数

```js
validateLogin: (url, uname, pwd) => {
    // 使用post请求，请求体data即传入两个必要值
    http
      .post(url, {
        username: uname,
        password: pwd,
      })
      // 在then中处理请求成功的方法
      .then((res) => {
        console.log(res);
        let token = res.data.msg;
        localStorage.setItem("token", token);
        fastMessage.success("成功登录！");
      })
      // 在catch中处理请求失败的方法
      .catch((err) => {
        fastMessage.error("用户名或密码错误！");
        return false;
      });

    // 返回值有无取决于你的项目需求
    return true;
  },
```

<br>

**使用异步的例子**

一般的，在我们使用 axios 请求后端服务器时必定会有一小段往返时间，假设我们不使用异步函数，那么就会造成还没有取回响应体就直接返回了，那么必然会得到一个空值！

使用 `try...catch...` 代码块处理响应成功与否，因为 catch 能捕获到任意层次深度的任意错误，故 axios 请求一旦失败则必被捕获！

异步执行请求保证了我们获取的响应体值为后端数据，最后的 return 也能正确的返回带值变量

```js
import http from "./http.js";

let userDatas;

// 异步函数，发送后端查询数据库得到所有用户信息
async function userDataQuery(url) {
  try {
    // 异步请求后端并拿到结构Promise
    let res = await http.get(url);
    // 将获取到的响应体中的值赋予给全局变量
    userDatas = res.data.data;
    console.log(userDatas);
  } catch (err) {
    console.log("这TMD是错的");
  }
  // 返回全局变量
  return userDatas;
}

export default {
  userDataQuery,
};
```

<br>

同志醒醒，这还没完，因为我们使用 async 构造异步函数，无论该函数返回啥，结果都是一个 promise 对象，我们只需要最后一步，对其进行数据提取并存储就完事了！！！

请看模块的 script 部分代码：

这里使用了 pinia 进行数据存储，setUsersList 是 actions 中的一个方法，它是用来设置 state 中的其中一个变量的值的，这一步操作下来即可把我们要的数据存储到 store 里面了！

因为变量 datas 已被 reactive 响应式，故 store 的更新也会带动该变量的更新，同时带动对应渲染的更新

```js
import { onMounted, reactive, ref } from "vue";
import apiQuery from "../../api/api-query.js";
import dbStore from "../../store/db-store.js";

const store = dbStore();
let datas = reactive(store.$state.userLists);

// 执行最后一步的promise处理
apiQuery.userDataQuery("/sdb/allusers").then((res) => {
  // 调用store中的方法直接把数据存储到status域中的变量
  store.setUsersList(res);
  // 随意输出一些内容作为验证
  console.log(res);
});
```

<br>

#### 对象与数组转换

执行异步 axios 请求时，对于 POST 类型的数据存储往往会遇到 formdata 是一个对象，而我们 vue 渲染时需要的偏偏就得是数组类型的

这一步可以在 store 中的 actions 配置的方法里面进行处理  
此处简单的使用了 foreach 把对象内容一一压入数组的方式实现，请注意每次执行该方法是变量值的配置！！！

```js
import { defineStore } from "pinia";
import Names from "./names.js";

const dbStore = defineStore(Names.DBSTORE, {
  // 随意配置一个数组变量userLists
  state: () => {
    return {
      userLists: [],
    };
  },

  actions: {
    // 设置变量的值，形参data即传来的对象object
    setUsersList(data) {
      // 转换前务必清空变量值，否则会造成值重复添加现象
      this.userLists = [];
      // foreach将对象中的值一个个取出来添加到数组里面
      for (let i in data) {
        console.log(data[i]);
        this.userLists.push(data[i]);
      }
    },
  },
  getters: {},
});

export default dbStore;
```

<br>

### 踩坑

#### axios 丢失响应式

> 本案例采用 `axios+elementplus` 作为演示

不知道各位有无遇到过这么一种状况，在 axios 请求中明明成功获取到了值，并且也把值赋予给了 reactive 属性，但是却发现页面并没有渲染出数据

这是因为对于 vue3，如果在 axios 请求中直接以赋值的方式修改 reactive 属性，那么该 reactive 就会丢失响应式

解决办法就是使用 push 插入数据

```html
<template>
  <div>快速备忘录 ({{ datas.notes.length }}/50)</div>
</template>

<script setup>
  import { onMounted, reactive, ref } from "vue";

  // 需要使用let而不是const，因为其属性值动态修改
  // 属性初始值最好和后端返回数据类型一致，比如后端返回数组，初始值就设定为 []
  let datas = reactive({
    notes: [],
  });
  onMounted(() => {
    server
      .get("/bk/admin/getnote")
      .then((res) => {
        // push插入数据
        datas.notes.push(...res.data.data);
      })
      .catch((err) => {
        console.log(err);
      });
  });
</script>
```

<br>

或者使用此方法

```js
let datas = reactive({
  notes: [],
});
onMounted(() => {
  server
    .get("/bk/admin/getnote")
    .then((res) => {
      // 直接赋值
      datas.notes = res.data.data;
    })
    .catch((err) => {
      console.log(err);
    });
});
```

<br>
