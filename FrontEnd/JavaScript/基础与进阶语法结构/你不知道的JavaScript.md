### 作用域与闭包

#### LHS 引用与 RHS 引用

LHS 引用类似于左值引用，即我们要赋值给哪一个变量，此时值是主动的，他寻找变量！  
如 `const a = 2` 其中值 2 主动赋值给左侧的 a 变量

RHS 引用类似于右值引用，即寻找赋值操作的源头，此时值是被动的，他需要找到发起赋值请求的主动变量；  
如 `fn(2)` 其中 2 作为实参传入函数，他需要找到被赋值的形参

> 不能简单地理解为：等号左侧是变量就是 LHS，等号右侧时变量就是 RHS

<br>

#### ReferenceError 异常

一般的，无论 LHS 还是 RHS，在当前作用域找不到对应变量时就会不断往外层作用域查询，直到找到为止；

`非严格模式下`，若最外层仍然找不到，系统自动创建对应名称全局变量，赋予未定值并返回；  
`严格模式下`，若最外层找不到，直接抛出 ReferenceError 异常

试图对一个非函数类型的值进行函数调用，或者引用 null 或 undefined 类型的值中的属性，那么引擎会抛出 TypeError 异常

<br>

#### 欺骗词法作用域

> 尽量不要使用欺骗，有证明将会导致性能下降

词法作用域可以简单的理解为上一节讲述的作用域概念；

使用 eval()方法，在代码中的某处再插入一行代码，让编译器认为该行代码是原本存在的，即可达到欺骗效果；  
如下代码，通过 eval 插入定义变量 b，让下一行代码中的 b 只在本层中找到变量 b，而不向外层查找，此即为欺骗

```js
function foo(str, a) {
	eval(str);
	console.log(a, b);
}
var b = 2;
foo("var b = 3", 1); // 1,3
```

<br>

#### 作用域变量冲突

如下方代码将会导致死循环，因为每次执行 bar 函数，都会设置 i=3，且这个 i 会被默认指定为 for 循环中的 i，那么永远存在 i==3<10，所以必定无限循环！！！

```js
function foo() {
	function bar(a) {
		i = 3; // 此处的i会被误指定为for中的i
		console.log(a + i);
	}
	for (var i = 0; i < 10; i++) {
		bar(i * 2); // 无限循环
	}
}
```

<br>

#### 函数术语释疑

函数声明：即一个正常的函数 `function foo(){}`  
函数表达式：开头关键词不是 function 的任意一个函数 `(function(){})()`  
匿名函数表达式：function(){} 或者 ()=>{}

立即执行函数表达式 IIFE：(function(){})()

<br>

#### 立即执行函数详解

有效避免变量污染，IIFE 函数内部的表达式访问范围仅限于该函数，而不会外泄：

```js
var a = 2;
(function () {
	var a = 3;
	console.log(a); // 3
})();
```

可以为 IIFE 函数传入一个参数，以便调用外部的变量  
下面将 window 作为实参传给了内部函数 IIFE 的形参 global

```js
var a = 2;
(function IIFE(global) {
	var a = 3;
	console.log(a); // 3
	console.log(global.a); // 2
})(window);
```

<br>

可以倒置函数的运行过程；  
将小括号里面的 def 函数作为参数传递给内部 foo 函数，然后 foo 再次调用并执行 def

```js
var a = 2;
(function IIFE(def) {
	def(window);
})(function def(global) {
	var a = 3;
	console.log(a); // 3
	console.log(global.a); // 2
});
```

> 可以把 IIFE 函数末尾的小括号写在外面 `(function(){})()`  
> 或者写在里面 `(function(){}())`  
> 二者的运行结果一致，但大家普遍喜欢写在里面

<br>

#### 块作用域

一个普通的块内的函数、变量，都会在执行完毕后自行垃圾回收掉

```js
{
    let a = 1;
    function(){}
}
```

<br>

#### 作用域提升

对于 var 声明的变量，它具有作用域提升，但提升的只是声明，而真正的赋值操作仍然保留在原地；  
故以下代码输出 undefined，因为真正的赋值在我们使用后

```js
console.log(a); // undefined

var a = 10;
```

函数声明优先于变量声明；  
顾名思义，同名的函数/变量，会优先把函数提升到顶层，然后再到变量声明

```js
foo(); // 1

var foo;

function foo() {
	console.log(1);
}

foo = function () {
	console.log(2);
};
```

<br>

#### 作用域闭包

所谓闭包，即能持有对内部作用域的访问，且该引用不会被垃圾回收掉！  
如下代码就展示了闭包的使用效果

1. 通过执行 foo()得到一个 bar()函数作为返回值
2. 然后执行 baz()来运行对应的 bar()
3. 此刻我们可以看到 bar()函数能轻松地访问到内部函数作用域中的 a 值，并且因为变量 baz 的存在，永远不会 GC 掉这个作用域

> 这就是闭包的基本原理，其他的代码也可以实现，只要记住这个基本原则即可！

```js
function foo() {
	var a = 2;

	function bar() {
		console.log(a);
	}

	return bar;
}
var baz = foo();
baz(); // 2
```

<br>

因为 settimeout 倒计时完毕，是吧事件添加到事件触发线程的任务队列中去，等到整个 JS 执行线程空了才会执行事件触发线程；  
所以我们需要在 for 中使用一个 IIFE 函数来保证最终的结果是正确的；

```js
for (var i = 1; i <= 5; i++) {
	(function (j) {
		setTimeout(() => {
			console.log(j);
		}, j * 1000);
	})(i);
}
```

> 如果不使用 IIFE 函数，直接写 settimeout，那么最终结果输出 5 个 6，而不是 1-5 逐一输出！

<br>

#### 模块

对于一个模块，在 return 中使用对象式写法，将内部对函数暴露出来，使得外部函数可以调用内部函数的词法作用域！  
但是不可以暴露模块内的变量；

```js
function CustomModule() {
	var something = "none";
	function doSomething() {
		console.log(something);
	}
	return {
		doSomething: doSomething,
	};
}

var module = CustomModule();
module.doSomething(); // none
```

> 模块模式必备的两个条件：
>
> 1. 必须有外部的封闭函数，该函数必须至少被调用一次
> 2. 封闭函数必须返回至少一个内部函数，这样内部函数才能在私有作用域中形成闭包，并且可以访问或者修改私有的状态

<br>

### this

<br>

#### 默认绑定

默认绑定适配 `独立函数调用`  
默认绑定 this 指向全局对象；  
故直接调用函数，该函数内部的 this 即指向全局对象；

> 以上情况针对的是非严格模式下，而严格模式下的 this 默认绑定为 undefined

```js
function foo() {
	console.log(this.a);
}
var a = 2;
foo(); // 2
```

<br>

#### 隐式绑定

隐式绑定格式： `xxx.func()` 或者 `xxx.xxx.func()`
无论 func()前面有多少个对象，func 的 this 指向永远是离他最近的一个对象
譬如下方代码， `obj1.obj2.foo()` 中 foo()的 this 就指向 obj2

```js
function foo() {}
var obj2 = {
	a: 42,
	foo: foo,
};
var obj1 = {
	a: 2,
	obj2: obj2,
};
obj1.obj2.foo(); // 42
```

<br>

#### 隐式丢失

使用函数别名套娃太多次就会造成隐式丢失现象！  
如下方代码，bar 中的 foo 是直接使用了函数别名，而不是调用 foo()  
最终我们使用 bar()造成了隐式丢失，在非严格模式下取得了全局对象；

> 可以这么理解
> bar()相当于 (obj.foo)() 而不是我们想象的 obj.foo()，故前者遵照我们的默认绑定原则，必然会指向全局对象了！

```js
function foo() {}
var obj = {
	a: 2,
	foo: foo,
};
var bar = obj.foo;
var a = 100;
bar(); // 100
```

<br>

#### 显示绑定

即使用 call apply bind 进行绑定，人为指定上下文 this  
如下方代码，对 foo.call 人为将 this 指向对象 obj，故调用函数 foo 后，是出结果为 2

```js
function foo() {
	console.log(this.a);
}
var obj = {
	a: 2,
};
foo.call(obj);
```

硬绑定：即一旦对一个对象使用 call 或者其他函数绑定后，无论后续怎么修改都不会改变原来绑定的对象；  
apply 和 call 作用一致，好好看代码，理清逻辑关系！！！

```js
function foo(sth) {
	console.log(this.a + sth);
	return this.a + sth;
}
var obj = {
	a: 2,
};
var bar = function () {
	return foo.apply(obj, arguments);
};
var b = bar(3); //2 3
console.log(b); //5
```

<br>

#### API 调用上下文

一个很抽象的例子，使用了结构赋值的方法

```js
function foo(el) {
	console.log(el, this.id);
}
var obj = {
	id: "hello",
};
[(1, 2, 3)].forEach(foo, obj); // 1hello 2hello 3hello
```

<br>

#### new 绑定

经典面试题：当我们使用 new 时，发生了什么？

1. 创建（或者说构造）一个全新的对象。
2. 这个新对象会被执行 `[[Prototype]]` 连接。
3. 这个新对象会绑定到函数调用的 this。
4. 如果函数没有返回其他对象，那么 new 表达式中的函数调用会自动返回这个新对象。

下面就是一个很简单的构造函数例子；  
构造函数内部使用 this 直接指向其内部的 constructor 存储区，那么后续调用 bar.a 的时候，就是取出 constructor 存储区的内容，而非原型对象的内容

```js
function foo(a) {
	this.a = a;
}
var bar = new foo(2);
console.log(bar.a); // 2
```

<br>

#### this 绑定优先级

> 柯里化(currying) -> 参考 bind 硬绑定
> 指把接受多个参数的函数变换成接受一个单一参数(最初函数的第一个参数)的函数，并且返回接受余下的参数且返回结果的新函数的技术

绑定优先级： 硬绑定 > new 绑定 > 隐式绑定 > 默认绑定

<br>

#### 其余绑定例外

当你使用 call 进行绑定的时候传入的是一个 null 或者 undefined，那么就自动使用默认绑定规则替代；
`foo.call(null)`

DMZ 非军事化区；  
即将 this 绑定到一个空的费委托对象 DMZ 上去，不会对程序产生任何影响

```js
function foo(a, b) {
	console.log(a + b);
}
var dmz = Object.create(null);
foo.apply(dmz, [2, 3]);
var bar = foo.bind(dmz, 2);
bar(3); // 2 3
```

<br>

### 对象

#### 字面量与对象

var 创建字面量而非对象，而仅有对象才可以使用诸如长度访问、文字操作等方法；  
而引擎会自动把 var 声明的字面量自动转换成对象，而无需显式构造：

```js
var str = "helloworld"; // 字面量
console.log(str.length);
```

**俩特例：**  
null 和 undefined 没有对应的构造形式，它们只有文字形式；  
Date 只有构造，没有文字形式；

> 现在有两个取对象中属性的写法：  
> `obj['name']` 或者 `obj[0]`
> 事实上，引擎都会先把下标转换成字符串类型然后才取，且对象中的属性名最终都会变成字符串类型  
> 故存在 `obj[0]===obj['0']`

<br>

#### 属性描述符

ES5 新增，为对象内的所有属性都配备属性描述符，使用 `getOwnPropertyDescriptor` 获取它们

```js
var obj = {
	a: 2,
};

console.log(Object.getOwnPropertyDescriptor(obj, "a"));
// { value: 2, writable: true, enumerable: true, configurable: true }
```

或者使用 `defineProperty` 人工定义属性描述符

```js
Object.defineProperty(obj, "a", {
	value: 10,
	writable: true,
	configurable: true,
	enumerable: true,
});
```

> **属性简介**  
> writable 决定是否可以修改属性的值
> Configurable 只要属性是可配置的  
> enumerable 属性可否被枚举

<br>

**几个常用的属性描述符操作**  
Object.preventExtensions 禁用为对象添加新属性  
Object.seal 对象不能添加属性，亦无法重新配置或删除任何属性  
Object.freeze 在 seal 基础上设置 writable:false

```js
Object.preventExtensions(obj);
Object.seal(obj);
Object.freeze(obj);
```

<br>

#### 迭代器遍历

使用内置的 `@@iterator` 来遍历数组  
ES6 中的符号 `Symbol.iterator` 来获取对象的 `@@iterator` 内部属性

```js
var arr = [1, 2, 3];
var it = arr[Symbol.iterator]();

it.next(); // {value:1,done:false}
it.next();
```

<br>

### 类

#### 一些常见原理

在继承或者实例化时，JavaScript 的对象机制并不会自动执行复制行为；

多态：JS 中的多态，子类是只能通过 super 得到父类的`一个复制`，而非父类的原本实力对象，所以子类只要不重写父类对应的方法，那么两个类之间就完全没有影响；

JS`没有`多继承机制！

<br>

#### 混入

即从一个对象中复制所有属性到一个新对象里面去；  
这是一个最简单的混入代码：

```js
function mixin(source, target) {
	// 遍历原对象的所有属性
	for (var ket in source) {
		// 若新对象内和原对象属性没有重合，那么复制
		if (!(key in target)) {
			target[key] = source[key];
		}
	}
	// 返回新对象
	return target;
}
```

<br>

### 行为委托

#### 委托理论

即定义一个父对象，并定义很多个子对象来存储对应的数据和行为，把特定的任务对象都关联到父对象上，让子对象们在需要的时候可以进行委托；

下面是一个简短的委托代码  
Task 定义了一个父对象；
SubTask 使用 Object.create 所创建，这样就委托了父对象 Task；  
子对象 SubTask 即可添加新的方法或者任意属性了！

> 注意：
> 委托者：SubTask，委托目标：Task
> 为 SubTask 新增的方法 prepareTask 是存储在 SubTask 内部的！
> 委托者必须尽量少的重写委托目标中的方法或者属性，否则会导致引用歧义

```js
Task = {
	setID: function (ID) {
		this.id = ID;
	},
	outputID: function () {
		console.log(this.id);
	},
};
// 让SubTask委托Task
SubTask = Object.create(Task);
SubTask.prepareTask = function (ID, label) {
	this, setID(ID);
	this.label = label;
};
```

<br>

其他注意事项

1. 无法在两个或两个以上互相（双向）委托的对象之间创建循环委托

<br>

#### 类与对象

ES6 提供的 class 实际上依然是通过 prototype 机制实现的，因为 js 实际上并没有真正意义上的类

例子：创建一个 widget 类，并且其下有一个子类 button
对相关联风格委托方式写法：(下方代码中的单个$符号是我们引用 jQuery 后使用的！)

> 使用委托设计模式，可避免丑陋的显示伪多态调用！

```js
var Widget = {
	init: function (width, height) {
		this.width = width || 50;
		this.height = height || 50;

		// $elem代表一个元素，这里先初始化为null
		this.$elem = null;
	},

	// 向指定位置插入元素，并且为元素赋予默认的width和height
	insert: function ($where) {
		if (this.$elem) {
			this.$elem
				.css({
					width: this.width + "px",
					height: this.height + "px",
				})
				.appendTo($where);
		}
	},
};

var Button = Object.create(Widget);

Button.setup = function (width, height, label) {
	// 委托调用
	this.init(width, height);
	this.label = label || "default";

	// 使用jQuery中的$来向指定元素插入text属性！
	this.$elem = $("<button>").text(this.label);
};
```

<br>

#### 更妙的设计与语法

类实例内省：通过创建方式来判断对象的结构和功能。
以下代码展示了通过 instanceof 来推测对象的功能，也就是内省过程

```js
function Foo() {}
Foo.prototype.method = function () {};
var a1 = new Foo();
if (a1 instanceof Foo) {
	a1.method();
}
```

<br>

### 类型

#### 冷门关键词

因为 typeof 返回对应数据的类型，是一个字符串，所以一下结果毅然返回 string
`typeof typeof 43`

undefined!=undeclared
然而，使用 typeof 取 undefined 和 undeclared 的值返回的都是 undefined

```js
var a;
a; // undefined
b; // undeclared
```

<br>

#### typeof 防范机制

可以使用 typeof 检测一个全局变量是否是 undefined，如果是的话就重新声明一个！
`typeof function !== "undefined"`

<br>

### 值

在 ES6 中（早期版本是没有 SAFE 和这个修饰的！）：
整数的最大值：`Number.MAX_SAFE_INTEGER`
整数的最小值：`Number.MIN_SAFE_INTEGER`

NaNs 是特殊值，它和自身不相等，是唯一一个非自反值
即存在 `NaN != NaN` 返回 true

<br>

### 原生函数

#### 访问内部属性

访问对象的内部属性需要使用以下方法

```js
Object.prototype.toString.call("123");
// 返回 [Object string]
```

<br>

### 强制类型转换

#### 基本概念

类型转换发生在静态类型语言的编译阶段；  
强制类型转换则发生在动态类型语言的运行时（runtime）

<br>

#### JSON

JSON.stringify 用来把 JSON 文件字符串化，调用方法 tostring
字符串化的结果永远是字符串，所以会出现下面的双双引号结果

```js
JSON.stringify(42); // 42
JSON.stringify("42"); // ""42""
```

JSON.stringify 在对象中遇到 undefined、function 和 symbol 时会自动将其忽略，在数组中则会返回 null

> toJSON()指的是：“返回一个能够被字符串化的安全的 JSON 值”，而不是“返回一个 JSON 字符串”

<hr>

可以向 JSON.stringify 传递一个 replacer 参数，表示仅转换每个属性；  
比如下方代码表示仅转换对象 a 中的属性 b，所以 c 没有字符串化

```js
var a = {
	b: 42,
	c: "42",
};
JSON.stringify(a, ["b"]); // "{"b":42}"
```

JSON.stringify 有第三个参数 space，表示指定输出的缩进格式
`JSON.stringify(a,null,3)`

<br>

#### boolean

对于所有真值为 true，而所有假值为 false

所有字符串都是真值，除了空字符串是假值外

显式强转为布尔类型最常用的是两个感叹号！！

```js
var a = "";
!!a; // false
```

<br>

#### 强转归纳

对于显式强转，比如下方字符串转数值，以及数值转字符串
这里不适用 new，并不创建封装对象

```js
var num1 = 100;
var num2 = String(num1);

var str1 = "1.11";
var str2 = Number(str1);
```

单个加号运算符当做一元运算符，也是显式强转运算，可以把字符串类型数据转换成数值类型
`var a = +"100"`

> js 中的构造函数没有参数时可以不用带()
> 如 `var time = new Data().geTime();`

<br>

#### 其他知识点

`~x` 等同于 `-(x+1)`
`~42 = -(42+1) = -43`

抽象渗漏：指代码中暴露了底层的实现细节

很多程序员喜欢用~~代替 Math.floor(..)进行向下取整；
而事实上~~仅适用于 32 位数字
`~~-49.6 == -49`

#### 被诟病的==

`==`负责执行隐式强转

当使用&&或者||连接两个包含==的比较式时，二者是先后执行而非同时执行的！
如下会先判断 a==10 之后才判断 a==11
`if(a==10 && a==11){}`

<hr>

千万不要使用==进行假值相等比较，会出现你意想不到的情况
下面例举了四种"假阳"的情况

```js
"0" == false; // true
false == 0; // true
false == ""; // true
"" == 0; // true

42 == "42"; // true
"foo" == ["foo"]; // true
```

<br>

#### 安全使用隐式强转法则

如果两边的值中有 true 或者 false，千万不要使用==
如果两边的值中有[]、""或者 0，尽量不要使用==

因为 typeof 总是返回七种基本类型之一，故在 typeof 管理下的==强转是被允许的
`typeof x == "function"`

一般的，我们为了更安全，抛弃==改而使用===

<br>

#### 抽象关系比较

抽象关系比较即针对诸如`x<y`这种格式的代码中隐式强转规则
若双方中有一个是非字符串，则均转成数字后执行数字大小比较
若双方均为字符串，则按照字母顺序比较

```js
var n1 = [42];
var n2 = ["43"];
a < b; // true

var n3 = ["42"];
var n4 = ["042"];
a < b; // false
```

<br>

### 语法

#### 表达式变动

事实上，ES5 规定变量声明实际上是有返回值的，只不过被算法屏蔽掉了，返回结果为空

目前可以使用两种方式将获得语句的结果值赋值给另一个变量
直接 eval 填入代码，或者塞入 ES7 新增的 do 代码块

```js
var a, b;
a = eval("if(true){b=100;}");

a = do {
	if (true) {
		b = 100;
	}
};
```

标签语句：可以使用 goto 回到该标签的位置继续执行代码

```js
function foo() {
	bar: {
		console.log("hello");
		break bar;
	}
	console.log("word");
}
foo(); // hello world
```

<br>

### 程序性能
