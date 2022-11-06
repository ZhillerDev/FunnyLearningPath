---
title: JS原型链通俗讲义
date: 2022-07-20 06:59:45
tags:

categories:
  - 前端面试八股
---

### 函数类型简介

> 函数主要有普通函数和构造函数，任何一个函数都可以成为构造函数！

1. 构造函数：可以被 new 的函数叫做构造函数
2. 普通函数：不使用 new 进行函数实例化的函数
3. 实例：只要一个函数被 new 了，那他立即成为一个实例，如 `const fn = new Func();`

<br>

### 原型对象关系理解

<br>

**什么是原型？**
说人话就是：对于任意一个实例对象 obj，他同时拥有两个存储区，一个是原型存储区，另一个是本函数自带的存储区；当我们创建一个空函数时，可以直接从函数的原型对象中取出属性；而函数非空时，就可以从函数自带存储区拿出属性

<br>

**原型与原型对象**  
原型存在的意义就是组成原型链；  
原型链存在的意义就是继承；  
继承存在的意义就是属性共享；

<br>

**原型链与原型链终点**  
原型链 即 `__proto__` 的路径
原型链终点 即 `Object.prototype.__proto__ === null`

<br>

**proto 与 prototype**  
假设目前有构造函数 Person 和他的对应实例 person

1. 构造函数的 `prototype` 指向函数的原型对象： `Person.prototype`
2. 实例的 `__proto__` 指向函数的原型对象： `person.__proto__`
3. 故构造函数和实例指向原型对象的关系是： `Person.prototype === person.__proto__`

4. 原型对象的原型会向上溯源： `Person.prototype.proto === Object.prototype`
5. Object 原型对象的原型为 null： `Object.prototype.__proto__ === null`

> 可以一直对某一函数连续取原型，最终会取出 null

<br>

**constructor**  
任何一个原型对象都有这属性，他指向原来的构造函数，以下两个等式结果都是一致的：  
`Person.prototype.constructor`  
`person.__proto__.constructor`

<br>

**Function**  
函数都是由 Function 原生构造函数创建的，所以函数的`__proto__`属性指向 Function 的 prototype 属性  
存在关系：`fn.__proto__ === Function.prototype // true` （fn 为某构造函数）  
存在特例：Function 的`__proto__`属性指向 Function.prototype

```js
function fn1() {
  console.log("function");
}

console.log(Function.prototype === fn1.__proto__); // true
```

<br>

### 对象构造相关

<br>

**创建对象（实例）的方法**

```js
// 构造函数创建实例
function fn1(name) {
  this.name = name;
}
const fnn1 = new fn1("helloworld");

// 字面量创建对象
const fnn2 = { name: "nameless" };

// new Object创建对象
const fnn3 = new Object();
fnn3.name = "goodbye";

// Object.create创建对象
const fnn4 = Object.create({});
fnn4.name = "helloworld";
```

> 如上方代码，fnn3 和 fnn4 的构造函数都是 Object

<br>

**Function 和 Object**

定理：  
`函数` 是 Function 构造函数的实例  
`对象` 是 Object 构造函数的实例

<br>

**new 操作符的作用过程**  
首先创建新对象，并将其 `__proto__` 属性指向构造函数的 prototype 属性  
然后将构造函数调用的 this 指向该新对象，指向构造函数；
