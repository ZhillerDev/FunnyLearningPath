### 基本手势

#### clickable

该 modifier 可以检测单击、双击、长按事件

`combinedClickable` 可点击方法

1. enabled 可否点击状态
2. onLongClick 长按事件函数，以此类推

```kotlin
@OptIn(ExperimentalFoundationApi::class)
@Composable
fun ClickGesture() {

    // 非必要，使用一个状态来管理按钮是否可点击
    var enableState by remember {
        mutableStateOf<Boolean>(true)
    }

    Box(
        Modifier
            .size(200.dp)
            .background(Color.Cyan)

            // 注册点击事件
            .combinedClickable(
                enabled = enableState,
                onLongClick = {
                    Log.d(TAG, "ClickGesture: 长按")
                },
                onDoubleClick = {
                    Log.d(TAG, "ClickGesture: 双击")
                },
                onClick = {
                    Log.d(TAG, "ClickGesture: 单击")
                }
            )
    )
}
```

<br>

#### swiper

以下代码实现了一个简单的吸附开关（拖动滑块会自动左右吸附）

比较复杂，请看代码注释

```kotlin
// 定义两个枚举类表示状态
enum class Status {
    CLOSE, OPEN
}

// 实验性方法必须要加上这个注释
@OptIn(ExperimentalMaterialApi::class)
@Composable
fun SwiperDemo() {

    // 滑块的总宽度
    var blockSize = 48.dp
    // dp转换到px单位
    var blockSizePx = with(LocalDensity.current) { blockSize.toPx() }
    // 记录swiper的state
    var swipeableState = rememberSwipeableState(initialValue = Status.CLOSE)

    // 外部盒子包裹
    Box(
        modifier = Modifier
            .size(height = blockSize, width = blockSize * 2)
            .background(Color.LightGray)
    ) {
        // 内部盒子作为整个吸附按钮
        Box(
            modifier = Modifier
                // 根据swiper状态实时更新的偏移值来更新开关偏移值
                .offset {
                    IntOffset(swipeableState.offset.value.toInt(), 0)
                }
                .swipeable(
                    // 参数1：记录swiper状态
                    state = swipeableState,
                    // 参数2：吸附锚点，使用映射形式描述
                    anchors = mapOf(
                        0f to Status.CLOSE,
                        blockSizePx to Status.OPEN
                    ),
                    // 参数3：吸附的临界阈值
                    thresholds = { from, to ->
                        if (from == Status.CLOSE) {
                            FractionalThreshold(0.3f)
                        } else {
                            FractionalThreshold(0.5f)
                        }
                    },
                    // 参数4：拖动的方向，仅能设置为垂直或者水平方向！
                    orientation = Orientation.Horizontal
                )
                .size(blockSize)
                .background(Color.DarkGray)
        )
    }
}
```

<br>
