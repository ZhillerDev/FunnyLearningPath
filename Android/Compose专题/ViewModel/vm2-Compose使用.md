### 计数器案例

viewmodel 依然是管理状态的，只不过他的功能更加全面，它具有以下三大优点：

1. 存活范围大：可以脱离 composition 存在
2. 存活时间长：不会因为横竖屏切换或者进程杀死而导致状态丢失

> 故全局状态最好抽离出一个个 viewmodel 进行管理

<br>

在 compose 中使用 viewmodel 前需要添加依赖：  
`implementation 'androidx.lifecycle:lifecycle-viewmodel-compose:2.5.1'`

<br>

依旧是计数器为案例，编写一个最简单的 viewmodel

```kotlin
// 直接继承ViewModel
class DemoViewModel : ViewModel() {
    // 状态设置为私有
    private val _counter = mutableStateOf(0)
    // 这才是对外暴露的状态
    val counter: State<Int> = _counter
    // 我们需要改变的是私有的状态
    fun increment() {
        _counter.value = _counter.value + 1
    }
}
```

<br>

直接在 `composable` 组件里面实例化指定 `viewmodel` 即可

`viewModel()` 的作用是实例化指定类型的 viewmodel，这是我们新加入的依赖中的方法

直接调用 viewmodel 中的方法时不可以使用方法引用 `如viewmodel::xxx` ，而必须要使用正常的调用形式 `viewmodel.xxx()`

```kotlin
@Composable
fun VMTest() {

    // 特别注意！这里使用viewModel()来进行实例化！！
    val viewModel: DemoViewModel = viewModel()

    Column(
        Modifier.padding(16.dp)
    ) {
        Text(
            text = "${viewModel.counter.value}",
            textAlign = TextAlign.Center
        )
        Button(onClick = { viewModel.increment() }) {
            Text(text = "点我加一")
        }
    }
}
```
