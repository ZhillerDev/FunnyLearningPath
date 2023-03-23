### 子容器 slice

#### 设置特定的 reducer

将 `reducer` 写成对象的形式，并配置 reducer 和 prepare

`prepare` 可以设置接收的形参，可以便于我们调用

```jsx
const postsSlice = createSlice({
	...
	reducers: {
		addPost: {
            // 用来执行reducer的功能
			reducer(state, action) {
				state.push(action.payload);
			},
            // 用来设置接收的形参
			prepare(title, content) {
				return {
					payload: {
						id: nanoid(),
						title,
						content,
					},
				};
			},
		},
	},
});
```

此时，我们调用 reducer 就可以这样写了

```jsx
dispatch(addPost(title, content));
```

<br>

#### 抽离 selector

为避免每次于组件内使用 `useSelector` 时总是要写一长串箭头函数，我们可以直接在对应的子容器内写好，然后调用即可

首先在子容器写好

```js
export const selectAllPosts = (state) => state.posts;
```

然后在组件内部调用

```js
const posts = useSelector(selectAllPosts);
```

<br>

### createAsyncThunk

#### 参数与使用介绍

`createAsyncThunk` 用来定义异步请求

参数一为类型前缀，推荐格式为：`slice名称/当前变量名`  
参数二为异步请求负载，可以使用 fetch 或者 axios  
参数三为回调函数（可选非必填）

下面用到了一个虚假 JSON 生成接口网站，可以根据我们请求的 URL 自动反馈假的 JSON 数据

```js
import { createSlice, createAsyncThunk, nanoid } from "@reduxjs/toolkit";
import axios from "axios";

// 假JSON数据生成网站地址
const POST_URL = "https://jsonplaceholder.typicode.com/posts";

// createAsyncThunk需要单独配置为一个常量
export const fetchPosts = createAsyncThunk("axio/fetchPosts", async () => {
	// 使用axios的GET请求获取后端JSON，并对其进行异常捕获
	try {
		const res = await axios.get(POST_URL);
		return res.data[0];
	} catch (err) {
		return err.message;
	}
});

...
```

<br>

除此之外，我们还需要在 slice 内部使用 `extraReducers` 来实时检测 `createAsyncThunk` 的状态

`extraReducers` 使用 `addCase` 添加监测情况，每次使用异步请求后 extraReducers 都会检测其返回的 proxy 状态，然后执行对应的动作

通过检测不同请求状态，从 `action` 形参中取出返回数据并对 `initialState` 对应值进行修改

```js
const axiosSlice = createSlice({
	...

	// extraReducers自带一个builder构建器参数
	extraReducers(builder) {
		builder
			// addCase添加监测条件（或者叫状态）
			// 参数一：state表示当前slice的initialState
			// 参数二：action存储着createAsyncThunk请求完毕后返回的数据
			.addCase(fetchPosts.pending, (state, action) => {
				state.status = "loading";
			})
			.addCase(fetchPosts.fulfilled, (state, action) => {
				state.status = "succeeded";
				state.posts.push(action.payload);
				console.log(state.posts[0]);
			})
			.addCase(fetchPosts.rejected, (state, action) => {
				state.status = "rejected";
			});
	},
});
```

<br>

调用方式也很简单，直接在函数组件内部使用 `useDispatch` 即可调用该方法，实现异步请求

```jsx
const dispatch = useDispatch();

dispatch(fetchPosts());
```

<br>

### createEntityAdapter
