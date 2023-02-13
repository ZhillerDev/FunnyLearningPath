### onBackPressed()

这是 activity 中自带的拦截返回的方法；

每当我们按下底部导航的返回键时，就会调用该事件，我们可以通过重写它来阻止 activity 的销毁

譬如下方代码，我们直接把 super 注释掉，那么点击返回键时就不会有任何反应，因为我们并没有针对性的进行响应的事件处理

```kotlin
override fun onBackPressed() {
    // super.onBackPressed()
}
```

<br>

### onBackPressedDispatcher

fragment 没有提供 onBackPressed 方法，故大多数情况下使用 android 推荐的 onBackPressedDispatcher 来进行返回键的拦截操作；

接下来是一个拦截返回并弹窗，询问是否退出的一个小案例；  
为便于大家理解，故直接使用 activity 而非 fragment；

首先使用属性委托 lazy，即懒加载的方式定义一个 onBackPressedDispatcher 变量

```kotlin
class NoteActivity : AppCompatActivity() {

    private val dispatcher by lazy {
        NoteActivity().onBackPressedDispatcher
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        ...
    }
}
```

<br>

接着在 `oncreate` 方法中注册 `onBackPressedDispatcher`；  
注册的方式即添加一个回调函数用处处理拦截时的动作，使用 `addCallback`；

`addCallback` 有两个参数，第一个为上下文，第二个为 `OnBackPressedCallback` 函数方法（下面的代码使用了 lambda 的形式提高效率）

OnBackPressedCallback 函数接收一个参数，我们一般都填 true；  
它还需要实现一个方法 `handleOnBackPressed` ，这里面就是处理对应实践的地方了

<br>

在 handleOnBackPressed 方法内：

1. 如果想阻止返回事件，那就什么都不做
2. 如果想允许返回事件，那就使用 finish，结束掉本 activity

```kotlin
override fun onCreate(savedInstanceState: Bundle?) {
    ...

    dispatcher.addCallback(
        this,
        object : OnBackPressedCallback(true) {
            override fun handleOnBackPressed() {
                // 构造一个AlertDialog，具体怎么用不细说
                AlertDialog.Builder(this@NoteActivity).apply {
                    setTitle("Oops！")
                    setMessage("请写上一个标题奥，不然咱没法保存")
                    setIcon(R.drawable.ic_info)

                    // 点击了确认按钮，直接使用finish销毁本activity
                    setPositiveButton("直接退出") { dialog, which ->
                        finish()
                    }

                    // 点了否定，就啥都不做，即不处理返回事件，那自然就不会销毁了
                    setNegativeButton("我再想想", null)
                }.show()
            }
        }
    )
}
```

<br>

最后，在 onBackPressed 中触发 onBackPressedDispatcher （因为返回键点击事件都由 onBackPressed 所拦截，onBackPressedDispatcher 仅仅是充当了一个助手的作用）

注意删掉或者注释原来的的 super，否则你点返回键就直接退出了，那就前功尽弃了！！！

```kotlin
override fun onBackPressed() {
    dispatcher.onBackPressed()
}
```

<br>

> 全部代码展示：

```kotlin
package com.zhiyiyi.sqlitedemo

import android.content.DialogInterface
import android.database.sqlite.SQLiteDatabase
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.KeyEvent
import android.view.MotionEvent
import androidx.activity.OnBackPressedCallback
import androidx.appcompat.app.AlertDialog
import com.zhiyiyi.sqlitedemo.databinding.ActivityNoteBinding
import com.zhiyiyi.sqlitedemo.utils.DBHelper

class NoteActivity : AppCompatActivity() {

    private lateinit var binding: ActivityNoteBinding

    // 懒加载
    private val dispatcher by lazy {
        NoteActivity().onBackPressedDispatcher
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityNoteBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // 注册回调事件
        dispatcher.addCallback(
            this,
            object : OnBackPressedCallback(true) {
                override fun handleOnBackPressed() {
                    AlertDialog.Builder(this@NoteActivity).apply {
                        setTitle("Oops！")
                        setMessage("请写上一个标题奥，不然咱没法保存")
                        setIcon(R.drawable.ic_info)
                        setPositiveButton("直接退出") { dialog, which ->
                            finish()
                        }
                        setNegativeButton("我再想想", null)
                    }.show()
                }
            }
        )
    }

    // 检测返回键点击事件
    override fun onBackPressed() {
        dispatcher.onBackPressed()
    }
}
```

<br>
