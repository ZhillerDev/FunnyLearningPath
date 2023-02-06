### Composable 简述

@Composable 注解使得 Compose 的编译器插件会把它当作影响函数类型的因子之一

实际上被标注@Composable 的函数其类型应该为：`@Composable ()->Unit`

`Box{}` 是 kotlin 高阶函数的简写，完整形式应该为：`Box{content=xxx}`

```kotlin
@Composable
fun IndexViewInit() {
    Box(modifier = Modifier.padding(10.dp)) {
        ...
    }
}
```

<br>

### Composable 性质

Composable 函数可视为一棵树状结构，并按照 DFS 的方式逐一渲染

`Compose 编译器` 会对添加了 `@Composable` 注解的函数自动添加一个参数，这和挂起函数被编译后自动添加一个 continuation 类型的形参是一致的！

<br>
