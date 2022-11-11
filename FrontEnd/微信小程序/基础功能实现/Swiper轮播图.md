### 基本使用

首先需要在根目录下创建文件夹 images/swiper，用于存储轮播图图片（后续可以使用图床）

轮播图的结构很简单，就是多个 swiper-item，后续使用 wx-for 替换

```html
<view class="banner-container">
  <!-- autoplay自动播放 -->
  <!-- indicator-dots在轮播图下方显示导航点 -->
  <!-- circular开启循环播放 -->
  <!-- bindchange绑定一个方法，当轮播图切换一次就执行一次方法并传入数据 -->
  <swiper
    class="banner"
    autoplay
    indicator-dots
    circular
    bindchange="swiperchange"
  >
    <swiper-item>
      <image src="../../images/swiper/b1.png"></image>
      <view class="banner-info">123</view>
    </swiper-item>
    <swiper-item>
      <image src="../../images/swiper/b2.png"></image>
      <view class="banner-info">123</view>
    </swiper-item>
    <swiper-item>
      <image src="../../images/swiper/b3.png"></image>
      <view class="banner-info">123</view>
    </swiper-item>
  </swiper>

  <!-- 这里使用动态绑定显示当前轮到的是哪一张图片 -->
  <view>{{currentIndex+1}}/3</view>
</view>
```

<br>

这还不够，请看 js 文件

```js
Page({
  data: {
    // 定义当前轮播到的序号
    currentIndex: 0,
  },
  swiperchange(e) {
    // 代码很简单，获取当前轮播到的序号并改变值
    this.setData({
      currentIndex: e.detail.current,
    });
  },
});
```

<br>
