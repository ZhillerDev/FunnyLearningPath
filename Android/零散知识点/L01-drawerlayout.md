### 基本框架

定义一个抽屉式布局需要两个条件：

1. 最外层 DrawerLayout 标签包裹
2. 里层分为两个部分，一个写主页面，一个写抽屉页面

<br>

分析抽屉布局属性：`layout_gravity="start"`  
他表示将布局重心对齐到父对象的 start（即左端面）

> android 推荐使用 start/end 来表述布局的左右，当然你用旧版的 left/right 也是可以的！

```xml
<?xml version="1.0" encoding="utf-8"?>
<androidx.drawerlayout.widget.DrawerLayout xmlns:android="http://schemas.android.com/apk/res/android"定义
    xmlns:tools="http://schemas.android.com/tools"
    android:id="@+id/drawer_layout"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    tools:context=".MainActivity">

    <!--主页面布局-->
    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:gravity="center">

        <Button
            android:id="@+id/btn_open"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="打开滑动菜单"
            android:textColor="#000"
            android:textSize="18sp" />
    </LinearLayout>

    <!--滑动菜单布局-->
    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:layout_gravity="start"
        android:background="@color/white"
        android:gravity="center">

        <TextView
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="滑动菜单"
            android:textColor="#000"
            android:textSize="18sp" />
    </LinearLayout>

</androidx.drawerlayout.widget.DrawerLayout>
```

<br>

布局设置完毕，来到主 activity，获取按钮以及抽屉布局的组件实例，然后对抽屉布局使用 openDrawer 方法即可弹出抽屉了！

因为我们定义抽屉布局的重心在 start，故需要使用 `GravityCompat.START` 来从左端面弹出

```kotlin
package com.zhiyiyi.login

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import androidx.core.view.GravityCompat
import androidx.drawerlayout.widget.DrawerLayout

class DrawerActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_drawer)

        val btn = findViewById<Button>(R.id.btn_open)
        val drawer = findViewById<DrawerLayout>(R.id.drawer_layout)
        btn.setOnClickListener {
            // 直接点击按钮开启抽屉就完事了
            drawer.openDrawer(GravityCompat.START)
        }
    }
}
```

<br>
