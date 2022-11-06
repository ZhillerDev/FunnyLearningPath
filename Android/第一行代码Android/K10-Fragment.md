### 快速上手 Fragment

#### 概念

fragment 可以用作一个 activity 内部的小分块；

当我们从手机转换到 pad 上时，整体界面会发生变化（比如由单列视图变为双列），此时就需要 fragment 的参与了！

<br>

### 基本示例

> 在本实例中，我们要制作一个双列视图，左右列均为 fragment 构成

<br>

#### 设置左右列布局文件

新建布局文件 `left_frag.xml` 和 `right_frag.xml`

左列布局我们插入一个按钮并居中；

```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical"
    android:layout_width="match_parent"
    android:layout_height="match_parent">

    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:id="@+id/left_btn"
        android:text="左边的按钮"
        android:layout_gravity="center_horizontal"/>

</LinearLayout>
```

右列布局我们则插入一个文本；

```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical"
    android:layout_width="match_parent"
    android:layout_height="match_parent">

    <TextView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:id="@+id/right_text"
        android:text="右边的frag"
        android:layout_gravity="center_horizontal"/>

</LinearLayout>
```

<br>

#### 配置左右布局类

一般的，所有 fragment 都需要一个单独的类来对其页面进行渲染，以及部分事件处理；

创建类 `LeftFrag.kt`

使该类继承 Fragment，并实现方法，渲染 fragment：

这里使用了 inflater 对页面进行注册；

```kotlin
package com.zhiyiyi.listviewdemo

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment

class LeftFrag : Fragment() {
    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.left_frag, container, false)
    }
}
```

<br>

#### 主布局文件注册

我们需要在主 activity 的布局文件中使用这两个 fragment；

直接添加两个 fragment 标签，在 name 属性写上 fragment 布局处理的类即可；

```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent">

    <fragment
        android:id="@+id/leftfrag"
        android:name="com.zhiyiyi.listviewdemo.LeftFrag"
        android:layout_width="0dp"
        android:layout_height="match_parent"
        android:layout_weight="1" />

    <fragment
        android:id="@+id/rightfrag"
        android:name="com.zhiyiyi.listviewdemo.RightFrag"
        android:layout_width="0dp"
        android:layout_height="match_parent"
        android:layout_weight="1" />

</LinearLayout>
```

这边注册完毕后就大功告成了，直接运行看看成果把！

<br>
