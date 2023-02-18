### 前言与基本概念

本文章讲解属性驱动相关动画

这里将介绍比较常用的几个动画状态转换函数

<br>

#### Spring

Spring 用来指定动画切换的间隔时间长短与是否闪烁

spring 值在以下三种情况下有不同的特性：

1. 等于 1：瞬间切换（即无动画状态）
2. 大于 1：随着数值增大，切换速度越慢
3. 小于 1 但大于 0：动画开始闪烁，数值越小闪烁频率越高！

```kotlin
object Spring {
    const val StiffnessHigh = 10_000f   // 最慢速度
    const val StiffnessMedium = 1500f
    const val StiffnessMediumLow = 400f
    const val StiffnessLow = 200f
    const val StiffnessVeryLow = 50f
    const val DampingRatioHighBouncy = 0.2f
    const val DampingRatioMediumBouncy = 0.5f
    const val DampingRatioLowBouncy = 0.75f
    const val DampingRatioNoBouncy = 1f     // 瞬间切换
    const val DefaultDisplacementThreshold = 0.01f
}
```

> 最近测试了一下，发现 spring 值在 2.5-3 之间是比较正常的速度

<br>

### animateColorAsState

下面展示一个点击 Box 组件然后渐变切换背景颜色的例子

大家可以这么理解：每次点击一下 Box，就会切换 yellow 的值，而 color 的作用是监听 yellow 的变化，并且按照一定频率连续的返回一个颜色值（譬如红色变成黄色，那么 color 会不断按频率返回从红到黄之间颜色值，直到彻底切换成黄色）  
这样子如果连续起来就会形成一个动画了！

```kotlin
@Composable
fun ColorAsStateComp() {
    // 记住当前颜色
    var yellow by remember { mutableStateOf(true) }
    val color = animateColorAsState(
        // 当颜色变化时触发动作
        targetValue = if (yellow) Yellow else Red,
        // 动画过渡时长设置
        animationSpec = spring(3f),
        // 动画执行结束回调
        finishedListener = {
        }
    )

    Box(
        Modifier
            .fillMaxWidth()
            .height(120.dp)
            .background(color.value)
            .clickable {
                yellow = !yellow
            }
    )
}
```

<br>

### animateIntAsState

点按钮为数字+1，设置动画

```kotlin
@Composable
fun IntAsStateComp() {
    var count by remember {
        mutableStateOf(0)
    }
    val changeCount = animateIntAsState(
        targetValue = count,
        animationSpec = spring(3f)
    )

    Row(
        Modifier.fillMaxWidth(),
        verticalAlignment = Alignment.CenterVertically
    ) {
        Text(text = changeCount.value.toString())
        Button(onClick = { count++ }) {
            Text(text = "+")
        }
    }
}
```

<br>

### animateScrollTo

> 该方法接收一个数值，表示当前滚动位置，以平滑的方式滚动到指定位置

`animateScrollTo` 是挂起函数，必须要在协程作用域内执行

<br>

#### 实现回到顶部

需求：给予一个有 1000 项的列表，双击列表实现回到顶部

方法很简单，定义一个滚动状态监听 column 所处位置，然后定义双击方法使用 `animateScrollTo` 滚到顶部就好了

```kotlin
@Composable
fun ScrollBoxesSmooth() {

    // 记住滚动状态
    val state = rememberScrollState()
    // 记住协程作用域
    var scope = rememberCoroutineScope()

    Column(
        modifier = Modifier
            .padding(12.dp)
            .verticalScroll(state)

            // pointerInput可以实现双击手势检测
            .pointerInput(Unit) {
                detectTapGestures(
                    onDoubleTap = {
                        scope.launch {
                            // 滚动到顶部，即数值为0处
                            state.animateScrollTo(0)
                        }
                    }
                )
            }
    ) {
        // 渲染1000个列表项
        repeat(1000) {
            Text("Item $it", modifier = Modifier.padding(4.dp))
        }
    }
}
```

<br>
