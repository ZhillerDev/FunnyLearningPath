### 标准项目中使用 Viewbinding

> 因为 kotlin-android-extensions 被废弃了，且官方建议我们使用 viewbinding

使用方式亦非常简单；

<br>

#### 开启 viewbinding

首先在 build.gradle 中开启 viewbinding 使用

```js
android {
    viewBinding {
        enable = true
    }
}
```

<br>

#### 布局文件设置

在布局属性中添加 xmlns:tools 这一行，用来配置 viewbinding 相关属性；

千万要注意 viewBindingIgnore 必须设置为 false，这样布局文件才会自动生成一个与之关联的类供我们使用！！！

```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout
    xmlns:tools="http://schemas.android.com/tools"
    ...
    tools:viewBindingIgnore="false"
    >

    <Button
        android:id="@+id/btn"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="button" />

</LinearLayout>
```

<br>

#### 全局使用 viewbinding

> 布局文件生成的类格式为： Activity+类名称

首先设置一延后初始化私有变量 binding，他的类型即为布局文件自动生成的类

然后根据下方代码方法直接进行绑定即可

```kotlin
class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // 绑定后直接使用
        binding = ActivityMainBinding.inflate(layoutInflater)
        binding.btn.setOnClickListener {

        }
    }
}
```

<br>
