### LazyVerticalGrid

LazyGrid 包含两个布局实现，一个就是我们现在讲的垂直 Grid，还有一个就是水平 Grid（这个用的比较少）

Grid 在最新的 compose 已升级为正是方法而非实验方法，下面我的代码使用的是旧版本所以需要添加实验性注解

LazyVerticalGrid 可以快速的实现瀑布流效果，可以配合下拉加载或者上拉刷新使用

<br>

#### 小案例

> 目的：使用 LazyVerticalGrid 动态渲染组件

LazyVerticalGrid 具体属性及其用法请详细查看注释

```kotlin
// 旧版本的compose中的LazyVerticalGrid依然为实验性方法，需要添加此注解
@OptIn(ExperimentalFoundationApi::class)
@Composable
fun IndexViewInit(navController: NavController) {
    Box(modifier = Modifier.padding(10.dp)) {
        LazyVerticalGrid(
            modifier = Modifier.fillMaxWidth(),

            // 瀑布流列数，这里固定为1列
            cells = GridCells.Fixed(1),

            // 元素之间的水平与垂直间隙
            verticalArrangement = Arrangement.spacedBy(16.dp),
            horizontalArrangement = Arrangement.spacedBy(16.dp),

            // 渲染的元素
            content = {
                // items接收一个参数，表示欲渲染的元素数量
                items(3) {
                    // cardComp是我编写的一个组件，传入的it即为当前渲染到的元素索引
                    cardComp(it)
                }
            }
        )
    }
}
```

<br>

接上方渲染的外部元素 cardComp，使用了标准的卡片布局

这里通过 when 判断当前索引从而调用指定的外部组件！

> 这里的三个外部组件大家就自己去写了，代码太多我也不可能一一展示出来

```kotlin
@Composable
fun cardComp(index: Int) {
    Card(
        backgroundColor = LightGrayMain,
        contentColor = Color.Gray,
    ) {
        Column(
            Modifier
                .padding(horizontal = 8.dp, vertical = 16.dp)
                .fillMaxWidth(),
            verticalArrangement = Arrangement.Center,
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            // 判断索引值，然后调用外部组件
            when (index) {
                0 -> BasicTextFieldComp()
                1 -> SearchTextFieldComp()
                2 -> PasswordFieldComp()
            }
        }
    }
}
```

<br>
