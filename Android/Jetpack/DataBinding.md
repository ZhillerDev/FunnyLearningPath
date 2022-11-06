### 基本数据绑定例子

> databinding 可以用于单向和双向数据绑定，但由于整体使用结果过于复杂，而被 livedata 和 viewbind 取代

<br>

#### build.gradle 设置

databinding 默认捆绑，是一个年纪挺大的库了，所以我们仅需要在 build》gradle 注册一下就好了；

因为使用 databinding 的同时我们还需要使用 viewbinding，所以我们一起注册他俩

```js
android {
    buildFeatures {
        dataBinding true
        viewBinding true
    }

    ...
}
```

<br>

#### 设置数据类

创建单例类 Test，里面存放我们自建的一个数据类型 Fruit

```kotlin
object Test {
    data class Fruit(val name: String, val price: String)
}
```

<br>

#### databinding 专属布局

**data 布局的要素**

1. 最外层必须是 layout 标签
2. data 是存放数据的地方
3. data 外是正常的布局类型

<br>

**data 域内容介绍**

1. import 用于导入外部数据类型，譬如我们这里导入了数据类 Fruit
2. variable 存放单个数据，可以设置其名称 name 和数据类型 type

> 内置数据类型不需要导入，直接使用即可（譬如 String 和 Int 等等）

<br>

**调用 data 域中数据**  
如 `android:text` 中我们设置其属性值为 `@{fruit.name}`；  
符号 @{} 里面可以使用简单的 kotlin 语法，或者调用我们在 variable 标签内保存的单个数据！

```xml
<!-- 光标处于?xml内，按alt+enter，即可按照指示转换为data视图 -->
<?xml version="1.0" encoding="utf-8"?>
<layout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools">

    <data>

        <import type="com.zhiyiyi.bio.data.Test.Fruit" />

        <variable
            name="fruit"
            type="Fruit" />
    </data>

    <LinearLayout
        android:id="@+id/linearLayout"
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:orientation="vertical"
        tools:context=".MainActivity"
        android:layout_margin="10dp">

        <TextView
            android:id="@+id/tv1"
            android:layout_width="match_parent"
            android:layout_height="wrap_content"
            android:text="@{fruit.name}" />

        <TextView
            android:id="@+id/tv2"
            android:layout_width="match_parent"
            android:layout_height="wrap_content"
            android:text="@{fruit.price}" />

    </LinearLayout>
</layout>
```

<br>

#### 绑定 data

回到主 activity 的类中，添加如下代码

1. 先延后初始化 ActivityMainBinding，之后在 onCreate 方法中实例化
2. DataBindingUtil，使用该工具类进行 databinding
3. binding.fruit 绑定过后就可以直接对布局文件的 data 标签内数据进行赋值与改动

> 某些情况下 AS 自动补全功能会失效，也就是说你需要手动导入 androidx.databinding.DataBindingUtil 这个类才可以使用 DataBindingUtil，不然你会一直卡在这里写不下代码！！！

```kotlin
package com.zhiyiyi.bio

import android.database.DatabaseUtils
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.databinding.DataBindingUtil
import com.zhiyiyi.bio.data.Test
import com.zhiyiyi.bio.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        binding = DataBindingUtil.setContentView(
            this, R.layout.activity_main
        )

        binding.fruit = Test.Fruit(name = "pear", price = "100")
    }
}
```

<br>

### 进阶使用

#### 使用默认值

default 跟着的值不需要引号
`android:text="@{books.pages,default=330}"`

双问号的作用和三目运算符一致  
`android:text="@{books.pages ?? book.defaultPages}"`

<br>

#### 数据类型转换

databinding 不会进行自动数据转换，所以需要字节写！
`android:text="@{String.valueOf(book.pages)}"`

<br>

#### import 别名

如果包有重名，那么需要使用 alias 取名

```xml
<data>
      <import type="com.xxx.a.Book" alias="aBook"/>
      <import type="com.xxx.B.Book" alias="bBook"/>
      ...
</data>
```

<br>

#### 绑定点击事件

这里面的 user 是一个 variable 数据

```xml
<Button
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:onClick="@{()->user.testOnclick}"
    />
```

<br>

### 观察者

#### BaseObservable
