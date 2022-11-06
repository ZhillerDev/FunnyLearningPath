---
title: Call-Bind-Apply分异
date: 2022-08-31 10:38:43
tags:

categories:
  - 前端面试八股
---

### Call

call 函数和 apply 函数都可以改变函数的上下文，说人话就是改变 this 指向
代码中 this 指向 window，而 o 指向对象 o

```js
window.color = "red";
var o = { color: "blue" };
function getColor() {
  console.log(this.color);
}
getColor(); // red
getColor.call(this); // red
getColor.call(o); // blue
```

<br>

可以判断对象类型

```js
var arr = [];
Object.prototype.toString.call(arr); // [object Array]
```

<br>

#### 带参数的 call

call 第一个参数为上下文，后面的参数即为传入指定函数的形参

```js
var a = {
  name: "lily",
};
function b(name) {
  console.log(name);
  console.log(this.name);
}
b.call(a, "john");
// john
// lily
```

可以对匿名函数使用 call，以此为匿名函数指定一个上下文

```js
var car = {
  price: 99999,
};
(function (price) {
  console.log(price);
  console.log(this.price);
}.call(car, 100000));
// 100000
// 99999
```

<br>

#### 手写 call

```js
Function.prototype.myCall = function (context) {
  // 取得传入的对象（执行上下文），比如上文的foo对象，这里的context就相当于上文的foo
  // 不传第一个参数，默认是window,
  var context = context || window;
  // 给context添加一个属性，这时的this指向调用myCall的函数，比如上文的bar函数
  context.fn = this; //这里的context.fn就相当于上文的bar函数
  // 通过展开运算符和解构赋值取出context后面的参数，上文的例子没有传入参数列表
  var args = [...arguments].slice(1);
  // 执行函数（相当于上文的bar(...args)）
  var result = context.fn(...args);
  // 删除函数
  delete context.fn;
  return result;
};
```

<br>

### Apply

#### 基本用法与和 call 区别

用法和 call 基本一致，只不过第二个参数必须是一个数组，该数组将作为参数一一对应指定的函数中形参

> 只要上下文传入的是 this null undefined 或者其他非指向性的无意义内容，都会使函数自动默认指向 window

```js
function position(x, y, z) {
  console.log("the position is:", x, y, z);
}
position.apply(undefined, [1, 2, 3]); // the position is: 1 2 3
```

<br>

#### 一些 apply 骚操作

使用 apply 搭配 Math，做到清晰简单取最大最小值

```js
/* 使用 Math.min/Math.max 在 apply 中应用 */
var max = Math.max.apply(null, numbers);
// 一般情况是用 Math.max(5, 6, ..) 或者 Math.max(numbers[0], ...) 来找最大值
var min = Math.min.apply(null, numbers);
```

数组追加

```js
var arr1 = [1, 2];
var arr2 = [3, 4];
[].push.apply(arr1, arr2);
arr1; // [1,2,3,4]
```

数组合并

```js
var arr1 = [1, 2];
var arr2 = [3, 4];
Array.prototype.push.apply(arr1, arr2);
// 或者使用简单的办法：var arr = arr1.concat(arr2);
```

<br>

#### 手写 apply

```js
Function.prototype.myApply = function (context) {
  var context = context || window;
  context.fn = this;
  var result;
  // 判断第二个参数是否存在，也就是context后面有没有一个数组
  // 如果存在，则需要展开第二个参数
  if (arguments[1]) {
    result = context.fn(...arguments[1]);
  } else {
    result = context.fn();
  }
  delete context.fn;
  return result;
};
```

<br>

### Bind

#### bind 特性与逻辑简介

bind 的结果是一个新的，未执行的函数；
call、apply 是在改变上下文后立即执行该函数！

```js
var info = {};
function getInfo() {}
getInfo.apply(info); // 立即执行
getInfo.bind(info)(); // 再次调用即可执行
```

<br>

利用 bind 特性，使其和事件监听结合使用

```js
var obj = {
  name: "helloworld",
};
// 因为bind返回新函数，所以可以直接放在addEventListener内部使用
window.addEventListener("dblclick", dbClick.apply(obj), false);
function dbClick() {
  console.log(this.name);
}
```

<br>

#### 手写 bind

```js
Function.prototype.myBind = function (context) {
  if (typeof this !== "function") {
    throw new TypeError("Error");
  }
  var _this = this;
  var args = [...arguments].slice(1);
  // 返回一个函数
  return function F() {
    // 因为返回了一个函数，我们可以 new F()，所以需要判断
    if (this instanceof F) {
      return new _this(...args, ...arguments);
    }
    return _this.apply(context, args.concat(...arguments));
  };
};
```

<br>

### 原型扩展

在原型函数上使用 call 或者 apply 进行扩展，不会污染原函数！

```js
function test() {
  // 重新指定forEach方法的含义
  Array.prototype.forEach.call(arguments, (item) => {
    console.log(item);
  });
}
```
