---
title: Blob深入解析与放弃
date: 2022-08-27 07:00:52
tags:

categories:
  - 前端面试八股
---

### 什么是 Blob

**Blob（Binary Large Object）表示二进制类型的大对象**
在 JavaScript 中 Blob 类型的对象表示不可变的类似文件对象的原始数据

<br>

### 构造函数

#### blob 组成图解

![图片](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2020/6/2/172734410c51dbed~tplv-t2oaga2asx-zoom-in-crop-mark:3024:0:0:0.awebp)

以上是一个 Blob 内部的两个重要的组成内容，下面将介绍对应的内容所有包含的意思：
`type` 指代当前 blob 的 MIME 类型
`Blob` 指代包含于此 Blob 的另一个 Blob
`ArrayBuffer` 存放我们传入的数据
`DOMString` 不知道有什么用

<br>

#### 使用 blob 的标准方法

参数一：接收一个数组，表示欲存入的数据
参数二：接收一个对象，对象有俩属性 type 和 ending，ending 表示 blob 存储数据的结尾符号

> type 接收的 MIME 类型有：
> text/plain 文本类型，text/html 超文本标记语言，image/png 图片类型

```js
var myBlobPart = ["helloworld"];
const blob = new Blob(myBlobPart, {
  type: "text/plain",
  endings: "transparent",
});
```

![](https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2020/6/2/172734410fb92412~tplv-t2oaga2asx-zoom-in-crop-mark:3024:0:0:0.awebp)

<br>

#### Blob 的基本方法

我们**无法修改**已经被定义的 blob 中的数据；
但是可以将旧的 blob 融合到新的 blob 里面，实现字符串的扩展操作

1. slice([start[, end[, contentType]]])：返回一个新的 Blob 对象，包含了源 Blob 对象中指定范围内的数据。
2. stream()：返回一个能读取 blob 内容的 ReadableStream。
3. text()：返回一个 Promise 对象且包含 blob 所有内容的 UTF-8 格式的 USVString。
4. arrayBuffer()：返回一个 Promise 对象且包含 blob 所有内容的二进制格式的 ArrayBuffer。

<br>

### Blob 实战使用

#### 云端下载图片

首先 fetch 发送 request 请求该图片；
拿到的返回数据转换成 blob 类型，后续使用 then 得到该 blob；
直接提取该 blob 中的 url 后作为网页中 image 标签的 src

```js
const myImg = document.querySelector(".img");
const myRequest = new Request("pig.jpg");

fetch(myRequest)
  .then((rep) => {
    return rep.blob();
  })
  .then((myBlob) => {
    let url = URL.createObjectURL(myBlob);
    myImg.src = url;
  });
```

<br>

#### Blob 映射

每一个 blob 对象都可以转换成一个 独有的 url，叫做 blob url
该 url 很短且仅在文档打开的情况下才生效，必须要手动释放其占用的内存

> URL.createObjectURL 用来创建，而 URL.revokeObjectURL 用来释放

```js
const blob1 = new Blob(["asd"], { type: "text/plain", endings: "transparent" });
// 创建blob的专属url
const url = URL.createObjectURL(blob1);
// 释放blob url占用的内存
URL.revokeObjectURL(url);
```

<br>

#### 模拟 blob url 下载文件

整体思路：
点击按钮后模拟一个链接标签 a，为其指定 blob url 后执行点击操作，实现文件的下载
blob url 执行完毕后必须进行内存释放，使用 URL.revokeObjectURL

```js
<body>
    <button class="download">下载文件</button>
    <script>
      const downloadInit = (fileName, blob) => {
        // 创建一个链接元素a，模拟实现添加url并点击后移除的全步骤
        const link = document.createElement("a");
        link.href = URL.createObjectURL(blob);
        link.download = fileName;
        link.click();
        link.remove();
        URL.revokeObjectURL(link.href);
      };

      const btn = document.querySelector(".download");
      btn.addEventListener("click", (evt) => {
        const fileName = "asd.txt";
        const blobs = new Blob(["第一次使用blob"], { type: "text/plain" });
        downloadInit(fileName, blobs);
      });
    </script>
  </body>
```

<br>

#### 辅助图片压缩

```html
<body>
  <input type="file" accept="image/*" onchange="loadFile(event)" />
  <script src="./compress.js"></script>
  <script>
    const loadFile = function (event) {
      // 使用FileReader读取文件
      const reader = new FileReader();
      // 设置读取器加载时操作
      reader.onload = async function () {
        let compressedDataURL = await compress(reader.result, 90, "image/jpeg");
        let compressedImageBlob = dataUrlToBlob(compressedDataURL);
        uploadFile("https://httpbin.org/post", compressedImageBlob);
      };
      reader.readAsDataURL(event.target.files[0]);
    };
  </script>
</body>
```

<br>

#### 上传本地图片并显示在网页端

使用 input 获取本地上传的图片文件；
之后将图片文件转换成 blob url，并将其指定为 img 的 src，即可显示出图片

```html
<body>
  <input type="file" accept="image/*" onchange="getFile(this)" />
  <img style="width: 200px; height: 200px" class="img" />

  <script>
    function getFile(input) {
      // 获取到input组件的文件并转换成blob url
      var file = input.file[0];
      var blob = URL.createObjectURL(file);
      var img = document.querySelector(".img");
      // 设置图片地址为Blob url
      img.src = blob;
      // 图片加载完毕后释放blob url
      img.onload = () => {
        URL.revokeObjectURL(this.src);
      };
    }
  </script>
</body>
```

<br>

### Blob vs ArrayBuffer

`ArrayBuffer` 对象用于表示通用的，固定长度的原始二进制数据缓冲区
`Blob` 类型的对象表示不可变的类似文件对象的原始数据

- Blob 对象是不可变的，而 ArrayBuffer 是可以通过 TypedArrays 或 DataView 来操作
- ArrayBuffer 是存在内存中的，可以直接操作
- 你可能仍需要 FileReader 之类的 File API 才能与 Blob 一起使用
- Blob 与 ArrayBuffer 对象之间是可以相互转化的
