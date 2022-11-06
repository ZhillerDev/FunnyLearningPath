### 介绍

<br>

#### 什么是行为参数化？

这种模式可以把一个行为（一段代码）封装起来，并通过传递和使用创建的行为（例如对 Apple 的不同谓词）将方法的行为参数化

<br>

#### 例子

如使用 runnable 接口创建不同线程时，可以利用匿名函数直接对接口进行实现

```java
Thread t=new Thread(new Runnable() {
    public void run(){
        System.out.println("Hello world");
    }
});
```

<br>

仅当被实现的接口有且仅有一个方法需要被实现时，可以直接使用 lambda 表达式

以下代码等同于上方使用匿名函数的方法

```java
Thread t=new Thread(()-> System.out.println("Hello world"));
```

<br>

### 苹果商例子

> 在原书上的基础做出改动

<br>

#### Java8 以前的非行为参数处理办法

首先创建一个 Apple 类，该类有两个参数 color 和 weight；

使用 intellij 的 alt+insert 快捷键快速插入 getter、setter 、tostring 以及构造方法

> Apple.java 代码清单

```java
public class Apple {

    private String color;
    private int weight;

    public Apple(String color, int weight) {
        this.color = color;
        this.weight = weight;
    }

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }

    public int getWeight() {
        return weight;
    }

    public void setWeight(int weight) {
        this.weight = weight;
    }

    @Override
    public String toString() {
        return "Apple{" +
                "color='" + color + '\'' +
                ", weight=" + weight +
                '}';
    }
}
```

因为苹果商在选购苹果时需要判断是否是红色苹果，否则不收；

故编写一个接口 `AppPredicate` 来进行苹果属性的判断；

而实现类 `AppleRedHandle` 则完成了“判断苹果是否为红色”的操作

> AppPredicate.java 代码清单

<br>

```java
public interface AppPredicate {
    Boolean accept(Apple a);
}

class AppleRedHandle implements AppPredicate{
    @Override
    public Boolean accept(Apple a) {
        return "red".equals(a.getColor());
    }
}
```

<br>

那么很好，现在我们就要执行苹果分类的操作了！

下面是一些关键代码的解释：

filterApple 方法

1. 为了进行分类，他的第二个参数是一个接口，我们必须为其提供一个实现类、匿名函数或者 lambda 其中之一，而下方的代码使用实现类；
2. 我们在 `AppPredicate.java` 中每编写一个实现类都可以实现一种分类功能，在这里即可进行适配，来决定使用那种不同的分类策略！

> FilterApples.java 代码清单

```java
public class FilterApples {
    public static void main(String[] args) {
        // 新建一个背包，里面装了三个苹果
        List<Apple> inv = Arrays.asList(
                new Apple("red", 123),
                new Apple("green", 155),
                new Apple("red", 120)
        );

        // 通过分类方法获取分类后的苹果
        List<Apple> redApples = filterApple(inv, new AppleRedHandle());

        // 打印苹果信息
        getApplesInfo(redApples);
    }

    // 苹果分类方法
    // 他的第二个参数使用接口AppPredicate来接受一个实现类
    public static List<Apple> filterApple(List<Apple> inv, AppPredicate app) {
        List<Apple> res = new ArrayList<>();
        // 对每个苹果进行判断，如果符合条件那么就返回true
        for (Apple apple : inv) {
            if (app.accept(apple)) {
                res.add(apple);
            }
        }
        return res;
    }

    // 输出苹果属性的方法
    public static void getApplesInfo(List<Apple> apples) {
        for (Apple apple : apples) {
            System.out.println(apple.toString());
        }
    }
}
```

<br>

#### 行为参数化处理方法

我们保持以上代码都不变，我们只需要使用匿名函数编写分类方法即可省去冗余的实现类的编写！

现在我们的分类器可以按照以下方法直接使用匿名函数进行接口实现

```java
// 实现了找到青苹果的方法
List<Apple> greenApples = filterApple(inv, new AppPredicate() {
    @Override
    public Boolean accept(Apple a) {
        return "green".equals(a.getColor());
    }
});
```

<br>

或者更简单一些，直接使用 lambda 函数简化；

注意：当且仅当接口仅有一个方法需要被实现才可以使用 lambda！

```java
// 实现了找到质量小于150苹果的方法
List<Apple> lightApples = filterApple(inv, (Apple apple) -> apple.getWeight() < 150);
```

<br>
