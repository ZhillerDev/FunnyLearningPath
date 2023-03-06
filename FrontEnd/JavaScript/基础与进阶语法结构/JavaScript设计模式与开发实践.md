### 面向对象的 JS

#### 多态

相对于 Java 的静态类型，JS 的弱类型模式能让我们高效的使用多态

```js
// 抽象出一个总方法，类似于基类
var renderMap = (map) => {
  if (map.show instanceof Function) {
    map.show();
  }
};

// 定义两个子类，分别实现show方法
let basicMap = {
  show: () => {
    console.log("初始化地图");
  },
};
let anotherMap = {
  show: () => {
    console.log("另一张地图");
  },
};

// 通过基类实现多态调用
renderMap(basicMap);
renderMap(anotherMap);
```

<br>

#### 封装

> 书中原话记录：通过封装变化的方式，把系统中稳定不变的部分和容易变化的部分隔离开来，在系统的演变过程中，我们只需要替换那些容易变化的部分，如果这些部分是已经封装好的，替换起来也相对容易。这可以最大程度地保证程序的稳定性和可扩展性。

我们可以借助 ES6 中国的 symbol 实现类似 private 修饰符的作用，定义一个私有属性或者方法

`Symbol.for` 可以将指定属性添加到全局私有属性库（非官方定义概念名，大家理解就好）里面  
`Symbol.keyFor` 从全局私有属性库中寻找指定 symbol

```js
// Symbol.for()可以定义一个全局私有属性
// 每次定义都会从全局symbol寻找是否有重复属性，如果存在就直接引用而不新建（比如下方sb2，就直接引用了sb1）
let sb1 = Symbol.for("uuid");
let sb2 = Symbol.for("uuid");

// 不用for注册的symbol就只是一个局部变量，不会添加到全局私有属性库里面
let sb3 = Symbol("uuid");

// 由于引用一致，故true
console.log(sb1 === sb2); // true
// 由于没有添加到全局私有属性库，故无法通过keyFor获取到值
console.log(Symbol.keyFor(sb3)); // undefined
```

<br>

#### 原型模式与对象系统

最常见的原型继承方式如下：

这是 `a.name` 执行时的完整过程

1. 现在对象 a 中查找，发现没有 name 属性
2. 再在 a 的构造器中找，即 `a.__proto__`，发现其连接着 `A.prototype`
3. 而 `A.prototype` 又指向了对象 obj
4. 此时直接从对象 obj 中获取 name 属性

```js
var obj = { name: "sven" };

var A = function () {};
A.prototype = obj;

var a = new A();
console.log(a.name); // 输出：sven
```

<br>

类之间的继承可以以以下的方式模拟

```js
// 指定A的原型为一个对象，对象中包含属性name
var A = function () {};
A.prototype = { name: "sven" };

// 将B的原型指定为新实例A
var B = function () {};
B.prototype = new A();

// 构造实例B，然后取出name
var b = new B();
console.log(b.name); // 输出：sven
```

<br>

### this\call\apply

#### this

一般的，为了简化根据 ID 获取 DOM 的代码，我们可以这样写

```js
// 方式一
var getId = function (id) {
  return document.getElementById(id);
};

getId("div1");
```

而不可以这样写

```js
// 方式二
var getId = document.getElementById;
getId("div1");
```

某些浏览器在执行 `document.getElementById` 时会用到 this，而方式二中属于普通函数调用，此时的 this 指向 `window` 而不是 `document` ，故报错！

可以通过 `apply` 或者 `call` 修改 `this` 指向，从而使得方式二也可以被使用

<br>

#### call\apply

call 和 apply 第一个参数可以为 null，此时的 this 指向即保持默认

特殊情况下，第一个参数传入 null 是为了代替某些具体对象，比如下面的取最大值的方法

```js
let res = Math.max.apply(null, [1, 2, 3, 4, 5]);
console.log(res); // 5
```

<br>

call 修正 this 指向的小案例：

```js
// 修改this使其指向当前DOM，而不是window
document.getElementById("div1").onclick = function () {
  var func = function () {
    alert(this.id); // 输出：div1
  };
  func.call(this);
};
```

<br>

对于某些不支持 `Function.prototype.bind` 的浏览器来说，我们可以简单手写模拟一个

```js
// 模拟Function.prototype.bind
Function.prototype.bind = function (context) {
  var self = this; // 保存原函数
  return function () {
    // 返回一个新的函数
    return self.apply(context, arguments); // 执行新的函数的时候，会把之前传入的context
    // 当作新函数体内的this
  };
};

var obj = {
  name: "sven",
};
var func = function () {
  alert(this.name); // 输出：sven
}.bind(obj);

func();
```

<br>

借用构造函数

借用 `Array.prototype.push` 往 `arguments` 添加一个新的元素

```js
(function () {
  Array.prototype.push.call(arguments, 3);
  console.log(arguments); // 输出[1,2,3]
})(1, 2);
```

<br>

### 闭包与高阶函数

Array.prototype.sort 接受一个函数当作参数，用户可以自行在该函数内指定排序方式

```js
// 由小到大排序
let res = [1, 4, 2].sort((a, b) => {
  return a - b;
});
console.log(res);
```

<br>

#### Function 扩展函数

通过对 Function 原型执行扩展，可以达到类似装饰器的效果，这也是 AOP 风格的体现

```js
Function.prototype.before = function (beforefn) {
  var __self = this; // 保存原函数的引用
  return function () {
    // 返回包含了原函数和新函数的"代理"函数
    beforefn.apply(this, arguments); // 执行新函数，修正this
    return __self.apply(this, arguments); // 执行原函数
  };
};

Function.prototype.after = function (afterfn) {
  var __self = this;
  return function () {
    var ret = __self.apply(this, arguments);
    afterfn.apply(this, arguments);
    return ret;
  };
};

var func = function () {
  console.log(2);
};

func = func
  .before(function () {
    console.log(1);
  })
  .after(function () {
    console.log(3);
  });

func(); // 1,2,3
```

<br>

#### 柯里化函数

currying 又称部分求值。一个 currying 的函数首先会接受一些参数，接受了这些参数之后，该函数并不会立即求值，而是继续返回另外一个函数，刚才传入的参数在函数形成的闭包中被保存起来。待到函数被真正需要求值的时候，之前传入的所有参数都会被一次性用于求值

```js
var currying = function (fn) {
  var args = [];

  // 当调用柯里函数时不带任何参数，直接执行所有栈中函数
  // 当调用柯里函数时带入了参数，就会向args压入一个新的带参函数
  return function () {
    if (arguments.length === 0) {
      return fn.apply(this, args);
    } else {
      [].push.apply(args, arguments);
      return arguments.callee;
    }
  };
};

// 将cost写成一个IIFE函数，之后其将转变为柯里函数
var cost = (function () {
  var money = 0;
  return function () {
    for (var i = 0, l = arguments.length; i < l; i++) {
      money += arguments[i];
    }
    return money;
  };
})();

var cost = currying(cost); // 转化成currying函数

cost(100); // 未真正求值
cost(200); // 未真正求值
cost(300); // 未真正求值
alert(cost()); // 求值并输出：600
```

<br>

### 单例模式

#### 透明的单例模式

传统单例模式，需要你知道当前对象是单例的，且可以调用 getInstance 方法获取其实例

可以使用代理的方式管理单例，而对象本身不处理代理相关逻辑

```js
var CreateDiv = function (html) {
  this.html = html;
  this.init();
};

CreateDiv.prototype.init = function () {
  var div = document.createElement("div");
  div.innerHTML = this.html;
  document.body.appendChild(div);
};

// 使用代理创建以及识别单例
var ProxySingletonCreateDiv = (function () {
  var instance;
  return function (html) {
    if (!instance) {
      instance = new CreateDiv(html);
    }

    return instance;
  };
})();

var a = new ProxySingletonCreateDiv("sven1");
var b = new ProxySingletonCreateDiv("sven2");

alert(a === b); // true
```

<br>

#### 惰性单例

将获取单例的方法单独抽离出来，通过此方法获取对象的单例

```js
var getSingle = function (fn) {
  var result;
  return function () {
    return result || (result = fn.apply(this, arguments));
  };
};
```

<br>

### 策略模式

> 策略模式的目的就是将算法的使用与算法的实现分离开来

#### 策略模式发展

假设有这么一个场景：需要通过员工名字及其薪水计算奖金

经典方法需要每次接收对应的两个参数，方法内需要大量的 ifelse 做逻辑覆盖，并且缺乏弹性违背开闭原则；

<br>

#### 策略模式实现

```js
var strategies = {
  S: function (salary) {
    return salary * 4;
  },
  A: function (salary) {
    return salary * 3;
  },
  B: function (salary) {
    return salary * 2;
  },
};

var calculateBonus = function (level, salary) {
  return strategies[level](salary);
};

console.log(calculateBonus("S", 20000)); // 输出：80000
console.log(calculateBonus("A", 10000)); // 输出：30000
```
