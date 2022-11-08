### 基本数据绑定例子

> databinding 可以用于单向和双向数据绑定，但由于整体使用结果过于复杂，而被 livedata 和 viewbind 取代

<br>

#### build.gradle 设置

databinding 默认捆绑，是一个年龄挺大的库了，所以我们仅需要在 build》gradle 注册一下就好了；

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

### 与 recyclerview 结合

#### 编写单个列表项的 xml

既然要使用 recyclerview，那么必然要添加 item 的 xml，新建布局文件 `item_text_data_rv.xml`

在此使用 databinding 的基本布局 layout

这里传入了一个变量，将其命名为 basicBean，这是我们编写好的一个实体类

```xml
<?xml version="1.0" encoding="utf-8"?>
<layout xmlns:android="http://schemas.android.com/apk/res/android">

    <data>
        <!-- 引入实体类 -->
        <variable
            name="basicBean"
            type="com.zhiyiyi.login.bean.BasicBean" />
    </data>

    <!-- 简单布局，这里不做过多介绍 -->
    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_marginBottom="1dp"
        android:background="@color/white"
        android:foreground="?attr/selectableItemBackground"
        android:orientation="vertical"
        android:padding="16dp">

        <TextView
            android:id="@+id/tv_title"
            android:layout_width="match_parent"
            android:layout_height="wrap_content"
            android:text="@{basicBean.title}"
            android:textColor="@color/black" />

        <TextView
            android:id="@+id/tv_content"
            android:layout_width="match_parent"
            android:layout_height="wrap_content"
            android:text="@{basicBean.content}"
            android:textColor="@color/black" />
    </LinearLayout>
</layout>

```

<br>

#### 编写适配器

为了便于大家理解，这里进行分解

```kotlin
// 适配器接收一个实体类列表作为渲染数据使用
// 适配器泛型为ViewHolder，后续我们会创建该内部类
class StringDataBindingAdapter(private val lists: List<BasicBean>) :
RecyclerView.Adapter<StringDataBindingAdapter.ViewHolder>() {
    ...
}
```

<br>

创建内部类 ViewHolder

ItemTextDataRvBinding 这是我们在创建 databinding 后自动为 xml 布局文件映射的一个类；  
该映射类的格式是：xml 全大写文件名+Binging  
（譬如我们刚刚添加的布局名称为 item_text_data_rv.xml，那么这里映射的类就是 ItemTextDataRvBinding）

```kotlin
// 传入布局映射类
// ViewHolder构造函数使用映射类的根节点作为参数
class ViewHolder(itemTextDataRvBinding: ItemTextDataRvBinding) :
    RecyclerView.ViewHolder(itemTextDataRvBinding.getRoot()) {
    var binding: ItemTextDataRvBinding

    // 初始化代码块对binding进行初始化，使其通过映射类绑定布局文件
    init {
        binding = itemTextDataRvBinding
    }
}
```

<br>

老生常谈了，为 viewholder 传入一个实例化的映射类

```kotlin
override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
    val binding: ItemTextDataRvBinding =
        ItemTextDataRvBinding.inflate(LayoutInflater.from(parent.context), parent, false)
    return ViewHolder(binding)
}
```

<br>

viewholder 绑定阶段

我们可以吧下方的操作理解为注入:

1. setBasicBean，根据我们在布局文件中添加的变量 basicBean 自动生成的一个 setter 方法，便于为变量注入数据
2. executePendingBindings，因为注入了数据，导致 xml 发生改变，就必须要提交来刷新页面

```kotlin
override fun onBindViewHolder(holder: ViewHolder, position: Int) {
    // 再次进行databinding操作
    val binding: ItemTextDataRvBinding? = DataBindingUtil.getBinding(holder.binding.getRoot())
    // 为变量注入数据
    if (binding != null) {
        binding.setBasicBean(lists[position])
        binding.executePendingBindings()
    }
}
```

<br>

完整适配器代码块

```kotlin
import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.databinding.DataBindingUtil
import androidx.recyclerview.widget.RecyclerView
import com.zhiyiyi.login.bean.BasicBean
import com.zhiyiyi.login.databinding.ItemTextDataRvBinding


class StringDataBindingAdapter(private val lists: List<BasicBean>) :
    RecyclerView.Adapter<StringDataBindingAdapter.ViewHolder>() {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val binding: ItemTextDataRvBinding =
            ItemTextDataRvBinding.inflate(LayoutInflater.from(parent.context), parent, false)
        return ViewHolder(binding)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val binding: ItemTextDataRvBinding? = DataBindingUtil.getBinding(holder.binding.getRoot())
        if (binding != null) {
            binding.setBasicBean(lists[position])
            binding.executePendingBindings()
        }
    }

    override fun getItemCount(): Int {
        return lists.size
    }

    class ViewHolder(itemTextDataRvBinding: ItemTextDataRvBinding) :
        RecyclerView.ViewHolder(itemTextDataRvBinding.getRoot()) {
        var binding: ItemTextDataRvBinding

        init {
            binding = itemTextDataRvBinding
        }
    }
}
```

<br>
