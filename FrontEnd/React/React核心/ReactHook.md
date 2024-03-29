## Hook 规范

> 此为精简摘抄版本，原版请看官方文档：https://react.docschina.org/docs/hooks-rules.html

<br>

### 仅在顶层使用 Hook

不要在循环，条件或嵌套函数中调用 Hook  
确保总是在你的 React 函数的最顶层以及任何 return 之前调用他们

因为 Hook 的初始化是按照我们注册它们的顺序来的  
即，假设第一次按顺序渲染了两个 Hook，可是在第二次渲染时因为条件判断导致第一个 Hook 无法执行，而 react 就会把第二个 Hook 错误的认定为第一个 Hook，从而发生错误

<br>

### 只在 React 函数中调用 Hook

不要在普通的 JavaScript 函数中调用 Hook

在 react 组件（`React.Component`）内使用 Hook 是无效的

<br>

<br>

## 内置 Hook

### useState

#### 如何使用 state

第一步：引入 useState  
`import React, { useState } from "react";`

第二步：以数组解构的方式使用 useState

- useState 方法会返回两个东西，一个是绑定的属性，一个是修改属性的方法
- useState 接收一个参数，代表属性的初始值
- count 代表属性，setCount 代表设置属性的 setter，这两个名字可以自定义！
  `const [count, setCount] = useState(0);`

<br>

第三步：调用 state  
和 class 中使用 state 不同的是，这里略去了 this 调用，直接可以使用解构出来的属性以及对应 setter
`<p>you click {count} times</p>`  
`<button onClick={() => setCount(count + 1)}>plus 1</button>`

<br>

#### 案例演示

根据上述代码我们不难拼接出下面的这个计数器组件  
首先使用 useState 初始化 count 为 0，并结构出了对应的 setter，其次再在模板中使用 state

```jsx
import React, { useState } from "react";

export default function HookTest() {
  const [count, setCount] = useState(0);
  return (
    <div>
      <p>you click {count} times</p>
      <button onClick={() => setCount(count + 1)}>plus 1</button>
    </div>
  );
}
```

<br>

#### 注意事项

`useState` 在 class 内使用没效果，只能在 function 里面使用

`useState` 定义的初始值类型是任意的，不一定只是数字，还可以是数组、对象等任意东西

<br>

### useEffect

#### useEffect 与生命周期

`useEffect` 是专门用于函数组件的一个方法，他用来检测副作用（常见的副作用比如数据更新，此时我们就需要进行更新页面的操作）

`useEffect` 可以替代 class 组件写法中的生命周期钩子：`componentDidMount、componentDidUpdate、componentWillUnmount`

以下代码为 useEffect 写法  
代码很简单，当数据更新时立即触发 useEffect 函数，执行该函数内的代码

```jsx
import React, { useEffect, useState } from "react";

export default function EffectTest() {
  const [count, setCount] = useState(0);

  useEffect(() => {
    document.title = `you change this ${count}`;
  });

  return (
    <div>
      <p>change this {count}</p>
      <button onClick={() => setCount(count + 10)}>plus 10</button>
    </div>
  );
}
```

而对应的 class 写法则需要对两个生命周期钩子进行数据更新操作，极其麻烦

```jsx
componentDidMount() {
    document.title = `You clicked ${this.state.count} times`;
  }
  componentDidUpdate() {
    document.title = `You clicked ${this.state.count} times`;
  }
```

<br>

#### 需要清除的 effect

鉴于确保订阅增删稳定，我们需要在 useEffect 删除前一个 effect 之前，对其进行清除，然后再用新的 effect 覆盖之

在 useEffect 中的函数内返回一个函数，这个被返回的函数通常作为清除函数使用

```jsx
useEffect(() => {
  // 清除函数
  return function cleanup() {
    ...
  };
});
```

<br>

#### useEffect 特性

默认情况下，第一次渲染过后每次更新都会触发 useEffect 函数

useEffect 触发后会自动生成一个新的 effect 来覆盖旧的 effect，所以每次我们都会发现有微小的差异

React 保证了每次运行 effect 的同时，DOM 都已经更新完毕

<br>

#### 抽离 effect 与更新优化

可以一次性在同一个函数组件内定义多个 useEffect 钩子，react 将按照顺序依次执行这些钩子  
定义多个 useEffect 的好处是可以将不同性质的 effect 分离开来，方便管理

<br>

我们可以额外基于 useEffect 第二个参数，表示当且仅当第二个参数的值发生变化时，才执行 effect 方法，这样可以避免额外的性能损失

```jsx
useEffect(() => {
  document.title = `You clicked ${count} times`;
}, [count]); // 仅在 count 更改时更新
```

<br>

### useReducer

#### 快速上手

useReducer 可视为 useState 的一个扩展 hook，他协助我们进行精细化的数值控制，特别适于处理复杂逻辑

首先定义一个初始值，作为一个 state 传递给 reducer

```js
const initState = { count: 0 };
```

第二步，编写 reducer 函数  
`state` 即为传递过来的数值
`action` 为后续我们用到的 dispatch 传递过来的参数

```js
function reducer(state, action) {
  switch (action.type) {
    case "inc":
      return { count: state.count + 1 };
    case "dec":
      return { count: state.count - 1 };
    default:
      throw new Error();
  }
}
```

第三步，调用 useReducer

```jsx
export default function ReducerTest() {
  const [state, dispatch] = useReducer(reducer, initState);
  return (
    <>
      <div>count: {state.count}</div>
      <button onClick={() => dispatch({ type: "inc" })}>+1</button>
      <button onClick={() => dispatch({ type: "dec" })}>-1</button>
    </>
  );
}
```

整体逻辑：

- `useReducer` 接收两个参数，参数一为处理数值的 reducer，参数二为欲检测的数值
- `useReducer` 返回两个值，一个为状态 `state` ，一个为 `dispatch`（对应 reducer 函数中的 action 参数）

<br>

> 由于文字难以表达对应函数的作用，请上手自行测试多写，大体即可了解该 hook 的用法！

jsx 模板文件完整代码如下：

```jsx
import React, { useReducer } from "react";

const initState = { count: 0 };

function reducer(state, action) {
  switch (action.type) {
    case "inc":
      return { count: state.count + 1 };
    case "dec":
      return { count: state.count - 1 };
    default:
      throw new Error();
  }
}

export default function ReducerTest() {
  const [state, dispatch] = useReducer(reducer, initState);
  return (
    <>
      <div>count: {state.count}</div>
      <button onClick={() => dispatch({ type: "inc" })}>+1</button>
      <button onClick={() => dispatch({ type: "dec" })}>-1</button>
    </>
  );
}
```

<br>

## 自定义 Hook

### 规定

自定义 hook 必须以 `use` 开头

不同 hook 之间的 state 完全隔离

<br>

### 简单测试

自定义 hook 实际上就是我们编写的一个函数，该函数内包含任意内容，可以存在 useState 或者 useEffect 等

创建一个 js 文件 `CustomHook.js` 在里面写下第一个自定义 Hook  
该 hook 的逻辑很简单，检测传入的 id，如果大于 10 那么就返回值为 true 的 demo

```js
import { useState, useEffect } from "react";

function useCustomHook(id) {
  let [demo, setDemo] = useState(false);
  useEffect(() => {
    if (id > 10) setDemo((demo = true));
  });
  return demo;
}

export default {
  useCustomHook,
};
```

<br>

在 jsx 模板内调用该 hook  
简单实现了功能：点击按钮为 count+1，数值为 10 时通过自定义 hook 中的 effect 函数设置 isDemo 为 true，此时动态渲染 div

```jsx
import React, { useEffect, useState } from "react";
import CustomHook from "../hook/CustomHook";

export default function EffectTest() {
  const [count, setCount] = useState(0);

  // 直接调用自定义hook
  const isDemo = CustomHook.useCustomHook(count);

  // 因为自定义hook中存在一个useEffect，可以检测副作用，所以每次count的更新都会触发effect函数
  // 故isDemo都是动态变化的，与此同时带动下方的动态渲染
  return (
    <div>
      <p>点击按钮增加数值 {count}</p>
      <button onClick={() => setCount(count + 1)}>加一</button>
      <div>{isDemo ? "测试为真" : ""}</div>
    </div>
  );
}
```

<br>

## Hooks 原理分析
