### WindowInsetsCompat

> 它用来控制页面全局状态

<br>

### 隐藏状态栏并设置为透明

```kotlin
private fun transparentStatusBar() {
    val controller = ViewCompat.getWindowInsetsController(window.decorView)
    controller?.hide(WindowInsetsCompat.Type.statusBars())
    window.statusBarColor = Color.TRANSPARENT
}
```

<br>
