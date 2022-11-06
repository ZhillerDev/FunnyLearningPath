### 基本案例

> 本案例讲述 CALL_PHONE（拨打电话）权限的使用

#### manifest 注册权限

```xml
<uses-permission android:name="android.permission.CALL_PHONE"/>
```

<br>

#### 设置拨打电话按钮

进入主 activity 的布局文件内，添加一个按钮

```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="vertical"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:layout_margin="10dp"
    >

    <Button
        android:id="@+id/makeCall"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:text="Make Call" />

</LinearLayout>
```

<br>

#### 配置权限

编写拨打电话的方法：

使用 intent 拨打电话

```kotlin
private fun call() {
    try {
        val intent = Intent(Intent.ACTION_CALL)
        intent.data = Uri.parse("tel:10086")
        startActivity(intent)
    } catch (e: SecurityException) {
        e.printStackTrace()
    }
}
```

<br>

注册权限申请监听事件

1. requestCode 权限请求码，该码在 requestPermissions 方法中被设置
2. grantResults 存储授权结果，通过对其判断来获得用户是否允许该权限

```kotlin
override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
    super.onRequestPermissionsResult(requestCode, permissions, grantResults)
    when (requestCode) {
        1 -> {
            if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                call()
            } else {
                Toast.makeText(this, "You denied the permission", Toast.LENGTH_SHORT).show()
            }
        }
    }
}
```

<br>

配置按钮点击事件

1. ContextCompat.checkSelfPermission 检测权限是否已被赋予
2. PackageManager.PERMISSION_GRANTED 相当于该权限已被赋予的标志
3. ActivityCompat.requestPermissions 如果没有获得权限，那么就使用此方法请求权限

```kotlin
override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    setContentView(R.layout.activity_main)

    makeCall.setOnClickListener {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CALL_PHONE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.CALL_PHONE), 1)
        } else {
            call()
        }
    }
}
```

<br>
