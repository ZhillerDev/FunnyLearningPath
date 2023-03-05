### Vue 响应式原理

对比 Vue3 以及 Vue2，这是两个版本下对于响应式处理的不同方法

- `Vue2` 响应式基于 `Object.defineProperty`
- `Vue3` 响应式基于 `Proxy`

`vue2` 组合式写法 data 中的所有属性无论深度几层均会被赋予响应式，包括某些从未用到的属性，造成极大地性能开销  
`vue3` 改用 ref 和 reactive，具惰性，仅被使用到的属性才会赋予响应式

<br>

#### Vue2 实现

vue2 使用基础的 `Object.defineProperty`  
通过遍历对象内部的所有属性，依次为其添加响应式

同时亦导致一大缺点：为对象添加新属性时不再具有响应式，说人话就是响应式仅存在于被 `Object.defineProperty` 注册过的属性（初始化时）！

```js
// 准备一个测试用的对象
const person = {
  name: "tom",
};

// 手写简单reactive方法
// 接收三个参数：对象、key以及对应value
function reactive(obj, key, value) {
  // 为对象obj下的key定义属性，并配置getter、setter方法
  Object.defineProperty(obj, key, {
    get() {
      console.log("访问属性");
      return value;
    },
    set(newVal) {
      console.log("设置新值" + newVal);
      value = newVal;
    },
  });
}

// foreach遍历对象所有属性，使得所有属性都具备响应式能力
Object.keys(person).forEach((key) => {
  reactive(person, key, person[key]);
});

// 对已注册过的（即初始存在的）属性做出取值与修改操作，存在响应式
console.log(person.name);
person.name = "jack";

// 但是定义新的属性值时，不会触发响应式
person.age = 18;
```

<br>

#### Vue3 实现

为解决 vue2 中响应式实现的不足之处，改用 `proxy` 实现  
此时增加任何新属性都会自动将其变成响应式！

```js
const data = {
  name: "jack",
};

function reactive(target) {
  const handler = {
    get(target, key, receiver) {
      console.log(`访问${key}`);
      return Reflect.get(target, key, receiver);
    },
    set(target, key, value, receiver) {
      console.log(`设置新值${value}`);
      Reflect.set(target, key, value, receiver);
    },
  };

  return new Proxy(target, handler);
}

// 绑定响应式
const proxyData = reactive(data);

// 测试响应式数据
console.log(proxyData.name);
proxyData.name = "tom";
console.log(proxyData.name);
```

<br>

#### Proxy 与 Reflect 作用

**对于 `proxy`**

参数一为欲代理的对象，参数二为代理对象设置

`target` 表示原对象  
`key` 表示属性名称  
`value` 表示属性值  
`receiver` 表示被代理后的对象

```js
const person = {
  name: "jack",
};

const proxyPerson = new Proxy(person, {
  get(target, key, receiver) {},
  set(target, key, value, receiver) {},
});
```

<br>

**对于`reflect`**

推荐直接将其结合到 proxy 的 `getter` 和 `setter` 一起使用

下面介绍一下 reflect 两大关键作用

- `Reflect.get(target, key, receiver)` 获取对象 target 中属性名为 key 的值，且 this 指向代理对象 receiver
- `Reflect.set(target, key, value, receiver)` 设置原对象 target 中属性名为 key 的值为 value，并且 this 指向 receiver

这里通过 reflect 将 this 指向当前代理的对象，是为了避免 this 指向紊乱，能保证当前代理的 getter 和 setter 不出错！

```js
const person = {
  name: "jack",
};

const proxyPerson = new Proxy(person, {
  get(target, key, receiver) {
    Reflect.get(target, key, receiver);
  },
  set(target, key, value, receiver) {
    return Reflect.set(target, key, value, receiver);
  },
});
```

<br>

**其余注意事项**

这里必须通过代理对象获取到对应的值或者设置对应的值，但是不能通过使用诸如 `receiver[key]` 这种方式获取值！！！  
这会导致死循环

同时不建议使用 `target[key]` 直接从原对象获取值，因为原对象可能也是一个 proxy，若相互嵌套必定会增大 bug 发生率

<br>
