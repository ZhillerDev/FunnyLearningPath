### 快速上手

非常简单，wx.request 请求即可

```js
Page({
  data: {
    requestMain: "",
  },

  demoRequest() {
    wx.request({
      // 请求的url
      url: config.host,

      // 请求成功钩子
      success: (e) => {
        this.setData({
          requestMain: e.data,
        });
        console.log(e);
      },

      // 请求失败钩子
      fail: (e) => {
        console.log("请求失败了！");
      },
    });
  },
});
```

<br>

简单封装 request

```js
module.exports = {
  basicRequest: (ctx, url, arr) => {
    wx.request({
      url: url,
      success: (res) => {
        ctx.setData({
          [arr]: res.data,
        });
      },
    });
  },
};
```

调用时：  
请注意这里的 `requestMain` 字符串是在 data 中定义的变量！

```js
demoRequest(){
axios.basicRequest(this,config.host,'requestMain')
}
```

<br>
