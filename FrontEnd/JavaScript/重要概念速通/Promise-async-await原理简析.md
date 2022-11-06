---
title: Promise/async/await原理简析
date: 2022-07-15 14:20:07
tags:

categories:
  - 前端面试八股
---

### Promise 基础

<br>

#### promise 基本原理

promise 是专门用来进行异步请求的一个最常用方法；  
一个 Promise 实例对象执行后，会返回一个 promise 状态，为以下三个状态中的一个：

1. 已完成（fulfilled）：操作已完成
2. 已拒绝（rejected）：操作失败
3. 待定中（pending）：等待下一步 then 操作

三种状态具有以下的两个关系，且关系不能反过来
`pending` 可以转变为 `fulfilled`
`pending` 可以转变为 `rejected`

> 一个 promise 已完成或已拒绝，就将其判定为”已决议”状态

<br>

#### promise 三大返回值出现情况介绍

`已完成（fulfilled）` 返回一个值、返回一个空值、返回 fulfilled 状态的 promise
`已拒绝（rejected）` 抛出一个错误、返回 rejected 状态的 promise
`待定中（pending）` 返回 pending 状态的 promise

<br>

#### 基本使用与介绍

使用 new 构造 promise ，需要给他的 constructor 传入一个函数（通常是箭头函数）；  
函数包含两个可选固定参数，一个是 resolve，另一个是 reject，且他们都是一个方法而非变量；  
两个形参先后顺序不可颠倒，但是形参名可以随便取；

代码解释：

1. 使用 resolve()方法设置此 promise 对象为 `fulfilled` 状态
2. 或者使用 reject()方法设置此 promise 对象为 `rejected` 状态
3. resolve 和 reject 方法中都可接受一个实参，它将作为参数传递给下一个调用的 promise
4. 使用 then 接收上一个 promise 传来的数据，它可接受两个参数（目前我们只讲第一个），第一个是当 `fulfilled` 时接收到由 resolve 传来的参数，我们使用箭头函数来对其进行处理

```js
const p = new Promise((resolve, reject) => {
  setTimeout(() => {
    resolve(100);
  }, 1000);
});

p.then((val) => {
  console.log(val);
});

// 延迟1s后输出100
```

<br>

#### promise 执行顺序

重中之重！  
当调用 promise 实例时，需要经过以下两个顺序：

1. 被调用的 promise 内部函数全部执行完毕，promise 传参给下一对象
2. 调用 promise 对象的函数开始处理 promise
3. then 存在时，它将作为一个微任务，在所有宏任务执行完毕后才执行（所以通常是最后执行）

> 故以下代码可以这么理解：
> 首先执行 promise 内部函数，输出 log；随后调用 promise 的函数获得一个 promise 状态 pending 并输出！

```js
const promise1 = new Promise((resolve, reject) => {
  console.log("promise1");
});
console.log("1", promise1);

// 返回
// 'promise1'
// '1' Promise{<pending>}
```

<br>

### Promise 进阶

<br>

#### 快速设置状态

使用以下两个函数，快速的设置 promise 的状态为 fulfilled 或者 rejected

```js
// 设置fulfilled
Promise.resolve(200);

// 设置rejected
Promise.reject(404);
```

<br>

#### then 高级使用

then 接收两个参数，第一个为 fulfilled 状态的接收函数，第二个为 rejected 状态的接收函数；  
如下代码，我们对一个状态为 rejected 的 promise 进行 then 操作，那么他将会调用第二个函数；  
所以最终输出失败信息！

```js
Promise.resolve(10)
  .then(() => {
    // 抛出异常，使得状态变成rejected
    throw new Error("oops!!!");
  })
  .then(
    (success) => {
      console.log("成功了！" + success);
    },
    (error) => {
      // rejected状态下将会调用这个方法
      console.log("失败了！" + error);
    }
  );
```

事实上我们会使用 `catch` 代替 then 中的双方法写法（catch 专门捕获 rejected 状态传来的参数）

```js
Promise.resolve()
  .then(() => {
    throw new Error("新的错误产生了！");
  })
  .catch((error) => {
    console.log("这个错误是：" + error);
  });
```

<br>

#### Promise.all

定义：接收一组异步任务，然后并行执行异步任务，并且在所有异步操作执行完后才执行回调

同时按顺序执行多个 promise，promise 以数组的形式传入；  
可以传入非 promise 变量，最终结果原样输出；

> 最终结果以一个数组的形式输出

```js
const promise1 = Promise.resolve(3);
const promise2 = 42;
const promise3 = new Promise((resolve, reject) => {
  setTimeout(resolve, 100, "foo");
});

// 按顺序执行，最终输出一个数组
Promise.all([promise1, promise2, promise3]).then((values) => {
  console.log(values);
});
```

当所有 promise 中有一个失败，那么 all 立即返回失败；

```js
const pro1 = new Promise.resolve(100);
const pro2 = new Promise.resolve(200);
const pro3 = new Promise.reject("reject");

Promise.all([pro1, pro2, pro3]).then(
  (values) => {
    console.log(values);
  },
  (reason) => {
    console.log(reason); // 处理的promise中有一个失败，全部失败
  }
);
```

<br>

#### Promise.any

这是一个实验性方法，未获得绝大多数浏览器支持；  
它的作用和 all 完全相反，只要有一个 fulfilled 那就是成功，必须全都 rejected 才算失败；  
例子就不给了，any 用法和 all 一致，也是筛选所有 promise 后得出结果

<br>

#### Promise.race

使用格式和 all 一致，但是他的定义是：  
接收一组异步任务，然后并行执行异步任务，只保留取第一个执行完成的异步操作的结果，其他的方法仍在执行，不过执行结果会被抛弃。

<br>

### Promise 面试题

<br>

#### setTimeout 相关

请判断下方代码的输出结果？

```js
setTimeout(() => {
  console.log("timer1"); // a
  setTimeout(() => {
    console.log("timer3"); // c
  }, 0);
}, 0);
setTimeout(() => {
  console.log("timer2"); // b
}, 0);
console.log("start");
```

首先记住：每一个 `setTimeout` 都是一个宏任务，他会延后执行；  
目前的宏任务是最外层的函数，故最先输出最后一行的 log 结果： start  
按照宏任务队列原则，排在前面的宏任务先加入队列并先执行；  
故标记 a 处的宏任务先执行，输出 timer1  
接着标记 b 处的宏任务执行，输出 timer2  
最后执行标记 c 处宏任务，输出 timer3

> 因为 js 不存在多线程，所以没有嵌套层内部和外层同时执行这一说法，无论嵌套多深都必须按照每次执行一个宏任务的规定顺序来

```js
// 最终输出结果：
"start";
"timer1";
"timer2";
"timer3";
```

<br>

#### 宏任务微任务套路

当 setTimeout 和 then 混合在一起写的话就要注意宏任务微任务结合的问题；  
务必按照 `队列 + 执行顺序` 两步走  
即宏任务和微任务执行必须按照队列中顺序；执行顺序为 宏任务->微任务->宏任务->...

```js
// 下面使用数字表明了执行顺序
Promise.resolve().then(() => {
  console.log("promise1"); // 2
  const timer2 = setTimeout(() => {
    console.log("timer2"); // 5
  }, 0);
});
const timer1 = setTimeout(() => {
  console.log("timer1"); // 3
  Promise.resolve().then(() => {
    console.log("promise2"); // 4
  });
}, 0);
console.log("start"); // 1
```

<br>

#### then、catch、finally

一些讨厌的规定：

1. `Promise` 的状态一经改变就不能再改变
2. `.then` 和 `.catch` 都会返回一个新的 Promise
3. 必须使用 `throw` 抛出的错误才可以被 `catch` ，而 `return` 的错误无法被捕获
4. `.finally` 方法也是返回一个 Promise，作用类似 try...catch... 中的 finally
5. 无论 ` ` 了多少层，只要上层存在哪怕一个 `rejected` 都可以被 `catch` 到

<br>

如下，因 catch 上层无 rejected，执行时直接跳过 catch；  
最后一个 then 捕获到第一个 then 的返回值 2 并输出

```js
Promise.resolve(1)
  .then((res) => {
    console.log(res);
    return 2; // 相当于 resolve(2)
  })
  .catch((err) => {
    return 3;
  })
  .then((res) => {
    console.log(res);
  });
```

<br>

当 then 和 catch 里面接收的不是一个函数时，就会发生 `透传效应` ，即第一个非函数值会一直向下递送直到遇见一个函数或者到达末尾；

```js
Promise.resolve(1).then(2).then(Promise.resolve(3)).then(console.log);
// 输出 1
```

<br>

### Async/Await

<br>

**普通用法及其解释**

> 几个书写注意事项：  
> `async` 写在 function 的前面，`await` 写在对应方法或者变量的前面

1. await：被他修饰的一行代码在执行完后等待一秒再执行接着执行后面的代码
2. 所以以下代码的结果是：首先立即执行 res1 初始化，然后停等 1s，接着初始化 res2，继续停等 1s 然后才 log 输出，此时函数结束，用时共两秒

```ts
// 这里的request就是我们上面promise第一个例子封装的那个
async function fn() {
  const res1 = await request(1);
  const res2 = await request(res1);
  console.log(res2);
}
fn();
```

<br>

**返回值细节与注意事项**  
请牢记以下规则：

1. await 只能在被 async 修饰过的函数里使用
2. 被 async 修饰后的函数叫做 async 函数
3. async 函数若带有返回值，则成功执行完毕后返回一状态为`fulfilled`的 Promise
4. asynch 函数若没有 return，那么执行完毕返回一状态为 `undefined` 的 Promise
5. async/await 的作用简单来讲就是：用同步的方式来执行异步操作
6. await 后最好接着 Promise 函数，当然接其它的值依然有停等效果

<br>

### generator 函数

> generator 函数：  
> 形如`function*`的函数就叫做 generator 函数  
> 仅在 generator 函数中才可以用`yield`，他表示暂停点  
> 跳过当前暂停点到下一个暂停点需要使用`next()`方法  
> next 方法执行后会返回一个对象，对象中有 value 和 done 两个属性

value：暂停点后面接的值，也就是 yield 后面接的值  
done：是否 generator 函数已走完，没走完为 false，走完为 true

当到最后一个 yield 后还继续 next，那么就会返回一个 value 为 undefined，done 为 true 的对象！

```js
function* gen() {
  yield 1;
  yield 2;
}
const g = gen();
console.log(g.next()); // { value: 1, done: false }
console.log(g.next()); // { value: 2, done: false }
console.log(g.next()); // { value: undefined, done: true }
```

<br>
