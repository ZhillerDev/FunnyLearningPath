### 入门案例

> 参考视频 https://www.youtube.com/watch?v=NqzdVN2tyvQ

<br>

#### 环境配置

> 由于国外下载源过慢，这里改用 vite 搭设 react 环境

首先使用 `vite` 创建对应的 `react` 环境，选择 `javascript` 而不是 typescript  
`npm create vite@3`

进入对应项目文件夹后执行初始化 `npm i`

此时安装两个关键库  
`npm i react-redux @reduxjs/toolkit`

<br>

#### 项目驱动

> 此入门案例旨在让读者们快速了解容器创建的完整步骤，并了解其模板格式，便于日后套用与扩展

目标：实现一个简单的计数器功能

redux 的基本使用步骤：

1. 编写 store，作为全部容器的总管理器
2. 编写 slice，可将其视为单个容器，各个 slice 互相隔离
3. 编写组件，在组件内可以直接调用容器进行状态处理
4. 调用并渲染组件

<br>

#### 编写 slice

> 由于 slice 是最底层的容器，所以从这里开始写，而不是一开始就写 store

为了便于先前接触过 `vue` 框架下的 `pinia` 容器管理的读者快速理解，注释中对 `redux` 的功能与 `pinia` 做出了类比（可能部分概念并不完全对应，但能大体理解即可！）

以下展示了单个 slice 的基础模板，详细功能请直接看代码解析

代码清单 `features/counter/counterSlice`

```js
// 从redux toolkit库导入createSlice
import { createSlice } from "@reduxjs/toolkit";

// 容器中的值在此执行初始化（相当于state）
const initialState = {
	count: 0,
};

// 创建容器（相当于defineStore）
export const counterSlice = createSlice({
	// store的名字
	name: "counter",
	// store的值（相当于state）
	initialState,
	// store的方法（相当于actions+getters结合体）
	reducers: {
		increment: (state) => {
			state.count += 1;
		},
		decrement: (state) => {
			state.count -= 1;
		},
		reset: (state) => {
			state.count = 0;
		},
		// 这里的方法比较特殊，他接受除state外的一个action，后面会有提到
		incrementByAmount: (state, action) => {
			state.count += action.payload;
		},
	},
});

// 所有的actions都必须以这种方式执行导出！
export const { increment, decrement, reset, incrementByAmount } =
	counterSlice.actions;

// 固定格式，导出该容器的reducer
export default counterSlice.reducer;
```

<br>

#### 定义 store

`store` 作为总容器，他要做的事情很少，以至于仅需要用到一个属性 reducer

`configureStore` 定义总容器  
`reducer` 注册子容器

reducer 中注册子容器时，键值对的格式如下表示：  
`容器名:导入的容器别名`

代码清单 `app/store.js`

```js
import { configureStore } from "@reduxjs/toolkit";

// 这里导入的容器别名需要我们自己定义！
import counterSlice from "../features/counter/counterSlice";

export const store = configureStore({
	reducer: {
		// 注册子容器的格式如下
		// 容器原名:容器别名
		posts: counterSlice,
	},
});
```

<br>

#### 编写组件

很好，设置完总容器并妥善注册子容器后，接下来就是比较复杂的使用部分了

```jsx
import React, { useState } from "react";
import { useSelector, useDispatch } from "react-redux";
import { increment, decrement, reset, incrementByAmount } from "./counterSlice";

const Counter = () => {
	const count = useSelector((state) => state.counter.count);
	const dispatch = useDispatch();

	const [increAmount, setIncreAmount] = useState(0);
	const addValue = Number(increAmount) || 0;
	const resetAll = () => {
		setIncreAmount(0);
		dispatch(reset());
	};

	return (
		<section>
			<p>{count}</p>
			<div>
				<button onClick={() => dispatch(increment())}>+</button>
				<button onClick={() => dispatch(decrement())}>-</button>
			</div>

			<input
				type="text"
				value={increAmount}
				onChange={(e) => setIncreAmount(e.target.value)}
			/>
			<button onClick={() => dispatch(incrementByAmount(addValue))}>
				add amount
			</button>
			<button onClick={resetAll}>reset value</button>
		</section>
	);
};

export default Counter;
```

<br>

#### 收尾

这一部分就没什么事可做了，只需要在 app.jsx 里面调用刚刚写好的组件即可

```jsx
import React, { Suspense } from "react";
import Counter from "../features/counter/Counter";

export default function App() {
	return (
		<main className="App">
			<Counter />
		</main>
	);
}
```

之后来带 main.jsx，这里还有一些点需要大家注意

```jsx
import React from "react";
import ReactDOM from "react-dom/client";
import "./index.css";
import App from "./view/app";

// 导入总容器store
import { store } from "./app/store";
// 导入provider实现容器透传
import { Provider } from "react-redux";

ReactDOM.createRoot(document.getElementById("root")).render(
	<React.StrictMode>
		<Provider store={store}>
			<App />
		</Provider>
	</React.StrictMode>
);
```
