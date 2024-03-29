### 前言

transition 是为了同步多个动画之间关系的，并不仅仅是为了做组件移动或者形态变化的！

<br>

### updateTransition

`updateTransition` 用来管理多个动画同时进行

需求：点击 Box，实现高度变高以及蓝色变成红色的两个动画，且动画同时进行

下面为完整代码，为便于理解，这是我们使用 updateTransition 的基本步骤：

1. 因为 Box 的完整状态只有两个，分别是收缩和展开，那么就先定义一个可变状态 isExpanded 以布尔值表示
2. 使用 updateTransition 记住 isExpanded，每当其变化时都会触发一次更新事件
3. 借助委托方法扩展 transition 的动画，其中的 lambda 中的形参就是我们在 updateTransition 中绑定的状态
4. 将队员扩展后的变量导入到组件内部

```kotlin
@Composable
fun TransitionBoxSizeComp() {
    var isExpanded by remember {
        mutableStateOf(false)
    }
    val transition = updateTransition(targetState = isExpanded, label = "trans")

    val color by transition.animateColor(label = "") {
        if (it) Color.Red else Color.Blue
    }
    val size by transition.animateDp(label = "") {
        if (it) 120.dp else 60.dp
    }

    Box(
        Modifier
            .fillMaxWidth()
            .background(color)
            .height(size)
            .clickable {
                isExpanded = !isExpanded
            }
    )
}
```

<br>

### 简单切换动画

> 需求：给予一个元素，在不同 tab 之间点击时，该元素以动画的形式移动到指定 tab 范围内

`updateTransition` 可视为定义一个 transition 监听，监听指定项的变化以进行管理

`transition.animateDp` 扩展函数实现平滑移动  
`transition.animateColor` 扩展函数实现平滑切换颜色

```kotlin
@Composable
private fun HomeTabIndicator(
    // 表示被点击的tab在当前激活的tab的那一边（有左边和右边）
    tabPositions: List<TabPosition>,
    // 表示当前所有的tabs
    tabPage: TabPage
) {
    // updateTransition监听当前tab的点击变化
    val transition = updateTransition(tabPage, label = "Tab indicator")

    // transition.animateDp为每一个状态指定目标值
    // 即向左或者向右移动
    val indicatorLeft by transition.animateDp(label = "Indicator left") { page ->
        tabPositions[page.ordinal].left
    }
    val indicatorRight by transition.animateDp(label = "Indicator right") { page ->
        tabPositions[page.ordinal].right
    }

    // 设置颜色切换时的动画
    val color by transition.animateColor(label = "Border color") { page ->
        if (page == TabPage.Home) Purple700 else Green800
    }
}
```

<br>

### 骨架屏加载

首先我们需要设计骨架屏，即灰色的占位符

通过 copy 函数指定一个 alpha 通道设置透明度，alpha 使用动画控制动态修改浅深，以此实现骨架屏加载的效果

```kotlin
@Composable
private fun LoadingRow() {
    Row(
        modifier = Modifier
            .heightIn(min = 64.dp)
            .padding(16.dp),
        verticalAlignment = Alignment.CenterVertically
    ) {
        Box(
            modifier = Modifier
                .size(48.dp)
                .clip(CircleShape)
                // 背景颜色直接copy亮灰色，设置透明度通道实现闪烁加载效果
                .background(Color.LightGray.copy(alpha = alpha))
        )
        Spacer(modifier = Modifier.width(16.dp))
        Box(
            modifier = Modifier
                .fillMaxWidth()
                .height(32.dp)
                .background(Color.LightGray.copy(alpha = alpha))
        )
    }
}
```

<br>

在组件上方添加状态管理

`infiniteTransition` 定义一个无限循环的 transition，需要使用 `rememberInfiniteTransition` 管理状态

`infiniteTransition.animateFloat` 被定义动画的数值类型为浮点型

`animationSpec` 仅接收 `infiniteRepeatable` 作为接口的实现函数，在此处设置循环动画  
`keyframes` 定义动画时长以及对应关键帧所达到的值

`repeatMode` 重复模式，`RepeatMode.Reverse` 模式可以保证首尾衔接顺畅

```kotlin
@Composable
private fun LoadingRow() {

    // 管理无限循环动画的状态
    val infiniteTransition = rememberInfiniteTransition()

    // 控制alpha通道数值大小动态变化的动画
    // 因为数值为浮点，故使用animateFloat
    val alpha by infiniteTransition.animateFloat(
        initialValue = 0f,  // 最初为透明的0f
        targetValue = 1f,   // 最终为纯色1f

        // 设置循环动画
        animationSpec = infiniteRepeatable(
            keyframes自定义动画
            animation = keyframes {
                // 持续1000毫秒
                durationMillis = 1000
                // 当到达500毫秒时，alpha的值为0.7f
                0.7f at 500
            },
            // 设置为循环模式为：从头到尾又从尾到头一直循环
            // 默认模式下为：从头到尾又从头开始（会出现衔接突兀情况）
            repeatMode = RepeatMode.Reverse
        )
    )
    ...
}
```

<br>
