---
title: Axios速通
date: 2022-06-11 22:40:25
tags:

categories:
- 前端
---


### XMLHttpRequest(XHR)简介
以下案例展示了一个最经典的XHR异步请求示例，具体详细内容可以查看WDN官网：https://developer.mozilla.org/zh-CN/docs/Web/Guide/AJAX/Getting_Started  

1. 首先获取id为btn的按钮的DOM节点，并为之添加点击监听事件；
2. 实现点击后回调函数makeXHR内部构造；
3. 实例化XMLHttpRequest对象，并检测实例化是否成功，如果失败则直接跳过所有步骤宣布失败；
4. 实例化完毕后立即open一个网站，注意你使用的请求方法，然后send发送请求；
5. onreadystatechange 检测并分析响应内容；  
readyState检测服务端连接状态，XMLHttpRequest.DONE表示服务端成功响应  
xhr.status检测响应状态码，200表示成功！
6. 成功收到响应后便可使用responseText将所有响应内容转化为文本形式显示


```js
// 使用直接执行函数
(function () {
    var xhr;
    document.getElementById('btn').addEventListener('click', makeXHR);

    // 按钮点击监听事件
    function makeXHR() {
        xhr = new XMLHttpRequest();

        // xhr实例创建失败后的判断
        if (!xhr) {
            alert('无法创建XHR实例');
            return false;
        }

        // 首先open打开需要请求的网站，切记不可忽略www，然后send发送数据！
        xhr.open('GET', 'http://localhost:10086/serve');
        xhr.send();

        xhr.onreadystatechange = function () {
            // readyState检测链接完成状态，DONE表示服务器响应收到了并且无任何差错
            if (xhr.readyState === XMLHttpRequest.DONE) {
                // 接收状态码200表示ACK
                if (xhr.status === 200) {
                    // 直接获取GET到的文件内容
                    alert(xhr.responseText);
                } else {
                    alert('错误！无法获取request内容');
                }
            }
        }
    }
})()
```

<br>

### 万金油axios模板
axios是为了方便ajax请求而存在的；  
1. 导入axios包
2. 直接使用axios实例，传入对应请求方法和请求地址，data为传入数据（这可不是params!!!）
3. then内有回调函数response，其存储着我们获取到的响应数据

```js
const axios = require('axios');

axios({
    method: 'GET',
    url: 'http://www.xxx.com/',
    data: {
        name: 'xxx'
    }
}).then(
    response => {
        console.log(response);
    }
)
```

<br>

### 修改默认配置项
axios自带了很多请求配置，具体请查看官方文档：https://www.axios-http.cn/docs/req_config  
直接使用axios.defaults.xxx的形式设置全局配置项  
或者如第二行代码，创建实例后直接在实例内部覆写配置项

```js
// 全局默认配置项
axios.defaults.baseURL='http://www.zhiyiyi.com/'

// 实例内部配置项
const instance = axios.create({
  baseURL: 'https://api.example.com'
});
```

<br>

### 拦截器
即针对请求和响应是的各个周期做出的反应；  
axios.interceptors.request.use 设置全局的，请求时的拦截器  
axios.interceptors.response.use 设置去哪聚的，响应时拦截器  

```js
axios.interceptors.request.use(
    config => {
        // 发送请求前做些什么
        return config;
    }, error => {
        // 请求出问题后做些什么
        return Promise.reject(error);
    }
);

// 响应拦截器
axios.interceptors.response.use(
    response => {
        // 当响应发出前做些什么
        return response;
    }, error => {
        // 响应出错时做些什么
        return Promise.reject(error);
    }
);
```

<br>

当拦截器作为一个实例存在时，即可使用eject方法来移除指定的拦截器
```js
// 删除拦截器
const myInterceptor = axios.interceptors.request.use(function () {/*...*/});
axios.interceptors.request.eject(myInterceptor);
```

<br>

为自定义axios实例挂载对应拦截器
```js
// 自定义实例挂载拦截器
const instance = axios.create();
instance.interceptors.request.use(function () {/*...*/});
```

<br>

### 错误拦截
使用catch配合对应箭头函数（或function函数）即可捕获异常
```js
axios.get(url, params)
    .catch(err => {
        console.error(err);
    })
```

<br>

### 取消请求
不多说废话，直接看代码你就懂了
```js
const controller = new AbortController();

axios.get('/foo/bar', {
   signal: controller.signal
}).then(function(response) {
   //...
});
// 取消请求
controller.abort()
```
