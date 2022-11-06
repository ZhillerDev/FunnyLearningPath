### 线程处理

#### Android 多线程

Android 不允许直接在子线程内更新 UI！！！

直接使用 thread{}开启一个匿名线程，用 lambda 表达式写成；

```kotlin
thread{

}
```

<br>

### 基本服务

#### 完成一个基本的 service 步骤

1. 创建一个 service，并在 manifest 中注册
2. 在 service 中实现各种方法，特别是 onBind 方法
3. 主 activity 中使用 bindService 绑定我们创建的服务
4. 在主 activity 中对 service 做出指令

<br>

#### service 主要实现方法介绍

> 我们通过快捷创建的方法新建 MyService.kt，默认该类继承自 Service()

我们最常用的实现方法有如下几个，方法的作用自己看英文就懂了吧？

```kotlin
package com.zhiyiyi.listviewdemo.service

import android.app.Service
import android.content.Intent
import android.os.IBinder

class MyService : Service() {

    override fun onBind(intent: Intent): IBinder {
        TODO("Return the communication channel to the service.")
    }

    override fun onCreate() {
        super.onCreate()
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        return super.onStartCommand(intent, flags, startId)
    }

    override fun onDestroy() {
        super.onDestroy()
    }

    override fun onUnbind(intent: Intent?): Boolean {
        return super.onUnbind(intent)
    }

    override fun onRebind(intent: Intent?) {
        super.onRebind(intent)
    }
}
```

<br>

特别的，针对 `onBind` 方法，我们可以先让他实现 IBinder 接口，然后传入我们自己编写的一个 binder

此时，外部 activity 就可以通过该自定义 binder 实现对内部服务的自主操控了（譬如使用下载方法 startDownload）

```kotlin
// 实例化DownloadBinder
private val mBinder = DownloadBinder()

// 我们编写的自定义下载binder
class DownloadBinder : Binder() {

    fun startDownload() {
        Log.d("MyService", "startDownload executed")
    }

    fun getProgress(): Int {
        Log.d("MyService", "getProgress executed")
        return 0
    }

}

// 直接返回我们编写好的binder
override fun onBind(intent: Intent): IBinder {
    return mBinder
}
```

<br>

#### 主 activity 中开启服务

> 因为我们编写的 service 带有一个 binder，那么这里就不建议直接使用 startservice 进行开启服务操作，而应该使用 bindservice

代码很简单，我们首先自建一个匿名类实现完善 `ServiceConnection` 机制，后于按钮点击事件中运用 `bindService` 绑定服务就 OK 了

```kotlin
class MainActivity : AppCompatActivity() {

    // 延后初始化自定义binder
    lateinit var downloadBinder: MyService.DownloadBinder

    // 匿名类实现中，继承ServiceConnection接口，实现服务器建立连接与失去连接的方法
    private val connection = object : ServiceConnection {
        override fun onServiceConnected(name: ComponentName, service: IBinder) {
            // 实例化binder后，即可自主操纵binder内部的各种方法了！
            downloadBinder = service as MyService.DownloadBinder
            downloadBinder.startDownload()
            downloadBinder.getProgress()
        }

        override fun onServiceDisconnected(name: ComponentName) {
            Log.d("MyService", "onServiceDisconnected")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // 开启服务的点击事件
        bindServiceBtn.setOnClickListener {
            val intent = Intent(this, MyService::class.java)
            bindService(intent, connection, Context.BIND_AUTO_CREATE) // 绑定Service
        }
        // 解除服务绑定的点击事件
        unbindServiceBtn.setOnClickListener {
            unbindService(connection) // 解绑Service
        }
    }
}
```

<br>

#### service 生命周期

1. service 是单例的，无论执行多少次 `startService` 都只能最初创建的那个实例
2. 外部关闭 service 使用 `stopService` ，而 service 内部关闭自己使用 `stopSelf`
3. 若 service 内实现了 onBind 方法，启动服务时可以使用 `startService` 或者 `bindService`  
   但如果同时使用二者，那么注销服务的时候就必须同时写 `stopService` 和 `unbindService`
4. android8.0 之后，一旦应用不处于前台，则其使用的 service 很大概率会被后台回收掉

<br>

### 前台 service

首先声明权限

`<uses-permission android:name="android.permission.FOREGROUND_SERVICE"/>`

<br>

只需要对 service 的 onCreate 修改即可；

实际上就是使用 NotificationManager 创建通知频道后，用 PendingIntent 构建一个前台状态条而已；

具体细节修改请根据个人需要调节

```kotlin
override fun onCreate() {
    super.onCreate()
    Log.d("MyService", "onCreate executed")

    // 获取通知管理器
    val manager = getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
    // 判断SDK版本，并构建通知频道
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
        val channel = NotificationChannel("my_service", "前台Service通知", NotificationManager.IMPORTANCE_DEFAULT)
        manager.createNotificationChannel(channel)
    }

    // 使用PendingIntent创建一个前台通知
    val intent = Intent(this, MainActivity::class.java)
    val pi = PendingIntent.getActivity(this, 0, intent, 0)
    val notification = NotificationCompat.Builder(this, "my_service")
        .setContentTitle("This is content title")
        .setContentText("This is content text")
        .setSmallIcon(R.drawable.small_icon)
        .setLargeIcon(BitmapFactory.decodeResource(resources, R.drawable.large_icon))
        .setContentIntent(pi)
        .build()
    startForeground(1, notification)
}
```

<br>

### IntentService

`IntentService` 可以创建一个异步的、可自我停止的 service

一旦外部 activity 启动该类型服务，那么该服务将会在执行完 `onHandleIntent` 方法后立刻调用 `onDestroy` 销毁自己

```kotlin
// 继承IntentService，并将本service作为参数传递进去
class MyIntentService : IntentService("MyIntentService") {

    // service运行后的处理页面
    override fun onHandleIntent(intent: Intent?) {
        Log.d("MyIntentService", "Thread id is ${Thread.currentThread().name}")
    }

    override fun onDestroy() {
        super.onDestroy()
        Log.d("MyIntentService", "onDestroy executed")
    }

}
```

> 启动服务？直接 startService 即可，这里就不多废话了

<br>

### Service 进阶篇

> 还在填坑，别着急啊！！！
