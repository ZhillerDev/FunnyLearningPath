### SharedPreference

> 他的使用方法非常简单，不夸张的说，仅需要一个 getSharedPreferences 就可以完成大部分操作

<br>

#### 概念与权限

SharedPreference 存储文件的位置在：  
`data/data/你的工程包名/shared_prefs`

`getSharedPreferences` 的第二个参数需要传入一个操作模式，目前仅剩下 `MODE_PRIVATE` 这一个可选，他表示仅当前 app 可以操作此 SharedPreference

<br>

#### 存储数据

存储三步走：

1. 使用 `getSharedPreferences.edit()` 获取一个 `Editor` 对象
2. 使用诸如 `putString` 方法，按照数据类型并以键值对的形式插入数据
3. 最后使用 `apply()` 保存修改

<br>

方法一：实例化 editor 后按步骤执行

```kotlin
val editor = getSharedPreferences("data",Context.MODE_PRIVATE).edit()
editor.putString("name","jack")
editor.apply()
```

<br>

方法二：直接 lambda 解决，免去 apply

```kotlin
getSharedPreferences("data", Context.MODE_PRIVATE).edit {
    putString("name", "Tom")
    putInt("age", 28)
    putBoolean("married", false)
}
```

<br>

#### 获取数据

这玩意就更简单了，直接 getSharedPreferences 获取存储文件，然后按照 key 拿到 value 就好了

```kotlin
val prefs = getSharedPreferences("data", Context.MODE_PRIVATE)
val name = prefs.getString("name", "")
val age = prefs.getInt("age", 0)
val married = prefs.getBoolean("married", false)
Log.d("MainActivity", "name is $name")
Log.d("MainActivity", "age is $age")
Log.d("MainActivity", "married is $married")
```

<br>

### 简单存储案例

#### 设置存取按钮

在 mainactivity 的布局文件中，我们添加俩按钮，一个存东西，一个取东西；

```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical"
    android:layout_width="match_parent"
    android:layout_height="match_parent" >

    <Button
        android:id="@+id/saveButton"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="Save Data"
        />

    <Button
        android:id="@+id/restoreButton"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="Restore Data"
        />

</LinearLayout>
```

<br>

#### 主代码

写在 MainAcitvity.kt

```kotlin
package com.zhiyiyi.listviewdemo

import android.content.Context
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.core.content.edit
import kotlinx.android.synthetic.main.activity_main.*
import java.io.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        saveButton.setOnClickListener {
            getSharedPreferences("data", Context.MODE_PRIVATE).edit {
                putString("name", "Tom")
                putInt("age", 28)
                putBoolean("married", false)
            }
        }

        restoreButton.setOnClickListener {
            val prefs = getSharedPreferences("data", Context.MODE_PRIVATE)
            val name = prefs.getString("name", "")
            val age = prefs.getInt("age", 0)
            val married = prefs.getBoolean("married", false)
            Log.d("MainActivity", "name is $name")
            Log.d("MainActivity", "age is $age")
            Log.d("MainActivity", "married is $married")
        }
    }
}

```

<br>

### SharedPreference 进阶课
