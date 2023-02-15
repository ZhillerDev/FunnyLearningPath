### HomeScreen

> 以下两块分解代码均存放于 `HomeScreen.kt` 文件下

<br>

#### CharacterImageCard

数据展示通过一块小卡片，使用基础布局即可完成

`character` 形参为上级 `LazyColumn` 中的 item 传递而来，下面将补全代码

```kotlin
@Composable
fun CharacterImageCard(character: Character) {
    val imagerPainter = rememberImagePainter(data = character.image)

    Card(
        shape = MaterialTheme.shapes.medium,
        modifier = Modifier.padding(16.dp)
    ) {
        Box {

            Image(
                painter = imagerPainter,
                contentDescription = null,
                modifier = Modifier
                    .fillMaxWidth()
                    .height(200.dp),
                contentScale = ContentScale.FillBounds
            )

            Surface(
                color = MaterialTheme.colors.onSurface.copy(alpha = .3f),
                modifier = Modifier.align(Alignment.BottomCenter),
                contentColor = MaterialTheme.colors.surface
            ) {
                Column(
                    modifier = Modifier
                        .fillMaxWidth()
                        .padding(4.dp)
                ) {
                    Text(text = "Real name: ${character.actor}")
                    Text(text = "Actor name: ${character.name}")
                }
            }
        }
    }
}
```

<br>

#### HomeScreen

主页面渲染逻辑，使用 `LazyColumn` 渲染 `CharacterImageCard`

注意组件 `CircularProgressIndicator` 存在于 `androidx.compose.material:material` 中，部分情况下会无法自动导入，就需要我们手动 import 后即可

```kotlin
@Composable
fun HomeScreen() {
    // 初始化viewmodel
    val homeViewModel = viewModel(modelClass = HomeViewModel::class.java)
    // 将流转换为state
    val state by homeViewModel.state.collectAsState()

    LazyColumn {
        if (state.isEmpty()) {
            item {
                CircularProgressIndicator(
                    modifier = Modifier
                        .fillMaxSize()
                        .wrapContentSize(align = Alignment.Center)
                )
            }

        }

        items(state) { character: Character ->
            CharacterImageCard(character = character)
        }
    }
}
```

<br>

### MainActivity

最后回到我们的 `MainActivity.kt`

直接调用 HomeScreen 即可渲染

```kotlin
// 注意在这里设置好Android入口点
@AndroidEntryPoint
class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            CharacterTheme {
                // A surface container using the 'background' color from the theme
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colors.background

                ) {
                    HomeScreen()
                }
            }
        }
    }
}
```

<br>
