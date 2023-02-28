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
