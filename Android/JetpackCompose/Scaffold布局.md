### 搭设基本 Scaffold 页面

> scaffold 组件遵循 Material Design，可以协助开发者迅速构建对应框架页面

<br>

#### 准备工作

首先在 drawable 文件夹内，添加几张 vector images，用作我们的底部导航栏图标

![](../imgs/compose/scaffold/sc1.png)

<br>

在主页面中声明数据类，表示单个图标以及其解释文本

```kotlin
data class Item(
    val name: String,
    val icon: Int
)
```

<br>

新增组件 mainBody，逐一添加三个底部按钮的图标

```kotlin
@Composable
fun mainBody() {
    // 存储当前选中的底部按钮的状态
    var selectedItem by remember {
        mutableStateOf(0)
    }

    // 三个底部按钮
    val items = listOf(
        Item("主页", R.drawable.home),
        Item("列表", R.drawable.list),
        Item("设置", R.drawable.setting)
    )

    ...
}
```

<br>

#### 主体编写

首先是设置 topBar，即顶部导航栏对应按钮

代码很简单，但要注意使用的括号类型以及对应嵌套关系！

```kotlin
Scaffold(
    topBar = {
        TopAppBar(
            title = { Text("主页") },
            navigationIcon = {
                IconButton(onClick = { /*TODO*/ }) {
                    Icon(Icons.Filled.Menu, null)
                }
            }
        )
    },

    ...
){}
```

<br>

紧接着在 `topBar` 属性后面写底部导航栏属性 `bottomBar`

`items.forEachIndexed` 按照索引渲染，vue 的 `v-for` 懂吧，就这个原理！  
依次渲染 `BottomNavigationItem` 即可；

```kotlin
bottomBar = {
    BottomNavigation {
        items.forEachIndexed { index, item ->
            BottomNavigationItem(
                // selectedItem 是内置属性，表示当前选中的Item
                // onClick即切换当前激活的Item
                selected = selectedItem == index,
                onClick = { selectedItem = index },

                // 这几个属性看看英文就懂了，不解释
                icon = { Icon(painterResource(item.icon), null) },
                alwaysShowLabel = false,
                label = { Text(item.name) }
            )
        }
    }
}
```

<br>

这是总体的代码：

```kotlin
@Composable
fun mainBody() {
    var selectedItem by remember {
        mutableStateOf(0)
    }
    val items = listOf(
        Item("主页", R.drawable.home),
        Item("列表", R.drawable.list),
        Item("设置", R.drawable.setting)
    )

    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("主页") },
                navigationIcon = {
                    IconButton(onClick = { /*TODO*/ }) {
                        Icon(Icons.Filled.Menu, null)
                    }
                }
            )
        },
        bottomBar = {
            BottomNavigation {
                items.forEachIndexed { index, item ->
                    BottomNavigationItem(
                        selected = selectedItem == index,
                        onClick = { selectedItem = index },
                        icon = { Icon(painterResource(item.icon), null) },
                        alwaysShowLabel = false,
                        label = { Text(item.name) }
                    )
                }
            }
        }
    ) {
        // 在scaffold里面塞一个box，糊弄一下
        Box(
            modifier = Modifier.fillMaxSize(),
            contentAlignment = Alignment.Center
        ) {
            Text(text = "主页界面")
        }
    }
}

```

![](../imgs/compose/scaffold/sc2.png)

<br>

### 重要组件

#### 底色与内容色易错点

> 为什么要单独设置 scaffold、topappbar 等的内容色和背景色？为的是覆盖掉全局主题色而更好表达内容

我们需要通过下面这两个重要的属性来完成自定义色彩的流程：

1. backgroundColor 背景色
2. contentColor 内容色

<br>

特别注意！设置二者色彩时最好把它们都放到最后来设置，因为即便我们显式指定属性也最好要依照源码中属性排位来指定！

这是 TopAppBar 的源码  
不难发现我们要设置的两个属性排在了倒数二三的位置，这也就意味着我们不能先于它们之上的某些其他属性而设置

```kotlin
fun TopAppBar(
    title: @Composable () -> Unit,
    modifier: Modifier = Modifier,
    navigationIcon: @Composable (() -> Unit)? = null,
    actions: @Composable RowScope.() -> Unit = {},
    backgroundColor: Color = MaterialTheme.colors.primarySurface,
    contentColor: Color = contentColorFor(backgroundColor),
    elevation: Dp = AppBarDefaults.TopAppBarElevation
){...}
```

所以一般来说我们最好这样设置

```kotlin
TopAppBar(
    modifier = Modifier.padding(top = statusBarHeight),
    title = { Text("主页") },
    navigationIcon = {
        IconButton(onClick = { scope.launch { scaffoldState.drawerState.open() } }) {
            Icon(Icons.Filled.Menu, null)
        }
    },

    // 放到最后要记得哦！
    backgroundColor = LightGrayMain,
    contentColor = Color.Black
)
```

<br>

#### BottomNavigation

基本示例

主要分为几个步骤：

1. 定义数据类，表示需要渲染的按钮名字、图标、前往的路由等等
2. 定义一个数据集，数量多寡决定你渲染按钮的个数
3. `items.forEachIndexed` 依次渲染所有按钮
4. `BottomNavigationItem` 为专用的单个按钮空间，需要自己定义属性值

```kotlin
// 定义数据类
data class Item(val name: String, val icon: Int, val route: String)

@Composable
private fun mainBody() {
    // 记录当前激活按钮的索引变量
    var selectedItem by remember {
        mutableStateOf(0)
    }
    // 按钮渲染需要用到的数据集
    val items = listOf(
        Item("主页", R.drawable.ic_home, RouteConfig.R_INDEX),
        Item("列表", R.drawable.ic_user, RouteConfig.R_INFO),
        Item("设置", R.drawable.ic_pwd, RouteConfig.R_SETTING)
    )

    Scaffold(
        bottomBar = {
            BottomNavigation(
                modifier = Modifier.navigationBarsPadding(),
                backgroundColor = Color.White,
            ) {
                items.forEachIndexed { index, item ->
                    BottomNavigationItem(
                        // 当变量selectedItem和当前渲染的组件索引一致时，点亮该按钮
                        selected = selectedItem == index,
                        // 点击按钮，设置变量selectedItem为当前按钮索引
                        onClick = {
                            selectedItem = index
                        },
                        icon = { Icon(painterResource(item.icon), null) },

                        // 是否一直展示标签（若为false，则点击后会以动画的形式显示标签）
                        alwaysShowLabel = false,
                        // 设定标签
                        label = { Text(item.name) },
                        // 按钮激活时的颜色
                        selectedContentColor = Color.Black,
                        // 按钮未激活时的颜色
                        unselectedContentColor = Color.LightGray
                    )
                }
            }
        },
    ) {
    }
}
```

<br>

### 侧边抽屉 drawer

#### 监听抽屉布局返回事件

一般的，我们打开左侧抽屉后，单击返回键就会关闭抽屉而非退出 app

这里需要记录 scaffold 的抽屉状态并利用 backhandler 组件监听返回事件

`scaffoldState` 记录 scaffold 状态  
`scope` 协程作用域，处理挂起的函数

`BackHandler` 这是专门处理 compose 返回操作的监听组件，我们在这里处理关闭抽屉的方法

```kotlin
@Composable
fun drawerDemo() {
    // 记录对应的状态
    val scaffoldState = rememberScaffoldState()
    val scope = rememberCoroutineScope()

    Scaffold(
        // 创建一个简单的抽屉
        drawerContent = {
            Text(text = "抽屉")
        },
        // 这里将scaffold的状态存储起来
        scaffoldState = scaffoldState
    ) {
    }

    // 返回键监听
    BackHandler(
        // 监听开始的条件是：抽屉被打开
        enabled = scaffoldState.drawerState.isOpen
    ) {
        // 此时若点击了返回键，则使用协程scope关掉高抽屉
        scope.launch {
            scaffoldState.drawerState.close()
        }
    }
}
```

<br>
