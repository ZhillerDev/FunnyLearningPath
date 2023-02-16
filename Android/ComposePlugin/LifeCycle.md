### 开头准备

目前最新版本为 2.5.0，可以自己去官网看可用版本：https://developer.android.com/jetpack/androidx/releases/lifecycle?hl=zh-cn

依赖选择

```kotlin
dependencies {
    // ViewModel
    implementation("androidx.lifecycle:lifecycle-viewmodel-ktx:2.5.1")
    // ViewModel utilities for Compose
    implementation("androidx.lifecycle:lifecycle-viewmodel-compose:2.4.0")
    // LiveData
    implementation("androidx.lifecycle:lifecycle-livedata-ktx:2.5.1")
    // Lifecycles only (without ViewModel or LiveData)
    implementation("androidx.lifecycle:lifecycle-runtime-ktx:2.5.1")
}
```

<br>

### 概念解释

众所周知，lifecycle 顾名思义即监听一个 activity 或者 fragment 的完整声明周期，通过钩子函数在对应周期执行对应函数

可以通过继承以下任意一个接口实现自定义 lifecycle

1. `DefaultLifecycleObserver` ：有完备的钩子函数，可以直接重写并进行监听
2. `LifecycleEventObserver` ：只能通过 onStateChanged 监听当前生命周期状态并进行对应处理

<br>

### 监听一个 Activity

新建文件 MyObs.kt 为我们的自定义 lifecycle 文件  
你可以选择性重写你想要用到的钩子

```kotlin
package com.zhiyiyi.lifecycletest.observer

import android.content.ContentValues.TAG
import android.util.Log
import androidx.lifecycle.DefaultLifecycleObserver
import androidx.lifecycle.LifecycleOwner

// 继承DefaultLifecycleObserver
class MyObs:DefaultLifecycleObserver {

    // 给每一个钩子添加log，检测activity运行的不同时期
    override fun onCreate(owner: LifecycleOwner) {
        super.onCreate(owner)
        Log.i(TAG, "onCreate")
    }

    override fun onStart(owner: LifecycleOwner) {
        super.onStart(owner)
        Log.i(TAG, "onStart: ")
    }

    override fun onResume(owner: LifecycleOwner) {
        super.onResume(owner)
        Log.i(TAG, "onResume: ")
    }

    override fun onPause(owner: LifecycleOwner) {
        super.onPause(owner)
        Log.i(TAG, "onPause: ")
    }

    override fun onStop(owner: LifecycleOwner) {
        super.onStop(owner)
        Log.i(TAG, "onStop: ")
    }

    override fun onDestroy(owner: LifecycleOwner) {
        super.onDestroy(owner)
        Log.i(TAG, "onDestroy: ")
    }
}
```

<br>

在 activity 中的 onCreate 方法中直接进行监听

```kotlin
class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // 根据类初始化监听器，直接对该activity的完整生命周期进行监听
        lifecycle.addObserver(MyObs())

        setContent {
            ...
        }
    }
}
```

<br>
