### 图标库概述

google 将许多 app 常用的小图标都打包成一个库给我们这些开发者快速调用了；

这是图标的官方网址：https://fonts.google.com/icons

图标库的导入非常简单，打开 build.gradle:app，添加依赖：  
（截止 2022-9-26，本库的最新版本就是 1.2.1，当然你可以自己改）  
`implementation 'androidx.compose.material:material-icons-extended:1.2.1'`

<br>

### 图标列表实战

#### 定义图标数据集

首先使用 `data class` 定义一个图标数据集

包含两个属性，第一个属性为图标名称，第二个属性为图标；

使用 `Icons` 进行图标库中图标的调用，图标的默认属性就是 `ImageVector`

> ImageVector 是一个常用的图标类型，我们使用 Icon 组件时会用到

```java
data class IconDetail(val text: String, val img: ImageVector)

val icons = listOf(
    // 图标库中的图标都需要使用Icons进行调用！！！
    IconDetail("done", Icons.Default.Done),
    IconDetail("add", Icons.Default.Add),
    IconDetail("cake", Icons.Default.Cake)
)
```

<br>

#### 指定列表显示图标

这里使用了 LazyColumn 快速构造列表；

items(icons) 直接把图标数据集传入，依次遍历渲染；

Divider 一条简单的分割线，把每条数据都划分！

```java
@Composable
fun ScaffoldWidget() {
    LazyColumn(
        modifier = Modifier
            .fillMaxWidth()
            .padding(all = 10.dp)
    ) {
        items(icons) {
            Row(
                modifier = Modifier.padding(all = 20.dp)
            ) {
                Icon(imageVector = it.img, contentDescription = it.text)
                Text(text = it.text, modifier = Modifier.padding(start = 10.dp))
            }

            Divider(modifier = Modifier.fillMaxWidth())
        }
    }
}
```

<br>

#### 渲染结果与完整代码

![](../imgs/compose/composeicon/ci1.png)

代码清单：

这是一个但组件，需要你把它贴到 MainActivity 里面去查看，或者使用另一个 activity 进行渲染！

```go
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.Divider
import androidx.compose.material.Icon
import androidx.compose.material.Text
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Add
import androidx.compose.material.icons.filled.Cake
import androidx.compose.material.icons.filled.Done
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.unit.dp

@Composable
fun ScaffoldWidget() {
    LazyColumn(
        modifier = Modifier
            .fillMaxWidth()
            .padding(all = 10.dp)
    ) {
        items(icons) {
            Row(
                modifier = Modifier.padding(all = 20.dp)
            ) {
                Icon(imageVector = it.img, contentDescription = it.text)
                Text(text = it.text, modifier = Modifier.padding(start = 10.dp))
            }

            Divider(modifier = Modifier.fillMaxWidth())
        }
    }
}

data class IconDetail(val text: String, val img: ImageVector)

val icons = listOf(
    IconDetail("done", Icons.Default.Done),
    IconDetail("add", Icons.Default.Add),
    IconDetail("cake", Icons.Default.Cake)
)
```

<br>
