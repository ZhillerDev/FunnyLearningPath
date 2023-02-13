### DisposableEffect

> DisposableEffect 适用于在键发生变化或可组合项退出组合后需要清理的附带效应

对于部分非组合方法（即没有用@Composable 注解的方法），他们本身是没有生命周期的，需要我们自己定义一个生命周期并监听！

比如我们针对 MapView 定义一个 LifecycleObserver

```kotlin
private fun getMapLifecycleObserver(mapView: MapView): LifecycleEventObserver =
    LifecycleEventObserver { _, event ->
        when (event) {
            // 为mapView绑定了所有的生命周期
            Lifecycle.Event.ON_CREATE -> mapView.onCreate(Bundle())
            Lifecycle.Event.ON_START -> mapView.onStart()
            Lifecycle.Event.ON_RESUME -> mapView.onResume()
            Lifecycle.Event.ON_PAUSE -> mapView.onPause()
            Lifecycle.Event.ON_STOP -> mapView.onStop()
            Lifecycle.Event.ON_DESTROY -> mapView.onDestroy()
            else -> throw IllegalStateException()
        }
    }
```

<br>

之后在调用 MapView 的方法内调用该侦测器即可

`DisposableEffect` 监听的参数一旦发生变化就系统会移除观察器并再次将其添加到正确的 lifecycle

```kotlin
@Composable
fun rememberMapViewWithLifecycle(): MapView {
    val context = LocalContext.current
    val mapView = remember {
        MapView(context).apply {
            id = R.id.map
        }
    }

    val lifecycle = LocalLifecycleOwner.current.lifecycle
    DisposableEffect(key1 = lifecycle, key2 = mapView) {
        // 把当前的mapview添加到侦测器
        val lifecycleObserver = getMapLifecycleObserver(mapView)
        lifecycle.addObserver(lifecycleObserver)
        // 卸载侦测器
        onDispose {
            lifecycle.removeObserver(lifecycleObserver)
        }
    }

    return mapView
}
```

<br>

### produceState
