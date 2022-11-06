### 基本使用

> 本文对 CV 工程师极其友好，秉承拿来就用原则，最大可能保持代码简洁与易读

<br>

#### 创建 AlertDialog 模板

直接套用 Kotlin 语法糖，使用 apply 指定参数，最后 show 展示对话框

```kotlin
AlertDialog.Builder(this).apply {
    ...
}.show()
```

<br>

#### yes&no

基本带双选按钮对话框

```kotlin
AlertDialog.Builder(this).apply {
    // 设置基本参数
    setTitle("helloworld")
    setMessage("goodbye")
    setIcon(R.drawable.ic_launcher_background)

    // 参数一为按钮显示文本，参数二为点击事件
    setPositiveButton("yes", DialogInterface.OnClickListener { dialog, which ->
        Toast.makeText(this@MainActivity, "yes!!!", Toast.LENGTH_SHORT).show()
    })

    // 如果不想要点击事件，直接传入null即可
    setNegativeButton("no", null)
}.show()
```

<br>

#### 带可点击简单列表

使用 setItems 创建一个列表对话框，每一个列表项都注册了点击事件

> 一山不容二虎，setMessage 和 setItems 不可以同时出现！！！

```kotlin
val str: Array<CharSequence> = arrayOf("tom", "jack", "lily")
AlertDialog.Builder(this).apply {
    setTitle("helloworld")
    setItems(str, DialogInterface.OnClickListener { dialog, which ->
        println(which)
    })
}.show()
```

<br>

#### 单选与多选列表

setMultiChoiceItems 多选列表：参数一为表项名，参数二为是否选中，参数三为对每个表项的监听事件  
请注意每个参数的数据类型！！！

```kotlin
val name = arrayOf("北京", "南京", "海口")
val checked = booleanArrayOf(false, false, true)
AlertDialog.Builder(this).apply {
    setTitle("helloworld")
    setMultiChoiceItems(
        name,
        checked,
        DialogInterface.OnMultiChoiceClickListener { dialog, which, isChecked ->
            null
        })
    setNeutralButton("确认", DialogInterface.OnClickListener { dialog, which ->
        null
    })
}.show()
```

<br>

> setSingleChoiceItems 注册单选列表，这玩意和多选列表差不多，将就着摸索着用把！！！

<br>

#### 适配器
