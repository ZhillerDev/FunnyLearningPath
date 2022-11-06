### DownloadManager 三大组件介绍

#### DownloadManager

类似于下载队列，管理所有当前正在下载或者等待下载的项目；

他可以维持 HTTP 链接，并且在队列中的下载项目一旦失败，还能自动重新下载！

一般采取如下固定格式创建一个 DownloadManager

```kotlin
downloadManager = context.getSystemService(Context.DOWNLOAD_SERVICE) as DownloadManager
```

<br>

整个下载过程需要添加的权限：

```xml
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
```

<br>

#### DownloadManager.Query

目前还没有研究出什么作用来，他一般和 cursor 连用；

使用 query，获取目前 DownloadManager 中下载内容的所有信息；

> setFilterById，即通过下载项目的 id 来 query（查询） 到对应项目信息

```kotlin
fun getDownloadList(id: Long) {
    // 首选使用DownloadManager.Query()创建query对象
    val query = DownloadManager.Query().setFilterById(id)
    // 然后对downloadManager进行查询，得到cursor对象
    val cursor = downloadManager.query(query)
    // 简单的打印一下cursor对象
    println(cursor)
}
```

<br>

#### DownloadManager.Request

重头戏，使用它可以执行下载操作

Request 接收的第一个参数是一个下载地址的 Uri 形式，我们可以使用 `Uri.parse` 把原下载地址变成 uri 形式的！

> 使用 apply 函数来执行，可以简化代码

```kotlin
// 发起一个request请求，请求的下载地址为downloadUri
// 使用apply函数设置下载的对应参数
var request = DownloadManager.Request(downloadUri).apply {

    // 下载时在通知栏内显示下载进度条
    // 这是固定格式，抄就好了
    setNotificationVisibility(
        DownloadManager.Request.VISIBILITY_VISIBLE
                or DownloadManager.Request.VISIBILITY_VISIBLE_NOTIFY_COMPLETED
    )

    // 设置MIME类型，即设置下载文件的类型
    // 如果下载的是android文件，那么类型应当设置为application/vnd.android.package-archive
    setMimeType(filter)

    // 设置通知栏中下载标题
    setTitle(title)
    // 设置通知栏中下载详细内容介绍
    setDescription(description)

    // 设置下载文件保存在SDCard中的那一个公开目录
    setDestinationInExternalPublicDir(Environment.DIRECTORY_DOWNLOADS, downloadName)
}
```

<br>

### 下载图片小案例

#### 工具类 DownloadUtils.kt

在这个工具类内，我们将编写 download 下载方法，便于我们后续直接复用；

首先添加一个下载管理器，并设置他为延后初始化：

```kotlin
lateinit var downloadManager: DownloadManager
```

<br>

第二步，创建下载方法；  
`title` 和 `description` 分别表示通知栏内下载标题和详细介绍；  
`downloadName` 表示下载好的文件名称  
`downloadUri` 就是下载地址了  
`filter` 即下载文件时使用的过滤器

```kotlin
fun download(
    context: Context,
    title: String = "下载文件",
    description: String = "正在下载，请稍后...",
    downloadName: String,
    downloadUri: Uri,
    filter: String
) {}
```

<br>

第三步，创建一个 request 下载，然后把该 request 塞到下载管理器里面进行管理：

```kotlin
var request = DownloadManager.Request(downloadUri).apply {
    setNotificationVisibility(
        DownloadManager.Request.VISIBILITY_VISIBLE
                or DownloadManager.Request.VISIBILITY_VISIBLE_NOTIFY_COMPLETED
    )
    setMimeType(filter)
    setTitle(title)
    setDescription(description)
    setDestinationInExternalPublicDir(Environment.DIRECTORY_DOWNLOADS, downloadName)
}

downloadManager = context.getSystemService(Context.DOWNLOAD_SERVICE) as DownloadManager
downloadManager.enqueue(request)
```

<br>

第四步，直接在本方法内创建一个监听器，用于监听文件下载完毕事件和通知栏点击事件；

```kotlin
context.registerReceiver(object : BroadcastReceiver() {
    override fun onReceive(context: Context?, intent: Intent?) {
        when (intent?.action) {
            // 定义监听到下载完毕后我们要做的事情
            DownloadManager.ACTION_DOWNLOAD_COMPLETE -> {

                // 取出下载ID的办法
                val id = intent.getLongExtra(DownloadManager.EXTRA_DOWNLOAD_ID, -1)

                // 使用意图打开下载文件管理器，具体代码解释请看我的另一篇文章，这里不做过多赘述
                val intent = Intent().apply {
                    setAction(Intent.ACTION_OPEN_DOCUMENT)
                    addCategory(Intent.CATEGORY_OPENABLE)
                    setType(filter)
                }
                context?.startActivity(intent)
                Toast.makeText(context, "下载完毕", Toast.LENGTH_SHORT).show()
            }
            DownloadManager.ACTION_NOTIFICATION_CLICKED -> {

            }
        }
    }
    // 别忘了这里还有一个IntentFilter作为过滤器使用！！！
}, IntentFilter(DownloadManager.ACTION_DOWNLOAD_COMPLETE))
```

<br>

#### 主类调用下载方法

最后一步，直接在 mainactivity 中进行调用就好啦！

```kotlin
override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    setContentView(R.layout.activity_main)

    // 点击按钮，即可开始下载
    btn.setOnClickListener {
        val path =
            Uri.parse(
                "https://gimg2.baidu.com/image_searc" +
                        "h/src=http%3A%2F%2Fwww.52zzl.co" +
                        "m%2Fuploads%2Fallimg%2F180202%2F4-1P202" +
                        "1U320-53.jpg&refer=http%3A%2F%2Fwww.52zzl.com&a" +
                        "pp=2002&size=f9999,10000&q=a80&n=0&g=0n&" +
                        "fmt=auto?sec=1668251708&t=d98a9" +
                        "6444b2725d59e3654e4f32eca87"
            )

        DownloadUtils.download(
            this,
            "保存您的图片",
            "正在下载图片...",
            "image.png",
            path,
            "image/png"
        )
    }
}
```

<br>

#### 完整代码

> MainActivity.kt

```kotlin
package com.zhiyiyi.databasedemo

import android.app.DownloadManager
import android.content.IntentFilter
import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        btn.setOnClickListener {
            val path =
                Uri.parse(
                    "https://gimg2.baidu.com/image_searc" +
                            "h/src=http%3A%2F%2Fwww.52zzl.co" +
                            "m%2Fuploads%2Fallimg%2F180202%2F4-1P202" +
                            "1U320-53.jpg&refer=http%3A%2F%2Fwww.52zzl.com&a" +
                            "pp=2002&size=f9999,10000&q=a80&n=0&g=0n&" +
                            "fmt=auto?sec=1668251708&t=d98a9" +
                            "6444b2725d59e3654e4f32eca87"
                )

            DownloadUtils.download(
                this,
                "保存您的图片",
                "正在下载图片...",
                "image.png",
                path,
                "image/png"
            )
        }
    }
}
```

<br>

> 工具类 DownloadUtils.kt 代码清单

```kotlin
package com.zhiyiyi.databasedemo

import android.app.DownloadManager
import android.content.BroadcastReceiver
import android.content.ClipDescription
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.database.Cursor
import android.net.Uri
import android.os.Environment
import android.widget.Toast
import androidx.core.content.getSystemService
import java.io.File

object DownloadUtils {

    lateinit var downloadManager: DownloadManager

    fun download(
        context: Context,
        title: String = "下载文件",
        description: String = "正在下载，请稍后...",
        downloadName: String,
        downloadUri: Uri,
        filter: String
    ) {
        println("download")

        var request = DownloadManager.Request(downloadUri).apply {
            setNotificationVisibility(
                DownloadManager.Request.VISIBILITY_VISIBLE
                        or DownloadManager.Request.VISIBILITY_VISIBLE_NOTIFY_COMPLETED
            )
            setMimeType(filter)
            setTitle(title)
            setDescription(description)
            setDestinationInExternalPublicDir(Environment.DIRECTORY_DOWNLOADS, downloadName)
        }

        downloadManager = context.getSystemService(Context.DOWNLOAD_SERVICE) as DownloadManager
        downloadManager.enqueue(request)

        context.registerReceiver(object : BroadcastReceiver() {
            override fun onReceive(context: Context?, intent: Intent?) {
                when (intent?.action) {
                    DownloadManager.ACTION_DOWNLOAD_COMPLETE -> {
                        val id = intent.getLongExtra(DownloadManager.EXTRA_DOWNLOAD_ID, -1)
                        val intent = Intent().apply {
                            setAction(Intent.ACTION_OPEN_DOCUMENT)
                            addCategory(Intent.CATEGORY_OPENABLE)
                            setType(filter)
                        }
                        getDownloadList(id)
                        context?.startActivity(intent)
                        Toast.makeText(context, "下载完毕", Toast.LENGTH_SHORT).show()
                    }
                    DownloadManager.ACTION_NOTIFICATION_CLICKED -> {

                    }
                }
            }
        }, IntentFilter(DownloadManager.ACTION_DOWNLOAD_COMPLETE))
    }
}
```

<br>
