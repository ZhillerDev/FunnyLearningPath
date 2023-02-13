### 协程作用域

viewModelScope 定义 viewmodel 专属协程作用域

当 viewmodel 失效后，该作用域自动失效

```kotlin
private fun getMarsPhotos() {
    viewModelScope.launch {

    }
}
```

<br>

### 后备属性

> 原生 kotlin 不需要使用 mutableStateOf，也无需指定 State 泛型，此处为 compose 特有

为避免外部直接将 viewmodel 中的属性改为某些不合法值，需要对原属性进行私有化以禁止外部调用；之后用一个公开属性代理原属性

谓词需要注意以下几点以及我们当前可以实现的功能：

1. 原属性名称需以下划线开头，并且用 var 修饰代表可以被修改
2. 公开属性无下划线且用 val 修饰，代表外界只能读而不能写
3. 定义 setter 方法修改原属性，而公开属性绑定原属性，故值同时变化

```kotlin
class CounterVM:ViewModel() {
    // 原可变私有属性
    private var _counter = mutableStateOf(0)
    // 公开属性
    val counter:State<Int> get() = _counter
}
```

<br>
