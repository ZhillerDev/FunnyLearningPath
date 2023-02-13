### 概念

> google 官方 codelab 教程：[点击前往](https://developer.android.com/codelabs/jetpack-compose-advanced-state-side-effects?hl=zh-cn&continue=https%3A%2F%2Fdeveloper.android.com%2Fcourses%2Fpathways%2Fcompose%3Fhl%3Dzh-cn%23codelab-https%3A%2F%2Fdeveloper.android.com%2Fcodelabs%2Fjetpack-compose-advanced-state-side-effects#0)

Compose 中的附带效应是指发生在可组合函数作用域之外的应用状态的变化

LaunchedEffect 会在 Compose 中触发协程作用域限定的附带效应

假设我们在显示主页面前一般都会展示一个“起始页”，它往往是一个广告或者 APP 的 LOGO，持续 N 秒后自动关闭并显示主页面；  
在此期间 APP 进行后端通信拿取数据，这一步骤推荐在协程异步执行，执行完毕再调用方法关闭起始页

<br>

LaunchedEffect 使用方法：
他接受可变数量的参数，当这些参数发生改变时，则重新执行该协程作用域内的效应！  
故当其仅接受一个 true 作为参数时，该协程仅执行一次

```kotlin
// 在组件生命周期内，该LaunchedEffect作用域效应仅执行一次
LaunchedEffect(true){...}
```

<br>

### 起始页案例

> 实现原理：主页面定义一个状态记录是否显示起始页，起始页使用 LaunchedEffect 定义一个延迟后执行关闭当前起始页的方法

起始页代码：

```kotlin
@Composable
fun StartView(onTimeout: ()->Unit){
    Box(
        modifier = Modifier.fillMaxSize(),
        contentAlignment = Alignment.Center
    ) {

        // rememberUpdatedState用于记录最新的状态
        // 因为onTimeout很可能在协程执行过程中发生改变！
        val currentOnTimeout by rememberUpdatedState(onTimeout)

        // 协程设置为true，仅执行一次
        // 延迟3s后自动调用方法
        LaunchedEffect(true){
            delay(3000L)
            currentOnTimeout()
        }

        Text(text = "这是起始页", fontSize = 32.sp)
    }
}
```

<br>

主页面代码：  
传递关闭起始页的状态，正好交予起始页的协程异步处理

```kotlin
@Composable
private fun mainScreen(){
    Surface(
        modifier = Modifier.fillMaxSize(),
        color = MaterialTheme.colorScheme.background
    ) {

        // 定义是否显示起始页的状态，默认是开启的
        var showStartView by remember {
            mutableStateOf(true)
        }

        // 传递showStartView=false，当起始页的协程延迟3s后自动执行代码，关闭起始页
        if(showStartView){
            StartView {
                showStartView=false
            }
        }else{
            Text(text = "这是主页面")
        }
    }
}
```

<br>

### rememberCoroutineScope

开启 scaffold 抽屉界面的方法是一个挂起函数，必须在协程作用域内执行

`rememberCoroutineScope` 可以实现不在组合中开启一个协程作用域（比如在回调中），且一旦退出组合，该作用域自动失效

```kotlin
@Composable
fun CraneHome(
    onExploreItemClicked: OnExploreItemClicked,
    modifier: Modifier = Modifier,
) {
    val scaffoldState = rememberScaffoldState()
    Scaffold(
        scaffoldState = scaffoldState,
        modifier = Modifier.statusBarsPadding(),
        drawerContent = {
            CraneDrawer()
        }
    ) {
        // 定义非组合协程作用域
        val scope = rememberCoroutineScope()
        CraneHomeContent(
            modifier = modifier,
            onExploreItemClicked = onExploreItemClicked,
            openDrawer = {
                // 在协程作用域内执行挂起函数，打开抽屉
                scope.launch {
                    scaffoldState.drawerState.open()
                }
            }
        )
    }
}
```

<br>
