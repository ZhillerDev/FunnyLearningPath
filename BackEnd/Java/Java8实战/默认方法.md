### 默认方法

#### default

为接口编写一个默认方法，则其实现类就无须实现该方法，避免实现类再次实现某些无用的冗余接口！

譬如我们编写一个 `Subject` 接口，里面包含了一个默认方法 printTest，那么我们后续对该接口进行实现的话，就无须再次实现 `printTest` 方法了！

```java
public interface Subject {
    boolean select();

    default void printTest(){
        System.out.println("test");
    }
}
```

<br>

#### 默认方法重名问题

众所周知，一个类仅可以继承一个父类，但是可以实现 N 多个接口；  
然而若实现的接口中均存在同名的默认方法，那么 JAVA 就会进行一定规则的取舍，以判断使用哪一个方法；

<br>

**方法一**：按照“谁更具体就选谁”  
譬如接口 AB 均有默认方法 Hey()，而 B 继承了 A，且 C 实现 AB；  
可见虽然 AB 均有相同默认方法，但是 B 中的 Hey()更加具体，所以 C 就使用 B 中的对应默认方法；

<br>

方法二：显式调用  
请参考如下代码，当无法使用方法一进行判断的时候，我们必须显式指出我们需要继承哪一个接口的默认方法；  
如果不这么做，就会抛出异常；

> 接口代码清单：Subject.java

```java
public interface Subject {
    default void demo(){
        System.out.println("subject");
    }
}
```

> 接口代码清单：Course.java

```java
public interface Course {
    default void demo(){
        System.out.println("course");
    }
}
```

> 实现类代码清单：Math.java

```java
public class Math implements Subject,Course{
    @Override
    public void demo() {
        // 明确表示我需要实现的是Subject接口中的默认方法
        Subject.super.demo();
    }
}
```

<br>

#### 讨厌的菱形继承问题

C++中有所谓的菱形继承，字面意思，具体解释请百度百科；

若要规避这些棘手问题，请遵守以下三大法则：

1. 类或父类中显式声明的方法，其优先级高于所有的默认方法
2. 如果用第一条无法判断，方法签名又没有区别，那么选择提供最具体实现的默认方法的接口
3. 如果冲突依旧无法解决，你就只能在你的类中覆盖该默认方法，显式地指定在你的类中使用哪一个接口中的方法

<br>

### END
