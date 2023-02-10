### ViewModel 原生支持 kotlin 案例

#### 基本案例

`viewmodel` 是为了保存当当前 activity 切出或者销毁时，如何保存数据，以便下一次创建新的 activity 时进行调用

首先引入 lifecycle 依赖  
`implementation 'androidx.lifecycle:lifecycle-extensions:2.2.0'`

<br>

之后，编写一个类继承 viewmodel，在这里配置数据的存取

我们需要实现：activity 调用该 vm，设置需要获取的水果序号，然后按照序号取出水果

> 代码清单 QuizViewModel.kt

```kotlin
package com.zhiyiyi.listviewdemo.model

import android.util.Log
import androidx.lifecycle.ViewModel

class QuizViewModel : ViewModel() {
    // 初始化代码块，用于提示当前vm被调用
    init {
        Log.d("vm", "启动" + this.javaClass)
    }

    // 当当前的vm被销毁之前需要执行的方法
    override fun onCleared() {
        super.onCleared()
    }

    // 当前被选中的水果序号
    var currentIndex = 0

    // 简写get方法，获取指定序号的水果名称
    val getFruit: String get() = fruit[currentIndex]

    // 在这里存储水果数据
    private val fruit = listOf(
        "apple", "pineapple", "pear", "orange", "banana"
    )
}
```

<br>

viewmodel 全局仅存在一个实例，故配合懒加载，可以使用 val 定义 viewmodel 常量而非使用 var

故这里使用 `by lazy` 函数进行懒加载操作

因版本迭代，原 `ViewModelProviders.of(this)` 方法已被废弃，请使用以下代码中规定的新方法

> 代码清单 MainActivity.kt

```kotlin
package com.zhiyiyi.listviewdemo


import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.lifecycle.ViewModelProvider
import com.zhiyiyi.listviewdemo.model.QuizViewModel
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity() {

    private val quizVM: QuizViewModel by lazy {
        ViewModelProvider(this).get(QuizViewModel::class.java)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val quizVM = ViewModelProvider(this).get(QuizViewModel::class.java)

        btn_submit.setOnClickListener {
            val index = et_username.text.toString().toInt()
            quizVM.currentIndex = index
            et_username.setText(quizVM.getFruit)
        }
    }
}
```

<br>

#### bundle 存储

> 当按下返回键并退出 activity 的时候，实际上该 activity 已经被销毁了； 然而直接点击主屏幕按键，只会挂起该 activity，并不会销毁它

`oncreate` 函数中的 `savedInstanceState: Bundle?` 参数还记得吗？  
其中的 bundle 可看做是一个全局数据库，当 activity 被销毁后他会依然存在  
利用此特性我们可以进行数据的恢复操作

因 activity 被销毁后，会调用 onSaveInstanceState 方法，而我们恰好可以为该方法传入值并存储他，当下一次 activity 被创建的时候，就可以直接取出我们存储的值

重写 onSaveInstanceState 方法，在 activity 被销毁前存储变量

```kotlin
override fun onSaveInstanceState(outState: Bundle) {
    super.onSaveInstanceState(outState)
    outState.putInt("index", quizVM.currentIndex)
    Log.d("vm", quizVM.currentIndex.toString())
}
```

<br>

在 oncreate 方法中直接从 bundle 中取出值

```kotlin
override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    setContentView(R.layout.activity_main)

    val quizVM = ViewModelProvider(this).get(QuizViewModel::class.java)

    // 从bundle中取出值
    val currentIndex = savedInstanceState?.getInt("index", 0) ?: 0
    quizVM.currentIndex = currentIndex
    et_username.setText(currentIndex.toString())

    ...
}
```
