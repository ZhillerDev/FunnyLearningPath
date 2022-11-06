### 分离式组件

我们可以把组件移动到另外一个 kt 文件中书写，并在主文件进行引用即可，有利于解耦；

<br>

### 实例展示与 modifier 分析

下面展示一个简单的组件案例，并对其中的常见代码做出重点标注；

下图为运行后的完整结果：

![](../imgs/compose/bas-component-modifier/bcm1.png)

```java
@Composable
fun PhotoGraphCard(modifier: Modifier = Modifier) {
    Row(
        modifier = Modifier
            .clip(RoundedCornerShape(4.dp))
            .background(MaterialTheme.colors.surface)
            .clickable(onClick = {})
            .padding(16.dp)
    ) {
        Surface(
            modifier = Modifier.size(50.dp),
            shape = CircleShape,
            color = MaterialTheme.colors.onSurface.copy(alpha = .2f)
        ) {
            Image(
                painter = painterResource(id = R.drawable.ic_launcher_background),
                contentDescription = "null"
            )
        }
        Column(
            modifier = Modifier
                .padding(start = 8.dp)
                .align(Alignment.CenterVertically)
        ) {
            Text(text = "lily", fontWeight = FontWeight.Bold)
            Text(text = "3 minutes ago", style = MaterialTheme.typography.body2)
        }
    }
}
```

#### 完整结构

最外层套一个 Row，设置水平布局，左边为图像，右边为内容；

Surface 主要用来承载内部的 Image 组件；

Column 垂直布局，存放文本内容，一上一下两个文本；

<br>

#### 代码分析

`clip` 设置裁剪；

`RoundedCornerShape` 裁剪圆角，类似于 CSS 的 border-radius；

`background` 背景设置；

`MaterialTheme.colors.surface` 获取主题色；

`clickable` 设置点击事件（添加这个修饰后，此 Row 被点击后即出现水波纹效果！）

`padding` 设置内边距；

> 注意：修饰符之间是有先后顺序的！当你把 clickable 放在 padding 后面的话，水波纹效果将不会出现在 padding 里面

```java
Row(
    modifier = Modifier
        .clip(RoundedCornerShape(4.dp))
        .background(MaterialTheme.colors.surface)
        .clickable(onClick = {})
        .padding(16.dp)
) {
    ...
}
```

<br>

size 设置当前整个组件的大小；

shape 直截了当设置外形轮廓；

color 配置颜色；

copy(alpha = .2f) 表示将获取到的变量中的属性 alpha 更改为 0.2f，其余的属性值不变；

```java
Surface(
    modifier = Modifier.size(50.dp),
    shape = CircleShape,
    color = MaterialTheme.colors.onSurface.copy(alpha = .2f)
) {
    ...
}
```

<br>

`Alignment.CenterVertically` 垂直居中，当然还有水平居中；

`padding(start = 8.dp)` 左 padding 为 8 个 dp；

> 注意：两个文本中第二个文本使用了浅灰色，是根据官方推荐的 ”组件层次可以用颜色深浅表达”

```java
Column(
    modifier = Modifier
        .padding(start = 8.dp)
        .align(Alignment.CenterVertically)
) {
    Text(text = "lily", fontWeight = FontWeight.Bold)
    Text(text = "3 minutes ago", style = MaterialTheme.typography.body2)
}
```

<br>

### END

> 很水，但是内容很水 🥇
