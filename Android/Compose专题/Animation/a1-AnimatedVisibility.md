### 浮动按钮显隐案例

> 插入 FAB(浮动按钮)到 scaffold 布局内，我们需要当列表向下滑动时 FAB 自动收起，向上滑动时展开

完整实现流程：

1. scaffold 定义状态，检测 lazycolumn 滚动方向
2. 根据滚动方向来动态指定 extend 变量是否为 true
3. 将 extend 变量传递给 FAB 函数，动态显隐文本
4. 结束

<br>

#### isScrollingUp

再开始工作之前，需要为 LazyListState 自己编写一个扩展方法 isScrollingUp，用来检测当前滚动方向

```kotlin
@Composable
private fun LazyListState.isScrollingUp(): Boolean {
    var previousIndex by remember(this) { mutableStateOf(firstVisibleItemIndex) }
    var previousScrollOffset by remember(this) { mutableStateOf(firstVisibleItemScrollOffset) }
    return remember(this) {
        derivedStateOf {
            if (previousIndex != firstVisibleItemIndex) {
                previousIndex > firstVisibleItemIndex
            } else {
                previousScrollOffset >= firstVisibleItemScrollOffset
            }.also {
                previousIndex = firstVisibleItemIndex
                previousScrollOffset = firstVisibleItemScrollOffset
            }
        }
    }.value
}
```

<br>

#### FAB

定义一个 FAB 组件，使用 `FloatingActionButton` 可以便于自定义

`AnimatedVisibility` 函数可以使得组件显隐过渡平滑，且能根据 API 自定义显隐过程的持续时间以及过程  
最简单的使用方法是将其包裹你想要动态显隐的组件，并使用 `visible` 属性控制显隐

```kotlin
@Composable
private fun HomeFloatingActionButton(
    // 是否显隐，由上级scaffold定义
    extended: Boolean,
    onClick: () -> Unit
) {
    // 一个标准的带图标与文本的FAB
    FloatingActionButton(onClick = onClick) {
        Row(
            modifier = Modifier.padding(horizontal = 16.dp)
        ) {
            Icon(
                imageVector = Icons.Default.Edit,
                contentDescription = null
            )

            // 根据extend的值判断是否显示隐藏
            AnimatedVisibility(visible = extended) {
                Text(
                    text = stringResource(R.string.edit),
                    modifier = Modifier
                        .padding(start = 8.dp, top = 3.dp)
                )
            }
        }
    }
}
```

<br>

#### scaffold

按步骤走

```kotlin
@Composable
fun Home() {

    // 第一步：定义lazycolumn状态
    val lazyListState = rememberLazyListState()

    Scaffold(
        ...
        floatingActionButton = {
            HomeFloatingActionButton(
                // 第三步，判断当前滚动方向，将布尔返回值作为形参传递给HomeFloatingActionButton
                extended = lazyListState.isScrollingUp(),
            )
        }
    ) { padding ->
        LazyColumn(
            // 第二步：基于lazycolumn指定状态
            state = lazyListState,
        ) {
            ...
        }
    }
}
```

<br>

### 顶部气泡下弹

> 需求：点击按钮后于 app 顶部下弹一个全宽度小卡片，过一段时间自己收回去（类似于顶部弹出气泡通知）

同理，为提供流畅的显示隐藏动画，需要使用 `AnimatedVisibility` 包裹组件，且这里用到了自定义动画

enter 定义入场动画及起始点；  
exit 定义出场动画及动画截止点；

tween 可以设置动画的持续时间与 ease

```kotlin
@Composable
private fun EditMessage(shown: Boolean) {
    AnimatedVisibility(
        visible = shown,

        // 入场动画
        enter = slideInVertically(
            // 初始Y轴位置定义为负的总高度，此时卡片完全隐藏在顶部
            // 则入场动画为 -fullHeight -> 0
            initialOffsetY = { fullHeight -> -fullHeight },
            // 动画扩展设置
            animationSpec = tween(durationMillis = 150, easing = LinearOutSlowInEasing)
        ),

        // 出场动画
        exit = slideOutVertically(
            // 同理，出场动画设置终止位置
            // 故动画为 0 -> -fullHeight
            targetOffsetY = { fullHeight -> -fullHeight },
            animationSpec = tween(durationMillis = 250, easing = FastOutLinearInEasing)
        )
    ) {
        Surface(
            modifier = Modifier.fillMaxWidth(),
            color = MaterialTheme.colors.secondary,
            elevation = 4.dp
        ) {
            Text(
                text = stringResource(R.string.edit_message),
                modifier = Modifier.padding(16.dp)
            )
        }
    }
}
```

<br>
