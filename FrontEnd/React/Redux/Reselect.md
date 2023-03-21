### 快速入门

下载对应依赖：`npm install reselect`

创建一个新文件 `ReselectTest.js`

由于逻辑比较复杂，请直接看下方注释

```js
// 导包
import { createSelector } from "reselect";

// 第一步
// 定义一个数据源
const data = {
	count: 0,
};

// 第二步
// 选择我们需要用到的变量，这里的state表示数据源，必须写成函数的形式
const count = (state) => state.count;

// 第三步
// 使用createSelector对指定数据进行处理
// 参数一：欲监听的变量或属性（写成数组的形式）
// 参数二：对变量或属性做出的回调操作（这里写成箭头函数，且函数形参与参数一数组内变量数量一一对应）
const plus = createSelector([count], (count) => {
	count++;
	console.log(count);
});

// 第四步
// 调用select
plus(data);
```

<br>

使用 node 快速运行一下，发现输出 1

完整过程：plus 方法传入数据源 data，根据定义的 count 获取到数据源内部的属性 count；直接对 count 属性进行自增以及 log 输出的操作

<br>

### 整合 Redux

#### 创建 slice

```js
import { createSlice, createSelector } from "@reduxjs/toolkit";

const initialState = {
	count: 0,
};

const reselectSlice = createSlice({
	name: "reselector",
	initialState,
	reducers: {
		changeCount(state) {
			state.count += 1;
		},
	},
});

// 在这里定义好我们要监听的属性模板以及事业createSelector创建select
const countState = (state) => state.count;
export const countListener = createSelector([countState], (countState) => {
	console.log(countState);
});

export const { changeCount } = reselectSlice.actions;

export default reselectSlice.reducer;
```

<br>

#### 函数组件调用 slice

```jsx
import React, { useEffect } from "react";

import { useSelector, useDispatch } from "react-redux";
import { changeCount, countListener } from "./reselectSlice";

const ReselectLayout = () => {
	const state = useSelector((state) => state.reselector);
	const dispatch = useDispatch();

	// 副作用中每次都调用
	// 传入的state为父组件透传下来的数据源（烦死了解释不清楚。。。）
	useEffect(() => {
		countListener(state);
	});

	return (
		<>
			<div>{state.count}</div>
			<button
				onClick={() => {
					dispatch(changeCount());
				}}
			>
				修改数据
			</button>
		</>
	);
};

export default ReselectLayout;
```

<br>
