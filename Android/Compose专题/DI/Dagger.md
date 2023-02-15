### 简介

Dagger 的优势

1. 对类使用注解提供依赖关系，Dagger 会自动处理
2. Dagger 自动构建依赖关系图，根据该图注入对应实例
3. Dagger 会为图中每个类生成对应工厂方法，获取实例对象
4. 可以避免依赖循环

<br>

### Dagger 生成简单工厂

对欲要注入的类声明@Inject 注解，且其构造函数内的依赖对象对应类也必须要使用该注解标记，否则 Dagger 无法寻找到对应的依赖项并注入

```kotlin
// 使用@Inject注解声明注入
class UserRepository @Inject constructor(
    private val localDataSource: UserLocalDataSource,
    private val remoteDataSource: UserRemoteDataSource
) { ... }

// 对于被注入的类，依然也需要使用@Inject注解，否则Dagger无法识别！
class UserLocalDataSource @Inject constructor() { ... }
class UserRemoteDataSource @Inject constructor() { ... }
```

<br>
