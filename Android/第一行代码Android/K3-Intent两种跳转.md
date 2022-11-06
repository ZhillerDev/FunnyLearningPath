### 显式 intent

新建一个 activity，名字就叫做 `SecondActivity.kt`；

使用 Intent 构造函数，传入 activity 类作为参数，即可得到该 activity 的意图；

> 注意！虽然我们创建的 activity 都是 kt 文件，但是作为 intent 参数时使用的是 class.java

```kotlin
btn.setOnClickListener {
    val intent = Intent(this,SecondActivity::class.java)
    startActivity(intent)
}
```

<br>

### 隐式 intent

#### action 启动

新建 `SecondActivity.kt`

打开 `manifests` 文件；  
可以看见，在我们创建新的 activity 的同时，智能的 as 就自动在这注册了该 activity；

我们需要效仿主 activity，同样定义一个 intent-filter 标签；

`action` 定义动作，可以使用 android 自带的动作，或者自己随便取个名字（下面代码名字是自定义的）；

`category` 定义目录，若你只想通过 action 构造 intent，那么 category 就必须按照以下代码固定格式写！

```xml
<intent-filter>
    <action android:name="secondactivity_action"/>
    <category android:name="android.intent.category.DEFAULT"/>
</intent-filter>
```

现在我们仅需要把 action 完整的作为 intent 的参数就可以构造了（不需要再指定 this 了！）

```kotlin
btn.setOnClickListener {
    val intent = Intent("secondactivity_action")
    startActivity(intent)
}
```

<br>

#### category 附加

需要注意这么一个事实：同一个 activity  
仅能有一个 action，  
可以有多个 category

值得注意的是，category 和 action 一样，名称是 `可以完全自定义的`

此时，我们在原默认 `category` 的基础上再新增我们自定义的 category  
不可以删去默认的 DEFAULT，否则会导致 intent 无法启动并崩溃；

```kotlin
<intent-filter>
    <action android:name="secondactivity_action"/>
    <category android:name="android.intent.category.DEFAULT"/>
    <category android:name="secondactivity_category"/>
</intent-filter>
```

之后在按钮点击事件中，使用 addCategory 方法做出附加；  
此时的 action 依然保留！

> 请注意两个准则：  
> 可以单独使用 action 启动 intent  
> category 必须以 action 作为前提才可以使用（类似于附加）

```kotlin
btn.setOnClickListener {
    val intent = Intent("secondactivity_action")
    intent.addCategory("secondactivity_category")
    startActivity(intent)
}
```

<br>

### Android 自带的 intent

#### 立马打开网页！

以下代码使用了内置的 VIEW 作为 action，通过附加 uri 参数的方式打开网页

```kotlin
btn.setOnClickListener {
    val intent = Intent(Intent.ACTION_VIEW)
    intent.data = Uri.parse("http://www.baidu.com/")
    startActivity(intent)
}
```

<br>

#### 打电话

这次使用 `ACTION_DIAL`

注意，进行 parse 转换时，务必要在电话号码前加上 `前缀“tel:”`

```kotlin
btn.setOnClickListener {
    val intent = Intent(Intent.ACTION_DIAL)
    intent.data = Uri.parse("tel:10086")
    startActivity(intent)
}
```

<br>
