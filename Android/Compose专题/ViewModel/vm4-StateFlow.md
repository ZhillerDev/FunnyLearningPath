### StateFlow

使用 StateFlow，以流的形式管理属性状态

先定义一个 viewmodel，其中 color 属性以状态流的形式定义

```kotlin
class BasicViewModel : ViewModel() {
    // 定义私有属性，可变状态流
    private val _color = MutableStateFlow(0xFFFFFFFF)
    // 设置公有属性
    var color = _color.asStateFlow()

    // 随机色彩生成方法
    fun genColor() {
        val color = Random.nextLong(0xFFFFFFFF)
        _color.value = color
    }
}
```

<br>

然后于主界面调用

```kotlin
setContent {
    ShortcutDemoTheme {
        Surface(
            modifier = Modifier.fillMaxSize(),
            color = MaterialTheme.colorScheme.background
        ) {
            // 先获取viewmodel
            val vm = viewModel<BasicViewModel>()
            // 使用委托方式，收集状态流并转换为状态
            val color by vm.color.collectAsState()

            // 设置每点击一次Box组件，就改变一下背景颜色
            Box(
                modifier = Modifier
                    .fillMaxSize()
                    .background(Color(color))
                    .clickable {
                        vm.genColor()
                    }
            ) {}
        }
    }
}
```

<br>

### 私有 setter 减少代码量

一般的，我们编写 viewmodel 都会按照：一个私有属性对应一个公有属性，并附带一个可以修改私有属性的公有方法

这会使得代码冗杂，我们可以直接设置一公有变量，并将其 setter 设置为私有，既可以从外部调用（因为 getter 仍然是公有的）又使得外部无法修改，代码缩短到一行

且使用委托构造，可以避免额外书写 `.value` 获取值！

```kotlin
class BasicViewModel : ViewModel() {

    // 公有属性，隐藏setter
    var another by mutableStateOf("")
        private set

    // 修改another属性的方法
    fun changeAnother(value: String) {
        another = value
    }

}
```

<br>
