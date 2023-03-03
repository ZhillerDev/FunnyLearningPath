### 回到顶部

很简单，直接使用微信内置 API `pageScrollTo` 即可

duration 设置平滑的滚动时间

```js
// 悬浮按钮回到顶部
  fabBack2Top(){
    wx.pageScrollTo({
      duration: 500,
      scrollTop:0
    })
  },
```

<br>
