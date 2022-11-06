### 函数式接口

定义：仅定义一个抽象方法的接口；

哪怕接口有很多个默认方法，但只要仅有一个抽象方法，那他就还是函数式接口；

lambda 函数只能用于函数式接口；

<br>

### Predicate 函数式接口去重

#### 完整代码

> 看不懂没关系，先理解一下

```java
public class PredicateTest {

    public static <T> List<T> filter(List<T> list, Predicate<T> p) {
        List<T> res = new ArrayList<>();
        for (T t : list) {
            if (p.test(t)) {
                res.add(t);
            }
        }
        return res;
    }

    public static Predicate<String> nonEmpty = (String str) -> !str.isEmpty();

    public static void main(String[] args) {
        List<String> list = new ArrayList<>();
        list.add("123");
        list.add("");
        list.add("empty");

        list = filter(list, nonEmpty);

        for (String str : list) {
            System.out.println(str);
        }
    }
}
```

<br>

#### filter 过滤器

Predicate 是一个函数式接口，他有且仅有一个抽象方法 test，其返回值为 boolean；

以下过滤器的执行过程是：

- 新建一个列表；
- 判断参数列表中的元素是否能通过 Predicate 的 test，如果能，就加入到新列表内！
- 最后返回经过处理过后的新列表

```java
public static <T> List<T> filter(List<T> list, Predicate<T> p) {
    List<T> res = new ArrayList<>();
    for (T t : list) {
        if (p.test(t)) {
            res.add(t);
        }
    }
    return res;
}
```

<br>

#### Predicate 判断非空

因为 Predicate 为函数式接口，所以可以直接使用 lambda 表达式来定义规则；

这里的规则是，检测传入的字符串是否为空，如果为空则返回 false

```java
public static Predicate<String> nonEmpty = (String str) -> !str.isEmpty();
```

<br>

#### 实践

很简单，下方新建数组列表并插入了一些数据，包括一个空值；

然后使用 filter 方法，配合 predicate，成功进行操作；

最后输出列表内容即可

```java
public static void main(String[] args) {
    List<String> list = new ArrayList<>();
    list.add("123");
    list.add("");
    list.add("empty");

    list = filter(list, nonEmpty);

    for (String str : list) {
        System.out.println(str);
    }
}
```

<br>

### Consumer

他也是一个函数式接口；

Consumer 有一个抽象方法 accept，他接受泛型 T 的值，无返回值；

下面是一个自定义 foreach 方法的案例：

```java
// 对遍历到的每个元素都执行一次accept
public static <T> void customForEach(List<T> list, Consumer<T> consumer) {
    for (T t : list) {
        consumer.accept(t);
    }
}

public static void main(String[] args) {
    // accept方法定义为输出传入的值
    customForEach(
            Arrays.asList(1,2,3,4),
            (Integer i) -> System.out.println(i)
    );
}
```

<br>

### Function

`Function` 有一个抽象方法 `apply`,它接受一个泛型 T 的对象，并返回一个泛型 R 的对象

下面代码展示获取一个列表中所有字符串的长度并插入到一个新的列表内；

```java
public class FunctionTest {

    public static <T, R> List<R> mapLength(List<T> list, Function<T, R> function) {
        List<R> res = new ArrayList<>();
        for (T t : list) {
            // 对每个列表项进行apply操作后插入到新的列表内
            res.add(function.apply(t));
        }
        return res;
    }

    public static void main(String[] args) {
        List<Integer> integers = mapLength(
                Arrays.asList("hello","bigger","apple"),

                // 方法引用式写法
                // 这里function的T为String，而R自动推断为Integer
                String::length
        );
    }
}
```

<br>

### 其他常见函数式接口

`Consumer<T>` 抽象方法叫作 accept，代表的函数描述符是 T-> void

`Supplier<T>` 抽象方法叫作 get，代表的函数描述符是()-> T

`Callable<T>` 抽象方法叫作 call，代表的函数描述符是()-> T

`BiFunction<T, U, R>` 抽象方法叫作 apply，代表的函数描述符是(T, U)-> R

<br>

### 方法引用

#### 方法引用有以下三种

指向静态方法的方法引用（例如 Integer 的 parseInt 方法，写作 Integer::parseInt）

```java
// lambda形式
(args) -> Classname.mtd(args)
// 方法引用形式
Classname::mtd
```

指向任意类型实例方法的方法引用（例如 String 的 length 方法，写作 String::length）

```java
// lambda形式
(s1,s2) -> s1.getName(s2)
// 方法引用形式
Classname::getName
```

指向现有对象的实例方法的方法引用（假设你有一个局部变量 expensiveTransaction 用于存放 Transaction 类型的对象，它支持实例方法 getValue，那么你就可以写 expensive-Transaction::getValue）

<br>

#### 构造函数引用

可以借助方法引用的 new 来快速构建函数引用

```java
// 对于无参构造，可以使用supplier
Supplier<String> sp = () -> new String();
// 方法引用创建
Supplier<String> sp2 = String::new;
String s1 = sp.get();
```

<br>

假设有如下场景：我们编写了一个类 Car，它的构造函数接收两个参数，分别是 name 和 price；

我们可以使用基本的 lambda 构造函数，或者使用快捷的方法引用构造函数；

> 注意，BiFunction 的尖括号内提供的数据类型，前两个对应构造函数内形参的类型，最后一个是返回值类型！！！

```java
// 方法引用
BiFunction<String, Integer, Car> getCar = Car::new;
Car c1 = getCar.apply("bmw", 1000000);

// lambda
BiFunction<String, Integer, Car> getFakeCar = (name, price) -> new Car(name, price);
Car c2 = getFakeCar.apply("benz",9999999);
```

<br>

#### 更多函数式接口？

如果你想要使用方法引用的方式创建具有三个及以上形参的类，那么对应的函数式接口就需要我们自行编写了；

下面是书中给出的，接收三个参数并返回一个值的函数式接口：

```java
public static interface TriFunction<T, U, V, R> {
    R apply(T t, U u, V v);
}
```

<br>

### 谓词复合

> 以下例子将会用到上面我们创建的 Car 类

<br>

#### and

使用 and 将不同函数式接口链接在一起

cheapCar 找到便宜的车；  
cheapAndBMW 通过谓词复合，找到了既便宜又是宝马的车

```java
Predicate<Car> cheapCar = car -> car.getPrice() < 100;
Predicate<Car> cheapAndBMW = cheapCar.and(car -> "BMW".equals(car.getName()));
```

<br>

#### negate

对函数式接口取反

<br>

#### or

取或操作

<br>

### 函数复合

#### andThen

Function 接口自带了该方法，来让多个 Function 得以复合；

andThen 结果类似套娃，不好解释，看代码和注释：

```java
Function<Integer, Integer> f = x -> x + 1; // fx = x+1
Function<Integer, Integer> g = x -> x * 2; // gx = x*2
Function<Integer, Integer> h = f.andThen(g); // hx = g(f(x))
int res = h.apply(10); // 22
```

<br>

#### compose

方法和 andThen 相反，被执行函数是套在外面而不是里面的！

```java
Function<Integer, Integer> f = x -> x + 1; // fx = x+1
Function<Integer, Integer> g = x -> x * 2; // gx = x*2
Function<Integer, Integer> h = f.compose(g); // hx = f(g(x))
int res = h.apply(10); // 21
```

<br>
