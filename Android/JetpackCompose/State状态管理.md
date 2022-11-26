### Stateless & Stateful

**Stateless Composable**

1. 仅依赖参数变化的组件
2. 无状态，可以理解为值恒定，不发生改变
3. stateless 为一纯函数，当参数不变时 UI 不变，且不会参与调用方的重组

<br>

**Stateful Composable**  
顾名思义，有状态的组件，可以实时更新数值

<br>

### 快速上手

#### state 赋值方式

一共有三种：直接赋值、解构赋值、代理（最常用）

```kotlin
// 使用by代理，给予state0 getter、setter方法
// 最常用的方法
var state0 by mutableStateOf(0)

// 解构赋值，不常用
val (state1, state2) = mutableStateOf(0)
```

<br>

#### remember

> mutabeStateOf 的调用一定要出现在 remember 中，不然每次重组都会创建新的状态

remember 保证了参数的实时更新，且其能接收可变长参数，并对其中任何一个参数都可重新计算并返回结果

```kotlin
var counter by remember {
    mutableStateOf(0)
}
```

<br>

#### 状态上提

根据 react 的解释：把所有状态全部提升到顶层，则后续层次即可轻松的进行调用；

为此，简单准备以下模板，实现点击按钮动态增减数字的计数器：

该组件有三个重要参数：

1. counter 一个被上提的状态，后续会单独编写一个组件来进行管理
2. onPlus 增加数值的回调方法
3. onMinus 减小数字的回调方法

```kotlin
@Composable
fun CounterCompose(
    // 三个重要参数
    counter: Int,
    onPlus: () -> Unit,
    onMinus: () -> Unit
) {
    // 布局就不细讲了，很简单的左右排列
    Row(
        modifier = Modifier.padding(16.dp)
    ) {
        Text(
            text = "$counter",
            modifier = Modifier
                .fillMaxSize()
                .weight(1f),
            style = TextStyle(
                fontSize = 48.sp
            )
        )
        Column(
            Modifier.weight(1f)
        ) {
            // 调用方法别忘了尾部小括号，因为你不写的话IDE也不会报错，导致你忽略掉！
            Button(onClick = { onPlus() }) {
                Text(text = "plus")
            }
            Spacer(modifier = Modifier.height(2.dp))
            Button(onClick = { onMinus() }) {
                Text(text = "minus")
            }
        }
    }
}
```

<br>

状态上提函数

定义一个 `stateless` 状态变量 counter，并调用组件 `CounterCompose`

此时我们就可以直接在 `StateDemo` 方法内部进行所有顶层状态的管理了！

```kotlin
@Composable
fun StateDemo() {
    var counter by remember {
        mutableStateOf(0)
    }

    CounterCompose(counter = counter, onPlus = { counter++ }) {
        if (counter > 0) counter--
    }
}
```

<br>

### 状态持久化

<br>

#### 避免重建

一般的，我们无需让 android 在不同 activity 之间不断的重建造成冗余损耗  
故可以直接在 android.manifest 内部的某个 activity 内添加以下属性，即可关闭重建机制

`android:configChanges=""`

<br>

#### rememberSaveable

rememberSaver 可以保存任意类型的对象，但这里我们直接使用 parceable 类型；

使用前记得在 build.gradle 里面添加插件  
`id 'kotlin-parcelize'`

<br>

非常好！现在书写一个简单的数据类

```kotlin
// 该注解仅在添加插件后生效
@Parcelize
data class City(
    val name: String,
    val country: String
) : Parcelable
```

<br>

定义一个 saver，在一个 activity 触发以下两个状态时做出对应的反应：

1. onSaver 即保存时，把数据存储
2. onRestore 即重建后取出对应的数据

```kotlin
val CitySaver = listSaver<City, Any>(
    save = { listOf(it.name, it.country) },
    restore = { City(it[0] as String, it[1] as String) }
)
```

<br>

最后就是使用 `rememberSaveable` 方法来记录跨页面的数据，记得传参 `stateSaver`

```kotlin
@Composable
fun CityScreen() {
    var selectedCity = rememberSaveable(stateSaver = CitySaver) {
        mutableStateOf(City("one", "two"))
    }
}
```

<br>

### ViewModel
