### 基本流

> foreach 实现了外部迭代，它是建立于集合的基础上；  
> stream 流实现的是内部迭代

<br>

#### 中间操作与终端操作

对任意集合使用 `stream()` 方法来使用流操作来对其进行内部迭代；

中间操作：流水线上的中间操作，他会返回一个 stream

终端操作：一般位于流水线末尾，输出一个非 stream 类型的值

这是下方代码的完整运行过程：

1. 首先使用 stream 方法使用流来处理集合 names
2. 使用过滤器 filter，过滤出字符串中含有字符 a 的字符串
3. distinct 进行去重
4. toList 将以上所有中间操作过后得到的结果进行汇总，转换成 list 并输出

```java
public static void main(String[] args) {
    List<String> strings = new ArrayList<>();
    strings.add("asd");
    strings.add("asd");
    strings.add("qwe");

    List<String> names = strings.stream()
            // 过滤
            .filter(d -> {
                return d.contains("a");
            })
            // 去重
            .distinct()
            // 转换成列表
            .toList();
    for (String name : names) System.out.println(name); // 输出：asd
}
```

<br>

#### 一些常见的终端操作

`foreach`：消费流中所有元素并使用 lambda，该操作返回 void

`count`：返回流中的元素个数，返回类型为 long

`collect`：把流处理结果返回成一个集合，譬如 toList 方法

<br>

### 进阶流

#### 筛选各异的元素

distinct 进行去重

```java
public static void main(String[] args) {
    List<Integer> numbers = Arrays.asList(1, 2, 3, 1, 2, 4, 2, 2);
    numbers.stream()
            .filter(i -> i % 2 == 0)
            .distinct()
            .forEach(System.out::println);
}
```

<br>

#### 截断

limit，做多返回前 n 个元素

```java
public static void main(String[] args) {
    List<Integer> numbers = Arrays.asList(1, 2, 3, 1, 2, 4, 2, 2);
    numbers.stream()
            .limit(3)
            .forEach(System.out::println);
}
```

<br>

#### 跳过元素

skip(4) 表示跳过前四个元素，并返回后面所有的元素；

由于代码都差不多，这里不做过多演示

<br>

### 映射流

#### map

map 接收一个函数作为参数，他将对集合中的每个元素应用该函数，并返回该函数规定的值；

如下代码使用 map 获取了每个 dish 的名称并返回它，由此生成了由 dishname 构成的列表

```java
public static void main(String[] args) {
    List<Dish> dishes = new ArrayList<>();
    dishes.add(new Dish("apple", true, 120, Dish.Type.FRUIT));
    dishes.add(new Dish("pork", false, 900, Dish.Type.MEAT));
    dishes.add(new Dish("fish", false, 400, Dish.Type.FISH));

    List<String> dishNames = dishes.stream()
            .map(Dish::getName)
            .toList();
    dishes.forEach(System.out::println);
}
```

<br>

#### flatMap

map 是将每个元素都映射成了一个流；

flatMap 是将每个元素都放如同一个流中！

以下代码实现：提取单词表中出现的单词；  
`Arrays.stream` 可以将集合转换成流的形式；

> 应当首先应用 map，将元素进行映射后，在使用 flatMap 把这些映射完毕的元素全部装入一个统一的流里面！

```java
public static void main(String[] args) {
    // 创建单词表
    String[] words = {"goodbye", "hello", "night"};
    // 将单词表转换成流的形式
    Stream<String> swords = Arrays.stream(words);

    // 首先使用map，将所有字母都一一分割来开
    List<String> strings = swords.map(word -> word.split(""))
            // 使用扁平map，将提取到的字母装入一个流中
            .flatMap(Arrays::stream)
            // 字母去重
            .distinct()
            // 转换成列表
            .toList();
    strings.forEach(System.out::println);
}
```

<br>

#### 匹配

anyMatch 能应答：流中是否有至少有一个元素能匹配所给定的谓词（方法引用）

```java
public static void main(String[] args) {
    List<Dish> dishes = Menu.getMenu();
    if(dishes.stream().anyMatch(Dish::isVegetarian)){
        System.out.println("至少有一个素菜！");
    }
}
```

<br>

#### 全匹配与非全匹配

allMatch 方法检查谓词能否匹配所有元素；

noneMatch 方法作用与 allMatch 方法完全相反；

<br>

### Optional

#### findAny

`findAny` 方法默认返回一个 `Optional<T>`

ifPresent 会在值存在的时候执行后面的 lambda 函数

```java
public static void main(String[] args) {
    List<Dish> dishes = Menu.getMenu();
    dishes.stream()
            .filter(Dish::isVegetarian)
            .findAny()
            .ifPresent(d -> System.out.println(d.getName()));
}
```

<br>

#### findFirst

顾名思义，找到集合中的第一个元素

<br>

### 归约

#### 求和

下方展示了三种求和方式：

1. 直接使用 arrays 函数自带的 sum 方法求和；
2. 使用 reduce，参数一表示初始值，参数二为对值的操作
3. 同样使用 reduce，但是引入了谓词！

```java
public static void main(String[] args) {
    int[] nums = {1, 2, 3, 4, 5};

    int sum = Arrays.stream(nums).sum();
    int sum2 = Arrays.stream(nums)
            .reduce(0, (a, b) -> a + b);
    int sum3 = Arrays.stream(nums)
            .reduce(0, Integer::sum);
}
```

<br>

#### 归约相关阐发

map+reduce 常被作为一种形式来进行并行化；

使用 reduce 进行的迭代被内部抽象化掉了，所以有益于并行的执行；

parallelStream 可以方便的进行并行操作，但是也带来了同步不安全的隐患；

<br>

### 数值流

使用 mapToInt 返回特化的 IntStream

```java
public static void main(String[] args) {
    List<Dish> dishes = Menu.getMenu();
    int sum = dishes.stream()
            .mapToInt(Dish::getCalories)
            .sum();

}
```

<br>

#### boxed

将某个特化流转换成一般流，可以使用 boxed 方法

```java
IntStream is = dishes.stream().mapToInt(Dish::getCalories);
Stream<Integer> sis = is.boxed();
```

<br>

#### range

针对特化流 IntStream 和 LongStream，他们均有 range 和 rangeClose 方法；

range：遍历范围内所有数字除了右边界数字  
rangeClose：遍历包含右边界的所有范围数字

> 下面代码展示了获取 1-100 内所有偶数的个数

```java
IntStream evenNumbers = IntStream.rangeClosed(1,100)
        .filter(n->n%2==0);
System.out.println(evenNumbers.count());
```

<br>

### 构建流

#### 创建流的三种方式

Stream.of(xxx) 通过数值直接构建；

Arrays.stream() 数组创建；

File.lines() 通过获取的文件内容创建；

<br>
