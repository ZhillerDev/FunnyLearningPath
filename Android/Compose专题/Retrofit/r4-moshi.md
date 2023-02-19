### moshi 概念与简述

moshi 可以高效的解决 gson 面对反序列化时的问题，且能与 kotlin 高度融合

当数据类中有一个及以上的属性未设置默认值（即无初始化），则 gson 进行反序列化时会报错，而 moshi 会进行检测并保证内容正确输出

<br>

### moshi-kotlin

安装依赖：`implementation "com.squareup.moshi:moshi-kotlin:1.13.0"`

<br>

#### 简单 JSON 与数据类之间转换

定义简单数据类 User，保证存在默认值

```kotlin
data class User(
    val name: String = "jack",
    val age: Int = 100
)
```

<br>

这是展示如何使用 moshi 把数据类实例转换成 JSON 的形式，当然也包括反过来把 JSON 转换成数据类实例的完整流程

首先使用内置构建器构建 moshi，并为 moshi 实例指定一个适配器（所谓适配器即为一个数据类，可以看做解析模板，JSON 和对象实例之间的转换都是基于该适配器的！）

灵活使用 `toJson` 以及 `fromJson` 进行二者之间转换

> `addLast(KotlinJsonAdapterFactory())` 表示使用 kotlin 反射方式构建 moshi

```kotlin
fun MoshiInit() {
    // 固定格式，构建moshi
    val moshi = Moshi.Builder()
        .addLast(KotlinJsonAdapterFactory())
        .build()

    // 指定适配器，相当于配置解析的模板
    // 接收一个数据类作为适配器
    val jsonAdapter = moshi.adapter(User::class.java)

    // 一段JSON，我们将用其转换成数据类实例并输出
    val jsonString = """
        {
            "name":"lucy",
            "age":10086
        }
    """.trimIndent()

    // 实例转JSON
    Log.d("moshi", jsonAdapter.toJson(User("tom", 123)))
    // JSON转实例
    Log.d("moshi", jsonAdapter.fromJson(jsonString).toString())
}
```

<br>

#### 带泛型的参数解析

对于一部分数据类，其可能会使用泛型，并作为某项属性的类型

譬如给出如下带泛型的数据类 SuperUser

```kotlin
data class SuperUser<T>(
    val habit: String,
    val other: T
)
```

同时提供一个可作为泛型使用到的基础数据类（我们上一节写过的）

```kotlin
data class User(
    val name: String = "jack",
    val age: Int = 100
)
```

<br>

对带泛型的数据类实例执行转换为 JSON 的操作

主要可以分为这几步：

1. 构建需要解析的实例
2. 创建 parameterizedType，其中包含实例对象所使用到的全部数据类文件！
3. 根据 parameterizedType 创建对应的 adapter
4. 通过该 adapter 解析数据

```kotlin
fun MoshiInit() {
    // 照抄！可以直接使用的构建moshi
    val moshi = Moshi.Builder()
        .addLast(KotlinJsonAdapterFactory())
        .build()

    // 创建基础实例
    val singleUser = User("tom", 12)
    // 指定泛型为User，并设置第二个参数为实例singleUser
    val superUser = SuperUser<User>("game", singleUser)

    // 创建自定义parameterizedType
    // 我们仅需把数据类实例所用到的全部文件统统写进来，moshi会自动解析的！
    val parameterizedType = Types.newParameterizedType(
        SuperUser::class.java,
        User::class.java
    )

    // 根据parameterizedType，创建指定数据类型的adapter
    val jsonAdapter = moshi.adapter<SuperUser<User>>(parameterizedType)

    // 解析数据类实例并立即输出
    println(jsonAdapter.toJson(superUser))
}
```

<br>

### converter-moshi

> converter-moshi 为适配 retrofit 的 moshi 依赖，能快速实现 JSON 转换

添加依赖：`implementation "com.squareup.retrofit2:converter-moshi:2.9.0"`

<br>

#### 格式化数据类

引入 moshi 转换器后，我们可以为数据类添加对应的注解，实现自动生成适配器以及变量与 JSON 属性之间一一对应的关系

`@JsonClass` 常用语适配器自动生成开关，一般我们都会开启适配器生成的！

`@Json` 设置 JSON 文件中对应属性的名字，他可以和数据类中属性的名字不一致

```kotlin
@JsonClass(generateAdapter = true)
data class Person(
    @Json(name = "age")
    val Age: Int,
    @Json(name = "name")
    val Name: String
)
```

<br>

#### kotlin from json 插件

为避免大量重复性操作，建议直接在 androidstudio 内找到并下载插件 `kotlin from json`

这款插件可以通过你给予的 JSON 文本，自动分析并生成对应的实体类（甚至嵌套多层都可以直接生成）

如果需要生成 moshi 风格的实体类，我们需要在生成器面板的左下角点击 advanced，并选择生成 moshi 风格文件即可

<br>

####
