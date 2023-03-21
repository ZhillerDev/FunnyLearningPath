### Map 渲染问题

目前我们设置一个变量 render，用于存储 map 列表渲染的结果

map 对象 posts 是一个数组，数组里面有任意多个对象

```jsx
let render;
render = posts.map((itm, idx) => (
	<PostCard
		item={itm}
		key={idx}
	/>
));

return render;
```

这是咱们的子组件代码

```jsx
function PostCard(item) {
	return (
		<>
			<h2>{item.title}</h2>
			<div>id: {item.id}</div>
			<div>body: {item.body}</div>
		</>
	);
}
```

可是我们会发现无论如何都无法渲染

<br>

此时我们仅需要为子组件的形参加上一个花括号即可，就像下面的那样

```jsx
function PostCard({ item }) {
	...
}
```

<br>
