### 前言

reggie 项目已经为我们提供了完备的前台后台代码，我们仅需要编写后端代码即可实现所有功能；

目前针对后台代码做较为完善的分析，并着重划分重难点，为所有代码提供尽可能详实的注释；

<br>

### 后台文件结构分析

backend 文件夹中主要包含以下几个部分：

1. api 将接口请求抽离，便于后续 axios 直接调用
2. images 图片资源不解释
3. js 一些封装好的工具类
4. page 主要页面
5. plugins 即 vue 源码等 js 模块
6. styles 样式表不解释

> 为便于大家理解，分析顺序为 js->api->index.html->pages

<br>

### js 工具类

#### common.js

定义 url 后缀，没什么可解释的

```js
var web_prefix = "/backend";
```

<br>

#### index.js

主要用于解析 url

```js
/* 自定义trim */

// 删除左右两端的空格,自定义的trim()方法
function trim(str) {
  return str == undefined ? "" : str.replace(/(^\s*)|(\s*$)/g, "");
}

// 获取url地址上面的参数（非常实用的封装方法！建议牢记！）
// argname即我们要查找的参数名字
function requestUrlParam(argname) {
  // 取出完整url
  var url = location.href;
  // 找到从？开始到结尾的子字符串，并用&作为分隔符划分出字符串数组
  var arrStr = url.substring(url.indexOf("?") + 1).split("&");
  for (var i = 0; i < arrStr.length; i++) {
    // 如果找到了我们需要的参数，则进行格式化处理后返回参数对应的值
    var loc = arrStr[i].indexOf(argname + "=");
    if (loc != -1) {
      return arrStr[i].replace(argname + "=", "").replace("?", "");
    }
  }
  return "";
}
```

<br>

#### request.js

该文件封装了 axios 请求；

他主要实现了几个功能

1. 全局挂载 axios 实例 service
2. 设置 request 和 response 拦截器

```js
// IIFE，前端一运行即立刻注册axios
(function (win) {
  axios.defaults.headers["Content-Type"] = "application/json;charset=utf-8";

  // 创建axios实例
  const service = axios.create({
    // axios中请求配置有baseURL选项，表示请求URL公共部分
    baseURL: "/",
    // 超时
    timeout: 10000,
  });

  // request拦截器
  service.interceptors.request.use(
    (config) => {
      // 是否需要设置 token
      // const isToken = (config.headers || {}).isToken === false
      // if (getToken() && !isToken) {
      //   config.headers['Authorization'] = 'Bearer ' + getToken() // 让每个请求携带自定义token 请根据实际情况自行修改
      // }
      // get请求映射params参数
      if (config.method === "get" && config.params) {
        let url = config.url + "?";
        for (const propName of Object.keys(config.params)) {
          const value = config.params[propName];
          var part = encodeURIComponent(propName) + "=";
          if (value !== null && typeof value !== "undefined") {
            if (typeof value === "object") {
              for (const key of Object.keys(value)) {
                let params = propName + "[" + key + "]";
                var subPart = encodeURIComponent(params) + "=";
                url += subPart + encodeURIComponent(value[key]) + "&";
              }
            } else {
              url += part + encodeURIComponent(value) + "&";
            }
          }
        }
        url = url.slice(0, -1);
        config.params = {};
        config.url = url;
      }
      return config;
    },
    (error) => {
      console.log(error);
      Promise.reject(error);
    }
  );

  // 响应拦截器
  service.interceptors.response.use(
    (res) => {
      // res为后端传递过来的R对象，该对象包含三个属性，code\msg\data
      // 检测到code===0，即接口请求失败
      if (res.data.code === 0 && res.data.msg === "NOTLOGIN") {
        // 返回登录页面
        console.log("---/backend/page/login/login.html---");
        // 移除本地存储中的用户默认信息
        localStorage.removeItem("userInfo");
        // 浏览器重定向到登录页面
        window.top.location.href = "/backend/page/login/login.html";
      } else {
        // 数据正确，直接放行
        return res.data;
      }
    },
    //  接口请求失败后的处理方法
    (error) => {
      console.log("err" + error);
      let { message } = error;
      if (message == "Network Error") {
        message = "后端接口连接异常";
      } else if (message.includes("timeout")) {
        message = "系统接口请求超时";
      } else if (message.includes("Request failed with status code")) {
        message = "系统接口" + message.substr(message.length - 3) + "异常";
      }
      window.ELEMENT.Message({
        message: message,
        type: "error",
        duration: 5 * 1000,
      });
      // axios请求返回的结果默认promise
      return Promise.reject(error);
    }
  );
  // 全局挂载$axios变量为当前axios实例
  win.$axios = service;
})(window);
```

<br>

#### validate.js

数据校验模块，几乎都是正则表达式，没有背的必要性，拿来即用

```js
/*
 * 将校验规则抽离出来，异步validate的时候直接调用
 * */

// 将str与数组内两个可用用户类型作比较，大于0表示符合二者之一，返回true，即该用户是有效的
function isValidUsername(str) {
  return ["admin", "editor"].indexOf(str.trim()) >= 0;
}

function isExternal(path) {
  return /^(https?:|mailto:|tel:)/.test(path);
}

// 手机号码验证正则，不用背，直接cv
function isCellPhone(val) {
  if (!/^1(3|4|5|6|7|8)\d{9}$/.test(val)) {
    return false;
  } else {
    return true;
  }
}

//校验账号
function checkUserName(rule, value, callback) {
  if (value == "") {
    callback(new Error("请输入账号"));
  } else if (value.length > 20 || value.length < 3) {
    callback(new Error("账号长度应是3-20"));
  } else {
    callback();
  }
}

//校验姓名
function checkName(rule, value, callback) {
  if (value == "") {
    callback(new Error("请输入姓名"));
  } else if (value.length > 12) {
    callback(new Error("账号长度应是1-12"));
  } else {
    callback();
  }
}

function checkPhone(rule, value, callback) {
  // let phoneReg = /(^1[3|4|5|6|7|8|9]\d{9}$)|(^09\d{8}$)/;
  if (value == "") {
    callback(new Error("请输入手机号"));
  } else if (!isCellPhone(value)) {
    //引入methods中封装的检查手机格式的方法
    callback(new Error("请输入正确的手机号!"));
  } else {
    callback();
  }
}

function validID(rule, value, callback) {
  // 身份证号码为15位或者18位，15位时全为数字，18位前17位为数字，最后一位是校验位，可能为数字或字符X
  let reg = /(^\d{15}$)|(^\d{18}$)|(^\d{17}(\d|X|x)$)/;
  if (value == "") {
    callback(new Error("请输入身份证号码"));
  } else if (reg.test(value)) {
    callback();
  } else {
    callback(new Error("身份证号码不正确"));
  }
}
```

<br>
