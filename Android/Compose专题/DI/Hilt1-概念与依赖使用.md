### 概念

#### @HiltAndroidApp

任何 hilt 项目都必须存在一个带@HiltAndroidApp 注解的 Application 类  
此类将作为应用级依赖管理容器存在

初始化时我们仅需添加该注解即可，无需在类内填写任何代码，hilt 会自动生成

```kotlin
@HiltAndroidApp
class HiltDemoApplication:Application() {
}
```

除此之外，还需要在 androidmanifest.xml 的 application 元素下添加对应属性

```xml
<application
    android:name=".HiltDemoApplication">
</application>
```

<br>

#### @AndroidEntryPoint

为类开启成员注入模式，该注解可以适配以下类类型

1. Activity
2. Fragment
3. View
4. Service
5. BroadcastReceiver

<br>

对于非 compose 应用，若将 fragment 注入，则该 fragment 必须不能保存状态，否则会造成内存消耗；

对于 compose 应用，由于没有 fragment，所以一般的，我们仅需对 `MainActivity` 添加该注解即可；

```kotlin
// compose风格的注解，无需使用@Inject注入
@AndroidEntryPoint
class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            ...
        }
    }
}
```

<br>

#### @Inject

> @Inject 注解的变量或者对象都不可以是 private 私有的！

为任意一个类添加@Inject 注解，可以将其注入到 application 中去  
且如要使用被注解的类，仍然要添加@Inject 注解才可以引用

```java
// @Inject将本repository注入到application中去
class CharacterRepo @Inject constructor(
    private val characterApi: CharacterApi
) {
    ...
}
```

调用已注入对象的格式为：

```kotlin
@Inject
lateinit var repo: CharacterRepo
```

<br>

#### scope

只有对类或者方法添加 scope 注解，他们才会被标记为 scope 类型，其余的均会视为非 scope 类型

非 scope 类型：每次都会实例化；  
scope 类型：全局仅实例化一次，后续调用均为同一个实例；

```kotlin
// 仅注入，非scope类型
class UnscopedBinding @Inject constructor() {
}

// 注入且属于scope类型
@FragmentScoped
class ScopedBinding @Inject constructor() {
}
```

> 不要轻易地对任何类或者方法设置为 scope，则可能会消耗较大运行时内存，请视情况而定

<br>

#### @Module

原理：定义一个 hilt 模块，在模块内进行各个对象的实例化操作，与此同时指定的装配位置表示该模块的作用域

可以把添加了@Provides 注释的方法看成延后初始化，即这是一个完整的工作流程：

1. 在一个 hilt 模块内使用@Provides 实例化对象 A
2. 在任意注入类 B 内使用@Inject 定义一个延后初始化对象 A
3. 实例化过程在 hilt 模块内执行
4. 注入类 B 内直接可以调用延后初始化对象 A，而无需在自身进行初始化
5. (说白了，就是将初始化的流程交予 hilt 模块执行，在指定作用域内只需要延后初始化并调用就行了)

`@Module` 定义一个 hilt 模块，模块可以有多个，用来告知 hilt 应该如何提供实例

`@InstallIn` 定义模块装配位置，每个模块都必须存在此注解

`@Provides` 相当于模块专属的@Inject 注解，并且它不受 private 的限制，他标注的方法存在固定格式，请看下方代码注释

```kotlin
@Module
@InstallIn(ActivityComponent::class)
object HiltDemoAppModule {

    // @Provides注解的固定方法格式
    // 方法名：provider+欲实例化的类名
    // 返回值：欲实例化的类名
    // 方法体：return 实例
    @Provides
    fun providerHiltTest():HiltTest{
        return HiltTest("tom")
    }

}
```

<br>

常见装配位置：

| 装配名             | 装配位置    |
| ------------------ | ----------- |
| SingletonComponent | Application |
| ViewModelComponent | ViewModel   |
| ActivityComponent  | Activity    |
| FragmentComponent  | Fragment    |

<br>

#### @Binds

之前说过，@Provides 是用来注入方法的，而这个@Binds 是用来注入接口的

首先定义接口及其对应的实现类

```kotlin
// 定义简单接口
interface HiltTestInterface {
    fun getUserName():String
}

// 实现类记得注入！
class HiltTestInterfaceImpl @Inject constructor():HiltTestInterface{
    override fun getUserName(): String {
        return "jack"
    }
}
```

<br>

紧接着新建一个 hilt 模块，该模块定义为抽象类而不是单例

其中的注入方式全部使用 @Binds 注解

```kotlin
@Module
@InstallIn(ActivityComponent::class)
abstract class HiltDemoInterfaceAppModule {
    @Binds
    abstract fun providerHiltTestInterface(impl: HiltTestInterfaceImpl):HiltTestInterface
}
```

> 使用方式和@Providers 一样，都是延后初始化然后调用

<br>

#### 单例模块

@InstallIn 指定作用域为 SingletonComponent

之后为每一个注入的方法都添加@Singleton 注解即可

这在实例化 retrfit 的 builder 时很实用！

```kotlin
@Module
@InstallIn(SingletonComponent::class)
object HiltSingletonAppModule {
    @Provides
    @Singleton
    fun providerHiltTest():HiltTest{
        return HiltTest("tom")
    }
}
```

<br>

#### 配合 viewmodel

hilt 为 viewmodel 提供了一个专属的注解：@HiltViewModel

```kotlin
@HiltViewModel
class HomeViewModel @Inject constructor() : ViewModel() {
    ...
}
```

调用该 viewmodel 时按照常规方法来就可以，无需做特殊处理  
`val homeViewModel = viewModel(modelClass = HomeViewModel::class.java)`

<br>

### 添加依赖

#### 兼容性校验

根据多次测试，发现 hilt 版本过高会直接与 lifecycle 产生冲突，目前得出以下完善的解决方案：

`hilt` 以及 `hilt-plugin` 均使用 2.38.1 版本  
`lifecycle` 除了 `lifecycle-viewmode` 使用 2.4.0 之外，其余依赖可以使用 2.5.1 及以下版本

> 不要使用 hilt2.45，运行直接冲突报错

<br>

#### 依赖安装主流程

hilt 除了添加其本体以及 dagger 之外，还需要配置 hilt-gradle-plugin，这个比较麻烦  
配置 hilt 插件是因为这可以加快我们的注解开发速度

<br>

首先在 `build.gradle(module:app)` 中的 plugin 添加 hilt 插件

```groovy
plugins {
    id 'com.android.application'
    id 'org.jetbrains.kotlin.android'

    // 添加hilt插件
    id 'dagger.hilt.android.plugin'
}

// 注意还要开启kapt
apply plugin: 'kotlin-kapt'
```

<br>

之后于 `settings.gradle` 中识别并添加指定依赖

```groovy
pluginManagement {
    repositories {
        gradlePluginPortal()
        google()
        mavenCentral()
    }

    // 识别module中的plugins，并添加指定版本的依赖
    resolutionStrategy{
        eachPlugin{
            if (requested.id.id == 'dagger.hilt.android.plugin'){
                useModule("com.google.dagger:hilt-android-gradle-plugin:2.38.1")
            }
        }
    }
}
```

<br>

最后在 build.gradle(module:app) 中添加所有依赖即可

```groovy
dependencies {
    // dagger -hilt
    implementation "com.google.dagger:hilt-android:2.38.1"
    kapt "com.google.dagger:hilt-android-compiler:2.38.1"
    kapt "androidx.hilt:hilt-compiler:1.0.0"
}
```

<br>

> 注：如果你用的是 JDK8，你还需要在 build.gradle(module:app)添加如下内容

```groovy
compileOptions {
    sourceCompatibility JavaVersion.VERSION_1_8
    targetCompatibility JavaVersion.VERSION_1_8
}
kotlinOptions {
    jvmTarget = '1.8'
}
```

<br>
