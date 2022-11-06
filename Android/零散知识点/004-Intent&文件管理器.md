### Intent 开启文件管理器

必须设置以下三个 intent 属性才可以开启，否则直接闪退！

1. setAction，指定使用打开文件的 aciton
2. addCategory，使用 CATEGORY_OPENABLE
3. setType，过滤器，表示打开的文件管理器默认过滤那些文件

> 过滤格式：  
> image/\* 过滤所有类型图片  
> application/vnd.android.package-archive 仅过滤 apk 文件

```kotlin
val intent = Intent().apply {
    setAction(Intent.ACTION_OPEN_DOCUMENT)
    addCategory(Intent.CATEGORY_OPENABLE)
    setType("application/vnd.android.package-archive")
}
context?.startActivity(intent)
```

<br>
