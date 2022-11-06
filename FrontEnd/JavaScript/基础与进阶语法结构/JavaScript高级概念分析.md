---
title: JavaScript高级概念分析
date: 2022-06-06 12:19:48
tags:

categories:
  - 前端
---

### 原型模式

**Object.defineProperty**
使用此方法为对象添加新属性或者方法；

- Object.definePropertys 可以设置多个属性，各个属性逗号隔开
- writable 定义属性可否被改变
- value 该属性的值
- configurable 若为 false 则表示不可从该对象中删除该属性

```js
var obj1 = {};
Object.defineProperty(obj1, "name", {
  writable: false,
  configurable: false,
  value: "tom",
});
alert(obj1.name);
```

<br>

**原型对象理解**
任何函数都自带一个原型对象，向一个函数中加入属性或者内部对象有两种办法：

1. 直接写入属性或者对象，如 var name: 'xxx', func: function(){}
2. 属性或方法插入原型对象，（假设当前函数名为 Demo）如 Demo.prototype.name='xxx'（注意这是在函数体外设置的！）
3. 实例化后的函数可以直接调用原型对象中存储的属性或者方法
4. 倘若在函数内定义了和原型对象中一模一样的属性或者方法，就会覆盖掉原型对象中的对应内容，此时我们再执行调用时，取得的就是函数内定义的属性或者方法
5. 使用 delete 删除函数内设置的属性或方法，就可以再次使用原型对象内的对应内容了

```js
// 往原型对象中填入属性或者方法实例
function ProtoTest() {}
ProtoTest.prototype.name = "helloworld";
ProtoTest.prototype.sayName = () => {
  alert(this.name);
};
// 实例化对象后调用的就是原型对象
var pt1 = new ProtoTest();
pt1.sayName(); // helloworld

// 新设置的值会覆盖原型对象中的默认值，不会再调用原型对象
var pt2 = new ProtoTest();
pt2.name = "hellopython";
alert(pt2.name); // hellopython

// 把实例中重新设置的属性delete后就会再次访问原型对象中的默认值了
delete pt2.name;
alert(pt2.name); // helloworld
```

hasOwnProperty 检测属性是否属于实例对象还是原型对象，只有是实例对象时才返回 true；  
Object.keys 对一个函数的原型对象执行此操作将获取所有可枚举成员；  
Object.getOwnPropertyNames 强制获取所有可枚举或不可枚举的对象；

<br>

**原型对象同时多属性方法设置**
将原型对象以大括号的形式即可同时指定多个属性或者方法；  
constructor: car 强势使构造器指向原函数（使得 constructor 变成可枚举的！）

> 关键概念：实例永远指向原型而不是构造函数本身

```js
// 一次性制定多个属性和方法
function car() {}
car.prototype = {
  constructor: car,
  name: "bmw",
  price: 99999,
  use: () => {
    alert(name + " is running now!");
  },
};
```

<br>

### 作用域链

定义：内部函数访问外部函数的变量，采取的是链式查找的方式。  
链式查找：（以以下代码为例）即内部函数调用某变量，首先逐层向外查找，找到 fOuter 函数内的变量 num1，随即采用该变量而不会使用更外层的 var 定义的全局变量！！！  
（如果上一层找不到那么就一直递归往外找，直到找到或者找不到）

> 其中，顶层 num1 fOuter 处于 0 级链； 内部 num1 fInner 处于 1 级链； console.log 处于 2 级链

```js
var num1 = 10;
function fOuter() {
  var num1 = 20;
  function fInner() {
    console.log(num1); // output 20
  }
}
```

<br>

### 预解析与代码执行

JS 编译器首先执行预解析再到代码执行->

1. 预解析：js 编译器首先将 var 提升，同时也将 function 提升到当前作用域最前面；  
   (即你可以在 var 声明前调用变量，但没法在声明 function 前调用函数！)
2. 代码执行：从上到下依次执行

> 以下代码展示了预处理与提升的小案例  
> 首先提升 var 和 function，故二者排在最前面，特别注意这里的 num2 提升后仅仅是声明而没有赋值；  
> 紧接着 function 内部也是一个作用域，也要按规则提升 nums 变量；  
> 所有提升都解决了，就开始执行赋值过程，如 nums = 200;和 num2 = 100;  
> 赋值也解决了，就开始按照顺序执行没有被处理到的代码了，譬如这里的 demo();

```js
// 提升前
var num2 = 100;
demo();
function demo() {
  console.log(nums);
  var nums = 200;
}

// 预处理后实际代码运行结果
var num2;
function demo() {
  var nums;
  console.log(nums);
  nums = 200;
}
num2 = 100;
demo();
```

<br>

### 创建对象的三种办法

1. 字面量方式：大括号键值对赋值
2. new Object 方式：等号赋值
3. 构造函数方式：比较复杂，不在这一节讲解

```js
// 字面量方式
var obj1 = {
  name: "xiaoming",
  sayHi: function () {
    console.log("hellow orld");
  },
};
// 直接点操作符调用或者方括号写属性名调用
obj1.name;
obj1["name"];

// new Object方式
var obj2 = new Object();
obj2.name = "hello world";
```

<br>

### 构造函数创建对象

使用构造函数创建对象需要牢记以下几个规则：

1. 构造函数名称首字构造函数必须使用 this 来设置内部属性和方法
2. 构造函数无序 return 即可返回结果
3. 构造函数必须使用 new 实例化对象才可以构建
4. 任何构造函数都有一个构造函数属性 constructor，他保存着被构造函数的实例对象
5. this 指向 window，所以赋值可以直接通过 window 调用，譬如 window.name
6. 以下代码中，Demo()为一个构造函数，被 new 过的 Demo()就变成对象实例

```js
// 标准构造函数创建对象的模板
function Demo(name){
    this.name = name;
    this.getName: ()=>{return name;};
}
var sf = new Demo("hello");

// 构造函数对象和普通对象无异，可以直接使用
sf.getName();

// 构造函数属性constructor和原函数直接相等
alert(sf.constructor==Person);	// true
```

<br>

**new 执行的四个步骤（面试题）**

- 内存中新建一个空对象
- 让 this 指向这个空对象
- 执行构造函数里面的代码，给这个新对象添加属性和方法
- 返回这个新对象（因为这里 new 已经帮我们做好了 return 的操作，故不需要手动 return）

<br>

### DOM 文档对象模型

文档：一个页面就是一个文档，在 DOM 中用 document 表示；  
元素：页面中的标签就是元素，用 element 表示；  
节点：网页中所有的内容都是节点，用 node 表示；

<br>

**新增 querySelector 选择元素方式**
使用本方法可以获取文档中具有指定选择器的标签，获取第一个匹配的元素或者获取所有匹配的元素;  
注意 querySelector 接受一个参数，以选择器形式写出，别丢了 . 或者 #

```js
// 选择单个
var onlyNav = document.querySelector(".div1");

// 直接选择全部具有该选择器的标签，得到一个数组，起始元素索引为0
var nav = document.querySelectorAll("#div2");
```

<br>

### DOM 事件流

事件流表示从页面中接收事件的顺序；  
事件发生会在元素之间按顺序传播，这个传播过程就叫做 DOM 事件流；

DOM 事件流分为三个主要阶段：

- 捕获阶段：从 DOM 顶级结点开始，然后逐级向下传播到具体元素接收的过程；
- 当前目标阶段
- 冒泡阶段：首先由最底层节点开始，逐级向上传播到顶级 DOM 节点

<br>

### DOM 常见元素处理操作

**document 解析**
这里将举出最常用的 document 属性供参考：

- document.documentElement 指向文档的\<html\>标签
- document.firstChild 获取文档第一个元素
- document.firstChild 获取文档子元素的第一个元素
- document.body 获取\<body\>标签
- document.doctype 获取 doctype 属性
- document.title 获取 title
- document.URL
- document.domain
- document.referrer
- document.activeElement 判断指定标签是否处于激活状态
- document.readyState 存储文档目前加载的状况（complete 完毕,loading 正在加载）
- document.createElement('') 创建新元素
- document.appendElement('') 为父元素中插入一个子元素
- document.insertBefore('') 在父元素的前面插入子元素
-

<br>

**element 解析**

- element.innerText 指向的元素去掉首尾标签后剩下的内容
- element.innerHTML 代表整个元素（即包含标签和标签包裹的所有内容）
- element.style 后面直接跟着样式，为元素设置样式，且是行内样式（优先级最高）
- element.className 直接为元素指定一现存的样式，使用此方法会覆盖掉元素原有的所有 class
- element.getAttribute() 获取属性，且可以获取自定义属性

```js
// 使用element.className修改样式
var btn = document.getElementById("btn");
btn.onclick = function () {
  this.className = "btn-style";
};

// 使用此方法获取自定义属性
btn.getAttribute("customAttribute");
```

<br>

**classList 优化**
classList 的存在就是为了优化元素中类的增删查改

```js
// add remove就是简单的增删
// toggle表示当指定的class存在时就不管，若不存在就添加
// contains判断指定class是否存在
var div1 = document.getElementById("div1");
div1.classList.add("name");
div1.classList.remove("name");
div1.classList.toggle("hello");
if (div1.classList.contains("name")) {
}
```

<br>

**getElementsByTagName 解析**

```js
// getElementsByTagName获取零个或者多个包含该标签名的标签集合，为一个htmlcollection对象
// 该对象可以做到类似数组的方式获取内部标签
// 如果一个标签设置了name属性，那么还可以通过nameItem直接找到这个标签
var tagname = document.getElementsByTagName("img");
tagname.item(0).src; // 获取第一个标签的src属性
tagname[0].src; // 同上
tagname.length; // 找到了多少个Img标签
```

<br>

**不同命令创建元素效率对比**

- document.write 直接将内容写入页面流，但当文档流执行完毕后插入，就会导致页面全部重绘
- innerHTML 不导致重绘，使用数组操作创建元素时效率高
- createElement 效率较低但是结构清晰

<br>

### BOM 浏览器对象模型

定义：他提供了独立于内容而与浏览器窗口进行交互的对象

- BOM 由一系列方法对象组成
- BOM 没有官方标准，故相对于 DOM 这种有标准的，兼容性较差
- BOM 的顶级对象是 window
-

> window 对象：  
> 它是一个全局对象，所有的全局变量或者全局方法都存储在里面，直接使用格式 window.xxx 来调用即可（但大多数时间我们都可以省略掉前面的 window）

<br>

### 事件处理

使用 addEventListener 注册监听事件；  
对于较低版本浏览器可以使用 attachEvent 来取代 addEventListener；  
接收三个参数：欲处理事件名，事件处理函数，布尔值；  
布尔值为 true 表示捕获阶段调用函数，false 表示冒泡阶段调用函数；  
（以下代码实现了点击某按钮，然后触发 click 点击事件并执行对应代码的过程）

```js
var btn2 = document.getElementById("btn2");
btn2.addEventListener(
  "click",
  function () {
    alert(this.id);
  },
  false
);
```

使用 removeEventListener 可以移除注册过的监听器，里面的参数和注册时的参数一模一样  
特别注意如果想要移除，那么注册的时候就不可以使用匿名函数！

```js
var handler = function () {
  alert(this.id);
};
var btn3 = document.getElementById("btn3");
btn3.addEventListener("click", handler, false);
btn3.removeEventListener("click", handler, false);
```

<br>

### 事件对象

每一个监听事件被触发后，相关的参数会保存在事件对象里面去

- 事件对象是监听函数的一个形参，系统自动给它赋值，我们不需要传参！
- 事件对象可以对边取名，event evt e 都可以
- e.target 返回事件触发的对象
- e.type 返回事件的类型
- e.preventDefault 阻止默认事件（如链接跳转）
- e.stopPropagation 阻止冒泡标准写法

> e.target 返回触发事件的对象，而 this 则返回挂载事件监听的元素

```js
var div = document.getElementById("name");
div.addEventListener("click", function (event) {
  console.log(event);
});
```

<br>

**事件委托**
假设我们有一个 ul，他下面有很多 li，那么我们为 ul 注册点击事件后，我们点击 li 的时候就会根据事件冒泡原理上溯到 ul 的监听事件中去，此时即可触发！

<br>

**按键检测**
这里注册了按键弹起事件 keyup（addEventListener 注册时使用的名字不用加 on，但是正常的要加 on，比如 div.onkeyup）  
从事件对象中取出按下按键的对应 ASCII 码；  
keyup keydown 事件返回 ASCII 不区分大小写，而 keypress 返回的 ASCII 区分大小写

```js
var div2 = document.getElementsByClassName("hello");
div2.addEventListener("keyup", function (e) {
  console.log(e.KeyCode);
});
```

<br>

### 定时器

使用 setTimeout 设置定时器，里面有一个回调函数（即本方法被触发后才反过来执行的函数）和一个延迟时间，延迟时间单位为毫秒；  
使用 window 自带的方法取消掉带有 ID 的定时器，匿名定时器是无法取消的！

```js
// 设置最简单的定时器，延迟3000毫秒后执行回调函数
var timer = setTimeout(() => {
  console.log("hello world");
}, 3000);

// 清除定时器
window.clearTimeout(timer);
```

或者使用 setInterval 设置间隔执行某函数

```js
var inerval = setInterval(() => {
  console.log("间隔定时器");
}, 1000);
```

<br>

### this 指向问题

- 对于全局作用域下或者定时器内，this 都指向 window
- 多层嵌套函数关系，this 则指向上一级离他最近的一个函数

<br>

### 同步异步问题

- 同步方法：即一般的方法；异步方法：带有回调函数的方法；
- 执行栈：页面加载后首先执行的任务队列，里面按顺序执行各个函数
- 任务栈（消息队列）：存放异步方法中的回调函数，当执行栈的任务执行完毕后，任务栈转变为执行栈并开始执行里面的函数
- 异步进程：当执行栈中运行到一个异步方法时，把该方法丢给异步进程处理，进程处理完后又把回调函数丢给任务栈

### Location 对象

location 对象中存储着当前网址的许多信息，可以获取他们或者进行修改  
以下通过代码介绍一些基本的参数与原理：

```js
// 直接修改URL对应模块的内容，每改一次网页就刷新一次
// 也可以直接使用类似location.hash获取对应的属性
location.href;
location.hash = "#section1";
location.search = "?q=asdasd";
location.hostname = "www.baidu.com";
location.pathname = "strname";
location.port = 8080;

// 使用reload重新加载页面，不带参数的话就默认使用最快方法加载（也就是从缓存里面加载）
// 在reload里面传入参数true就让页面强制加载
location.reload();
location.reload(true);

// 这三行代码使用效果完全一致
location.assign("http://www.baidu.com/");
location.href = "http://www.baidu.com/";
window.location = "http://www.baidu.com/";
```

<br>

### navigator 对象

其包含很多浏览器信息，最常使用它获取 useragent 属性；  
以下代码展示通过正则匹配 useragent 来判断浏览器载体为手机还是 PC

```js
var str = /(phone|pad|pod|iphone|ipod|ios|ipad|android|mobile|windows phone)/;
if (navigator.userAgent.match(str)) {
  console.log("客户端为手机");
} else {
  console.log("客户端为PC");
}
```

<br>

### window 对象

主要概念牢记

- 可以使用 window 创建全局变量，且仅有这种方式创建的全局变量才可以用 delete 删除掉
- 使用 window 来访问某个未定义的变量不会出错，他会返回一个 undefined
- moveto 表示移动到指定位置，他接受两个参数 xy
- moveby 表示在原基础上移动多少个像素，参数分别写水平和垂直方向，负数为左移上移，正数为右移下移
- innerWidth 和 innerHeight 获得浏览器宽高
- resizeto 调整到指定的宽高（接收像素值）,resizeby 表示与原宽高的差值
- confirm 显示可选提示框，按下 ok 后返回 true，按下 cancel 或者红叉后返回 false
- prompt 弹出输入框，参一为提示文字，参二为输入框默认文字；返回值为输入框内容

```js
// window创建全局变量
window.gol2 = 200;
delete window.gol2;

window.moveTo(0, 0);
window.moveBy(-50, 100);

var pageWidth = window.innerWidth;
var pageHeight = window.innerHeight;

window.resizeTo(100, 100); // 调到100x100
window.resizeBy(100, 50); // 调到200x150

// comfirm使用
if (confirm("are you sure?")) {
  alert("so pleasure that you are sure");
} else {
  alert("why you are not sure???");
}

// prompt使用
var promptinput = prompt("请输入一些文字谢谢", "这是默认的文字");
if (promptinput) alert("你刚刚输入了文字！");
```

<br>

### history 对象

该对象存储着所有用户浏览过的网站地址（但有时间限制）

- history.go() 用户页面前进后退操作，-1 后退一页，2 前进两页
- history.back()或 history.forward()实现简单的后退前进一页
- history.length 保存着用户访问过的网页个数

<br>

### 元素偏移量 offset

使用其获取子元素相对于父元素的偏移位置；

- offsetWidth 获得子元素包含 padding margin border 的相对父元素的偏移位置；
- offset 获取结果没有单位，并且可以获取任意样式表中的任意值
- style 只针对行内样式表
- 故差参数最好用 offset，赋值最好用 style

<br>

### 立即执行函数

类似匿名函数，一经创建立即执行

- 有两种书写方式，即最后的（）写在里面还是外面，两种写法的结果都是一致的
- 尾部小括号里面可以传入实参到内部函数里！
- 内部函数属于一个封闭的局部作用域，这就是闭包的原理！

```js
// 两种立即执行函数的写法
(function(){
    console.log("hello");
})();
(function(){
    ...
}());

// 尾部小括号传入实参进行匿名函数调用
(function(a,b){
    console.log(a+b);
})(1,2);
```

<br>

### 元素滚动 scroll

最常用的莫属滚动事件 onscroll 了，在 addEventListener 里面删掉前面的 on；  
（章节过大，暂时搁置）

<br>

### 动画制作一条龙

动画制作的基本原理就是使用 setInterval 不断改变元素属性，并控制时间间隔与终止条件，来让元素运动起来；  
下面的代码将让一个 div 简单的向右移动，并在距离页面左侧 500px 时停下来；

```js
var box = document.querySelector(".box");
var interval = setInterval(function () {
  // 当检测到元素距离左侧偏移大于等于500时，清除间隔事件
  if (box.offsetLeft >= 500) {
    clearInterval(interval);
  }
  // 每次元素距离左侧都加上2px，因为offsetLeft没有单位，所以需要手动添加
  box.style.left = box.offsetLeft + 2 + "px";
}, 10);
```

<br>

### call/apply/bind 方法简析

首先注意一点：call apply bind 使用效果是一样的，就是语法结构有所不同

- 共同点：三者都有更改 this 指向的能力，第一个参数传入 this 的作用域，譬如代码中传入 db，此时对象 person 中的方法 getInfo 的 this.name 就转为指向我们所给予的作用域内的 name，即 name: '小王'而不是原来 this 指代的作用域！！
- call 第二至后续参数直接用逗号分隔，用来填入被代理的函数的形参
- apply 第二至后续参数必须使用数组形式表达，作用同上
- bind 第二至后续参数逗号分割，作用同上，但注意他返回一个函数，故我们需要末尾加()执行它！
- 三者接收的参数都可以是任意类型的，没有限制！

```js
var person = {
  getInfo: function (salary, days) {
    console.log(this.name + "工作了" + days + "天，薪水" + salary);
  },
};
var db = {
  name: "小王",
};

person.getInfo.call(db, "1w", 30); // 小王工作了30天，薪水1w
person.getInfo.apply(db, ["2w", 123]); // 小王工作了123天，薪水2w
person.getInfo.bind(db, "2w", 213)(); // 小王工作了213天，薪水2w
```

<br>

### 继承
