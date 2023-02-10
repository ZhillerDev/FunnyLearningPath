### 重组与刷新

#### 重组

Compose 则通过重组刷新 UI，且只有状态发生更新的 Composable 才会参与重组

Composable 会以任意顺序执行

重组中的 Composable 并不一定执行在 UI 线程，它们可能在后台线程池中并行执行

动画等场景中每一帧的变化都可能引起 Composable 的执行，因此 Composable 有可能会短时间内反复执行

Composable 最终总会依据最新的状态正确地完成重组

<br>

#### stable

kotlin 中存在许多稳定的对象，诸如基本 int string boolean 等

而被 var 声明的对象则是可变的，不稳定的；

使用@Stable 注解一个对象，将使其及其下属所有子类全部都声明为稳定的，编译器解析时也将依据稳定性来进行处理；

而 immutable 注解和 stable 注解差不多，都表示对象稳定，只是 immutable 则是完全稳定！

<br>

### 生命周期

> 不同于 activity，compose 没有复杂的前后台切换销毁问题，故生命周期很简单

<br>

#### 三大构件

1. OnActive（添加到视图树）:即 Composable 被首次执行，在视图树上创建对应的节点。
2. OnUpdate（重组）:Composable 跟随重组不断执行，更新视图树上的对应节点。
3. onDispose（从视图树移除）：Composable 不再被执行，对应节点从视图树上移除。

<br>

#### DisposableEffect

下面代码演示如何封装返回键点击事件的方法

即使用 `DisposableEffect` 方法来注册某些事件，添加对应回调方法

因为我们传入 `DisposableEffect` 的参数是一个 dispatcher，故会在每次 `onUpdate` 方法中重建一次 `backCallback`  
而如果传入的是常量，则仅在 onActive 执行一次！！！

```kotlin
@Composable
fun backPressHandler(enable: Boolean = true, onBackPressed: () -> Unit) {

    // 验证非空后注册onBackPressedDispatcher
    val backDispatcher = checkNotNull(LocalOnBackPressedDispatcherOwner.current) {
        "no!!!you cannot!!"
    }.onBackPressedDispatcher

    // 编写返回键点击后的回调方法，就直接使用我们传入的onBackPressed方法
    val backCallback = remember {
        object : OnBackPressedCallback(enable) {
            override fun handleOnBackPressed() {
                onBackPressed()
            }
        }
    }

    // DisposableEffect注册指定的dispatcher
    DisposableEffect(backDispatcher) {
        // 为对应dispatcher添加回调函数
        backDispatcher.addCallback(backCallback)
        // 所有的DisposableEffect都必须有这一回调，用来处理副作用
        onDispose {
            backCallback.remove()
        }
    }
}
```

<br>

调用起来就非常简单了，任意一个组件内部直接使用封装好的方法即可！

```kotlin
@Composable
fun demo(){
    backPressHandler{
        ...
    }
}
```

<br>

#### rememberCoroutineScope

用于在非 `composable` 环境下使用协程，譬如在 `button` 的 `onclick` 方法内部！

```kotlin
@Composable
fun CoroutineDemo(scaffoldState: ScaffoldState = rememberScaffoldState()) {
    val scope = rememberCoroutineScope()
    Scaffold(scaffoldState = scaffoldState) {
        Column {
            Button(onClick = {
                // 调用协程改变状态，显示snackbar
                scope.launch {
                    scaffoldState.snackbarHostState.showSnackbar("give it up")
                }
            }) {
                Text(text = "press me!!!")
            }
        }
    }
}
```

<br>

#### rememberUpdatedState

开启一个协程，并且在重组过程中协程不中断，而是持续获取最新状态；

LaunchedEffect 会在他的 key 发送变化时启动协程

```kotlin
@Composable
fun ScreenDemo(onTimeout: () -> Unit) {
    val currentTimeout by rememberUpdatedState(newValue = onTimeout)
    LaunchedEffect(key1 = Unit) {
        delay(1000)
        currentTimeout()
    }
}
```
