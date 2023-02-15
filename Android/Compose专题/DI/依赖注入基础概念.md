### 依赖注入

> 主要介绍依赖注入相关概念

<br>

#### 依赖注入与非依赖注入

什么是依赖注入（DI）？  
假设有类 Car，他需要使用到实例 Person，则称 Car 需要依赖 Person 而存在。那么存在以下两种方式：

非依赖注入：对象自己获取对应实例；  
依赖注入：直接为对象提供他需要的实例（即注入对象）；

```kotlin
// 非依赖注入
// 直接在类内实例化依赖对象，然后调用
class Car {
    private val engine = Engine()
    fun start() {
        engine.start()
    }
}
fun main(args: Array) {
    val car = Car()
    car.start()
}

// 依赖注入
// 直接通过形参为对象提供它所需要的实例
class Car(private val engine: Engine) {
    fun start() {
        engine.start()
    }
}
fun main(args: Array) {
    val engine = Engine()
    val car = Car(engine)
    car.start()
}
```

<br>

依赖注入又可以分为两种注入方式：

1. 构造函数注入（即上方代码展示的）
2. 字段注入（或称为 setter 注入），即对象内延后初始化实例，在主入口直接将实例赋予给对象

<br>

上面演示的是手动依赖注入，效率极低，目前主要使用 Dagger 库提供的自动依赖注入

而 Hilt 又继承了 Dagger 库的特性，提供了新的更为易用的 DI 框架

<br>

#### 服务定位器

依赖注入可以使用服务定位器代替，但是会导致代码更难测试以及生命周期冗杂，极不推荐

```kotlin
object ServiceLocator {
    fun getEngine(): Engine = Engine()
}

class Car {
    private val engine = ServiceLocator.getEngine()

    fun start() {
        engine.start()
    }
}

fun main(args: Array) {
    val car = Car()
    car.start()
}
```

<br>

#### 依赖注入优势

1. 重用类以及分离依赖项
2. 易于重构
3. 易于测试

<br>

### 手动依赖注入
