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

#### modules

### 添加依赖

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
                useModule("com.google.dagger:hilt-android-gradle-plugin:2.45")
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
    implementation "com.google.dagger:hilt-android:2.45"
    kapt "com.google.dagger:hilt-android-compiler:2.45"
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
