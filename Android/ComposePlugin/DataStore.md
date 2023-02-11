### DataStore 概念

DataStore 可以存储两种类型的数据，一种是 preference，一种是 protobuf

每个进程在同一时间内仅能打开一个 DataStore 实例（或者通过其他管理手段来实现多个 DataStore 交替使用）

一个 DataStore 可以视为一张数据表

<br>

默认存储位置：`data/data/com.xxx.xxx/files/datastore/xxx.pb`

<br>

### 快速上手

导入依赖：`implementation "androidx.datastore:datastore-preferences:1.0.0"`

<br>

#### 目标

将三个属性 name、age、habit 按照键值对的方式使用 preference 模式存储到本地  
实现查询功能

<br>

#### datastore 主文件

新建文件 `DataStoreInit.kt` 用来初始化 DataStore

首先需要确定 DataStore 的名字，或者可以理解为这个数据表的名字

并使用 kotlin 特有的扩展方法的语法，直接把 dataStore 绑定在全局上下文，即可“一次初始化全局使用”

```kotlin
// 定义DataStore名字
private const val BASIC_DB_NAME = "basic_test"

// 全局委托实例化DataStore，接收一个参数，即名字
private val Context.dataStore by preferencesDataStore(name = BASIC_DB_NAME)
```

<br>

我们需要在外部定义键的类型

`xxxPreferencesKey` 其中的 xxx 代表类型

```kotlin
// 直接定义一组类型时最好这样写
private object PreferencesKeys{
    // 变量名（大写字符）为开发时使用的键名
    // 括号内的名字为存储到本地的键名
    val BASIC_NAME = stringPreferencesKey("basic_name")
    val BASIC_AGE =  stringPreferencesKey("basic_age")
    val BASIC_HABIT = stringPreferencesKey("basic_habit")
}
```

<br>

直接定义一个数据类，方便我们后续直接提取出三个数据！

```kotlin
data class basicModel(val name:String,val age:String,val habit:String)
```

<br>

编写新类 DataStoreInit，他接受一个 context 上下文作为初始化形参

所有的 CRUD 操作都在此类中进行！

```kotlin
class DataStoreInit(private val context: Context) {
    ...
}
```

<br>

**读取数据**

读取数据时需要使用 trycatch 代码块进行异常捕获

```kotlin
// 下面代码是固定格式，不要纠结
val getBasicDB = context.dataStore.data
.catch {e->
    // 捕获异常并作出处理
    if(e is IOException){
        emit(emptyPreferences())
    }else{
        throw e
    }
}.map {preferences ->
    // 依据键值对的方式提取出存储的值
    // 格式：preferences[定义的键名]
    // ?:类似于三元运算，即左侧取出null则直接使用右侧值替换之
    val name = preferences[PreferencesKeys.BASIC_NAME] ?: "无名氏"
    val age = preferences[PreferencesKeys.BASIC_AGE] ?: "inf"
    val habit = preferences[PreferencesKeys.BASIC_HABIT] ?: "清心寡欲乜有爱好"

    // 把所有数据存储到数据类并将它返回！
    basicModel(name, age, habit)
}
```

<br>

**写入数据**

数据写入需要使用挂起函数（故调用该方法时也要使用协程作用域）

```kotlin
// 挂起函数，接收三个参数作为值输入
suspend fun insertBasicDB(name: String,age: String,habit: String):String{
    return try {
        // 依据键值对存储即可
        context.dataStore.edit {preferences ->
            preferences[PreferencesKeys.BASIC_NAME] = name
            preferences[PreferencesKeys.BASIC_AGE] = age
            preferences[PreferencesKeys.BASIC_HABIT] = habit
        }
        "基础数据插入成功"
    }catch (e:Exception){
        println(e)
        "基础数据插入失败"
    }
}
```

<br>

#### 主 UI

主页面配置：三个输入框，加一个插入按钮、一个查询按钮、一个清空表单按钮

功能大体解释：

1. 判断输入框有任意一个为空，则不允许插入
2. 根据返回值判断成功与否并弹出 Toast
3. 使用协程作用域 rememberCoroutineScope 进行读写操作

```kotlin
@Composable
fun DataStoreViewInit(navHostController: NavHostController){
    // 三个主要变量，记得remember
    var name by remember {
        mutableStateOf("tom")
    }
    var age by remember {
        mutableStateOf("100")
    }
    var habit by remember {
        mutableStateOf("play")
    }

    // 定义协程作用域，用于数据读写
    val scope = rememberCoroutineScope()

    Column(
        Modifier
            .fillMaxHeight()
            .fillMaxWidth(),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        TextField(value = name, onValueChange = {name=it}, label = { Text(text = "用户名")})
        TextField(value = age, onValueChange = {age=it}, label = { Text(text = "年龄")})
        TextField(value = habit, onValueChange = {habit=it}, label = { Text(text = "密码")})

        // 保存数据按钮
        Button(onClick = {
            // 判断编辑框均非空
            if(name!==""&&age!==""&&habit!==""){
                scope.launch {
                    // 保存数据
                    val res = DataStoreInit(MainContext).insertBasicDB(name, age, habit)
                    DialogUtil(MainContext).showSimpleToast(res)
                }
            }else{
                DialogUtil(MainContext).showSimpleToast("请填完所有信息！！！")
            }
        }) {
            Text(text = "提交信息")
        }

        // 查询数据按钮
        Button(onClick = {
            try {
                scope.launch {
                    // 查询数据
                    // 由于挂起函数返回类型为一个flow类型对象，所以使用collect取出值即可
                    DataStoreInit(MainContext).getBasicDB.collect {
                        name = it.name
                        age = it.age
                        habit = it.habit
                    }
                }
                DialogUtil(MainContext).showSimpleToast("已经取出数据！")
            }catch (e:Exception){
                DialogUtil(MainContext).showSimpleToast("完蛋了找不到数据！")
            }
        }) {
            Text(text = "查询信息")
        }

        // 清空表单按钮
        Button(onClick = {
            name=""
            age=""
            habit=""
        }) {
            Text(text = "清空表单")
        }
    }
}
```

<br>

补上一个我封装的弹出 Toast 的小工具类

```kotlin
class DialogUtil(private val context: Context) {
    fun showSimpleToast(str:String){
        Toast.makeText(context,str,Toast.LENGTH_SHORT).show()
    }
}
```

<br>
