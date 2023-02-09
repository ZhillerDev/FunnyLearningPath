### 准备阶段

Coil 官方中文文档：https://coil-kt.github.io/coil/README-zh/

下载依赖：

```java
// 原生
implementation("io.coil-kt:coil:2.2.2")

// compose
implementation("io.coil-kt:coil-compose:2.2.2")
```

<br>

由于 Coil 使用 okhttp 请求，故需要在 manifest.xml 添加网络权限：  
`<uses-permission android:name="android.permission.INTERNET" />`

<br>

### 分步快速上手

> 全部参考官方文档并做精简处理

<br>

#### AsyncImage

图片会自适应所有布局

可根据当前盒子大小按照原比例拉伸图片，并保证图片宽度占满整个盒子宽度（在不对 AsyncImage 设置任何 modifier 的情况下）

```kotlin
@Composable
fun CoilBasic(){
    // 异步加载网络图片，使用内置okhttp请求
    // 这是一张普通的苹果图片
    AsyncImage(
        model = "https://img0.baidu.com/it/u=4" +
            "023062467,4103551361&fm=253&fmt=auto&app=1" +
            "38&f=JPEG?w=650&h=434",
        contentDescription = null)
}
```

<br>

或者直接在 Image 组件中使用

rememberAsyncImagePainter 接收图片 URL 作为参数

```kotlin
@Composable
fun CoilImage(){
    Image(
        ...
        painter = rememberAsyncImagePainter(model = IMAGE_URL),
        contentDescription = null
    )
}
```

<br>

更深入的 AsyncImage 配置

```kotlin
@Composable
fun CoilAsyncDefine(){
    AsyncImage(
        // ImageRequest.Builder构建自定义图片显示方式
        model = ImageRequest.Builder(LocalContext.current)
            .data("https://pic-go-bed.oss-cn-beijing.aliyuncs.com/img/20220316151929.png")
            .crossfade(true)
            .build(),

        // 其他的不怎么重要的设置
        contentDescription = stringResource(R.string.description),
        placeholder = painterResource(id = R.drawable.place_holder),
        error = painterResource(id = R.drawable.error),
        onSuccess = {
            Log.d(TAG, "success")
        }
    )
}
```

<br>

#### SubcomposeAsyncImage

该组件为 AsyncImage 的变体，可以轻易地通过 loading 属性设置加载时动画

```kotlin
SubcomposeAsyncImage(
    model = "https://example.com/image.jpg",
    loading = {
        CircularProgressIndicator()
    },
    contentDescription = stringResource(R.string.description)
)
```

<br>

可以直接检查 painter 状态实现更复杂的逻辑

```kotlin
SubcomposeAsyncImage(
    model = "https://example.com/image.jpg",
    contentDescription = stringResource(R.string.description)
) {
    val state = painter.state
    if (state is AsyncImagePainter.State.Loading || state is AsyncImagePainter.State.Error) {
        CircularProgressIndicator()
    } else {
        SubcomposeAsyncImageContent()
    }
}
```

<br>

#### AsyncImagePainter

若无法使用 AsyncImage 组件，可以使用 AsyncImagePainter 代替

```kotlin
val painter = rememberAsyncImagePainter(
  model = ImageRequest.Builder(LocalContext.current)
    .data("https://pic-go-bed.oss-cn-beijing.aliyuncs.com/img/20220316151929.png")
    .build()
)
if (painter.state is AsyncImagePainter.State.Loading) {
  CircularProgressIndicator()
}
Image(
  painter = painter,
  contentDescription = stringResource(R.string.description)
)
```

<br>

### 实战
