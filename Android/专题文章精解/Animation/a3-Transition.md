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
