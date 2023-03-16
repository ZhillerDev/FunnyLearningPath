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

为避免每次于组件内使用 useSelector 时总是要写一长串箭头函数，我们可以直接在对应的子容器内写好，然后调用即可

首先在子容器写好

```js
export const selectAllPosts = (state) => state.posts;
```

然后在组件内部调用

```js
const posts = useSelector(selectAllPosts);
```

<br>
