### 小节 1

题目 1：假设 el 为一 DOM，则 `el.tagName='123'` 是错误的  
tagName 属性返回元素的标签名，不可以用它设置元素 tag

<br>

题目 2：解析下方代码输出（已给出详细注释）

```js
let A = function () {};
A.prototype.a = 1;
//此时 A.prototype = { a: 1 }

let B = new A(); //此时 B = {}
//在创建B时，已将B._proto_ = A.prototype = { a: 1 }
//即使后面A.prototype重新赋值，将A.prototype开辟了新的空间指向别的对象
//B._proto_并没有改，还是指向{a:1}这个对象

A.prototype = {
  //此时 A.prototype = { b: 2, c: 3 }
  b: 2,
  c: 3,
};
let C = new A(); //C = {}
A.prototype.d = 4; //此时A.prototype = { b: 2, c: 3, d: 4 }

console.log(B.a); //1
console.log(B.b); //undefined
console.log(C.c); //3
console.log(C.d); //4

//C.d
//着重区分： A.prototype.d = 4 和 A.prototype 重新赋值 不是一个概念
//A.prototype重新赋值时，A.prototype已经指向另一个对象了
//A.prototype.d = 4时，访问的还是同一个A.prototype 对象
```

<br>

题目 3：下列那些方法会改变原数组，那些不会？

```js
// 会改变原数组
push();
pop();
shift();
unshift();
splice();
sort();
reverse();
forEach();

// 不会改变原数组
filter();
concat();
slice();
map();
```

<br>

题目 4：这段代码正确吗？ `2.toString()`  
错误！因为 js 会将句点解析为小数点从而报错；  
我们可以这样写： `2..toString() 或者 2 .toString()`

<br>

题目 5：0/0 与 1/0  
这俩玩意在 JS 里面都不会报错，前者返回 NaN，不会被 catch 捕获

<br>

题目 6：求以下函数执行结果（题目+答案写在一块了）

```js
// 题目
(function () {
  var a = (b = 5);
})();
console.log(b);
console.log(a);

// 答案
// 输出：5，Uncaught ReferenceError: a is not defined
// 因为 var a = b = 5 即 var a=5；b=5；函数环境中不带var默认为全局变量
```

<br>

题目 7：NOSCRIPT 标签是做什么用的  
用来定义在脚本未被执行时的替代内容

<br>

题目 8：请说一下有几种弹出窗口的方式？  
confirm()显示的是确认框  
alert()显示的是警示框  
prompt()显示的是对话框  
open()用于打开新的窗口或者寻找已命名的窗口

<br>

题目 9：判断下列声明那些是错的？

```js
var a = ();     // 错误
var b = [];     // 声明数组
var c = {};     // 声明对象
var d = / /;    // 声明正则
```

<br>

题目 10：NaN 特殊性质  
NaN 无论与谁比较，包括它自己，结果都是 false  
譬如`NaN == NaN // false`

<br>

### 小节 2

题目 11：求 `var foo = "10"+3-"1"` 中 foo 的结果？

```js
// 为任意的数据类型+""即可将其转换成String字符串类型
// 为任意的数据类型 -0 即可将其转换成Number数值类型

var foo = "10" + 3 - "1";
console.log(foo); // 102
```

<br>

题目 12：
