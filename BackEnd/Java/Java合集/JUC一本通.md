## Java 线程

<br>

### 创建和运行线程

thread 构造，自己重写 run 函数

```java
// 构造方法的参数是给线程指定名字，推荐
Thread t1 = new Thread("t1") {
 @Override
 // run 方法内实现了要执行的任务
 public void run() {
 log.debug("hello");
 }
};
t1.start();
```

<br>

实现 runnable 接口

```java
// 创建任务对象
Runnable task2 = new Runnable() {
 @Override
 public void run() {
 log.debug("hello");
 }
};
// 参数1 是任务对象; 参数2 是线程名字，推荐
Thread t2 = new Thread(task2, "t2");
t2.start();
```

<br>

lambda 实现 runnable 接口

```java
// 创建任务对象
Runnable task2 = () -> log.debug("hello");
// 参数1 是任务对象; 参数2 是线程名字，推荐
Thread t2 = new Thread(task2, "t2");
t2.start();
```

<br>

## Java 内存模型

<br>

### 可见性

```java
static boolean run = true;
public static void main(String[] args) throws InterruptedException {
 Thread t = new Thread(()->{
 while(run){
 // ....
 }
 });
 t.start();
 sleep(1);
 run = false; // 线程t不会如预想的停下来
}
```

我们原本的意图是想通过修改 run 的值来让线程停下来，但事实上他根本就不会停

因为线程对于需要频繁使用到的变量，会预先执行缓存，他之后用到的变量就全部从缓存里面取；  
而我们修改的是主内存的值，线程却用的是旧的值，自然就不会停下来了！

<br>

### 原子性

子性（Atomicity）是指一个操作是不可分割的、不可中断的单个操作单位。在并发编程中，原子性是指一个操作在多线程环境下要么完全执行，要么完全不执行，不存在部分执行的情况。

原子性是确保多线程环境下数据一致性和线程安全性的重要概念。如果一个操作不具备原子性，多个线程同时对其进行读写操作时可能会产生竞态条件（race condition）和数据不一致的问题。

为了确保原子性，可以使用同步机制，如 synchronized 关键字或 Lock 接口，或者使用原子类（Atomic classes），如 AtomicInteger。

<br>

### 指令重排

**啥是指令重排？？？**

`指令重排（Instruction reordering）`是计算机编译器和处理器在执行指令时的一种优化技术。它的目的是通过重新排列指令的执行顺序，以最大程度地提高程序的性能。

指令重排的核心思想是尽可能地重排指令的执行顺序，以便最大程度地利用处理器的流水线（pipeline）和其他硬件特性  
重排后运行顺序变化了，但是结果是一致的（非多线程条件下）

Java 提供了一些内存屏障（Memory barriers）的机制，如 volatile 关键字和 java.util.concurrent 包中的并发工具，来避免指令重排特性在多线程并发条件下的影响

<br>

**指令重排应付方案**

1. `volatile 关键字` ：声明共享变量为 volatile，可以确保对该变量的读写操作具有可见性和有序性。volatile 关键字会禁止指令重排，保证了变量的修改对所有线程可见。
2. `锁（synchronization）` ：通过 synchronized 关键字或 Lock 接口来实现线程之间的同步，确保共享变量的访问是互斥的，并保证了可见性和有序性。锁的释放和获取操作会形成内存屏障，阻止指令重排。
3. `原子类（Atomic classes）` ：Java 提供了一组原子类，如 AtomicInteger、AtomicLong 等，它们提供了原子的读写操作，保证了可见性和有序性。原子类的操作使用了底层的硬件级指令，防止了指令重排。
4. `并发容器（Concurrent collections）` ：Java 的并发容器，如 ConcurrentHashMap、ConcurrentLinkedQueue 等，提供了线程安全的操作，并解决了可见性和内存一致性问题。这些容器内部使用了适当的同步机制，确保了线程之间的正确操作。

下面是使用 volatile 解决指令重排问题的小案例

```java
public class ReorderingExample {

    private static volatile boolean flag = false;

    public static void main(String[] args) {
        Thread writerThread = new Thread(() -> {
            flag = true;
        });

        Thread readerThread = new Thread(() -> {
            while (!flag) {
                // 保证可见性
            }
            System.out.println("Flag is true");
        });

        // 这样子就不会发生先读flag=0然后直接退出线程的窘境了
        writerThread.start();
        readerThread.start();
    }
}
```

<br>

### 有序性

`Java 内存模型（Java Memory Model，JMM）` 定义了一组规则来确保操作的有序性。

1. 程序顺序规则（Program Order Rule）：在一个线程中，按照程序的顺序执行的操作，其执行顺序是有序的。
2. volatile 变量规则（Volatile Variable Rule）：对一个 volatile 变量的写操作保证先于后续对同一个变量的读操作，从而保证了对 volatile 变量的操作有序性。
3. 锁规则（Lock Rule）：一个解锁操作保证先于后续对同一个锁的加锁操作，从而保证了对锁保护的数据的操作有序性。
4. 传递性规则（Transitive Rule）：如果操作 A 先于操作 B，操作 B 先于操作 C，那么操作 A 先于操作 C，从而保证了操作之间的传递性有序性。

<br>

**Happens-before**

"Happens-before" 是 Java 内存模型（Java Memory Model，JMM）中的一个概念，用于描述程序中操作执行的顺序和对共享变量的可见性。

happens-before 关系定义了一组规则，用于指定操作之间的顺序关系。如果操作 A happens-before 操作 B，那么 A 的执行结果对 B 是可见的，而且 A 的执行顺序在 B 之前。

<br>

### volatile

`volatile` 是一个关键字，用于声明变量。它的主要作用是确保多个线程之间对该变量的访问具有可见性和有序性。

当一个变量被声明为 volatile 时，它具有以下特性：

1. 可见性（Visibility）：当一个线程修改了一个 volatile 变量的值时，该变量的新值会立即被写入主内存（共享内存），而不是只保存在线程的本地缓存中。其他线程在读取该变量时，会从主内存中获取最新的值，而不是使用本地缓存的旧值。这确保了对该变量的修改对所有线程都是可见的。
2. 有序性（Ordering）：volatile 变量的读写操作会按照其在代码中出现的顺序进行排序，即保证了有序性。这意味着在一个线程中，对 volatile 变量的写操作先于后续的读操作，不会发生重排序。

> TIPS:请注意，volatile 不确保原子性，复杂环境下仍有可能发生竞态条件问题，需要利用 synchronized 或 lock 解决

<br>

### balking 模式

Balking 模式是一种设计模式，用于处理多线程环境下的并发控制。它适用于一种情况，即当线程检测到某个条件不满足时，就放弃（balk）执行当前操作，而不是等待条件满足后再执行。

检测节点可以在线程执行某个任务之前或期间

下面是使用 balking 模式进行共享数据类 Data 更新检测的小案例

```java
public class Data {
    private boolean isChanged = false;

    public synchronized void change() {
        // 检查条件是否满足
        if (isChanged) {
            // 如果条件已经满足，直接返回，放弃执行
            return;
        }

        // 修改数据的操作
        // ...

        // 设置条件已经满足
        isChanged = true;

        // 执行其他操作
        // ...
    }
}
```

如果数据已被修改，isChange=true，那就啥都不管  
如果数据未被修改，isChange=false，就必须要进行数据修改更新了

<br>

### 单例模式线程安全

采用`双重检查锁定（Double-Checked Locking）`的方式来改进单例模式的实现

```java
public final class Singleton {
    private Singleton() { }
    private static volatile Singleton INSTANCE = null;

    public static Singleton getInstance() {
        if (INSTANCE == null) {
            synchronized (Singleton.class) {
                if (INSTANCE == null) {
                    INSTANCE = new Singleton();
                }
            }
        }
        return INSTANCE;
    }
}
```

在双重检查锁定方式中，首先检查 INSTANCE 是否为 null，如果为 null，才进入同步块。在同步块内部再次检查 INSTANCE 是否为 null，这样可以避免多线程并发情况下创建多个实例的问题。  
在使用双重检查锁定时，需要将 INSTANCE 声明为 volatile 类型，以确保多线程环境下的可见性。

<br>

## 共享模型-无锁

<br>

### 指令交错

**单核指令交错**

单核处理器中，指令交错（Instruction Interleaving）是一种技术，通过交错执行指令的方式来提高处理器的效率和吞吐量

假设有两个指令序列 A 和 B，指令交错可以将指令序列 A 和 B 的执行步骤交错进行

单核处理器下的指令交错不涉及真正的并行计算！

<br>

**多核指令交错**

多核指令交错是指在多核处理器中，不同核心的指令可以交错执行，从而利用多核处理器的并行性能够提高整体的计算速度。

三种交错实现的方式

1. 线程级指令交错（Thread-Level Instruction Interleaving）：在多核处理器上同时执行多个线程的指令。不同线程的指令可以交错执行。
2. 指令级指令交错（Instruction-Level Instruction Interleaving）：在单个核心上同时执行多条指令的部分步骤。通过指令级并行性（ILP）的技术，如流水线、乱序执行。
3. 数据级指令交错（Data-Level Instruction Interleaving）：在多核处理器上同时执行多个数据操作的指令。不同数据操作的指令可以交错执行。

<br>

### CAS

**基本知识**

`CAS（Compare-and-Swap）`是一种并发编程中常用的原子操作，用于实现无锁算法和线程同步。

CAS 操作包含三个操作数：内存位置（或称为地址），期望的值和新值。它的执行过程如下：

- 比较内存位置的当前值与期望的值是否相等。
- 如果相等，则将新值写入内存位置。
- 如果不相等，则说明其他线程已经修改了内存位置的值，操作失败。

<br>

**存在问题**

ABA 问题：CAS 操作无法解决 ABA 问题，即在操作过程中，数据的值经历了从 A 到 B 再到 A 的变化，但 CAS 操作仍然返回成功。为了解决 ABA 问题，可以使用版本号或标记位等机制进行扩展。

自旋开销：如果 CAS 操作失败，线程会一直进行自旋尝试，直到成功为止。在高并发情况下，自旋可能会消耗大量的 CPU 时间，造成性能下降。

只能针对一个变量：CAS 操作只能针对一个内存位置进行操作，无法原子地操作多个变量。

<br>

**CAS 操作的底层原理**

原子比较：CAS 操作首先会比较内存位置的当前值与期望的值是否相等。这个比较是在处理器的硬件层面完成的。

原子交换：如果比较相等，CAS 操作会使用新值替换内存位置的当前值。这个替换也是在处理器的硬件层面完成的。

内存屏障：CAS 操作还会使用内存屏障（Memory Barrier）来确保对内存的访问顺序和可见性。内存屏障是一种硬件指令，用于控制指令序列的执行顺序和内存操作的可见性。

<br>

**原子操作无锁案例**

```java
import java.util.concurrent.atomic.AtomicInteger;

public class AtomicCounter {
    private static AtomicInteger counter = new AtomicInteger(0);

    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(AtomicCounter::incrementCounter);
        Thread t2 = new Thread(AtomicCounter::incrementCounter);

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        System.out.println("Counter value: " + counter.get());
    }

    public static void incrementCounter() {
        for (int i = 0; i < 100000; i++) {
            counter.incrementAndGet(); // 使用incrementAndGet进行原子加操作
        }
    }
}
```

<br>

**CAS 与 volatile 的比较**

CAS 是基于乐观锁的思想  
synchronized 是基于悲观锁的思想  
CAS 体现的是无锁并发、无阻塞并发

<br>

### 原子整数

```java
AtomicInteger i = new AtomicInteger(0);
// 获取并自增（i = 0, 结果 i = 1, 返回 0），类似于 i++
System.out.println(i.getAndIncrement());

// 自增并获取（i = 1, 结果 i = 2, 返回 2），类似于 ++i
System.out.println(i.incrementAndGet());

// 自减并获取（i = 2, 结果 i = 1, 返回 1），类似于 --i
System.out.println(i.decrementAndGet());

// 获取并自减（i = 1, 结果 i = 0, 返回 1），类似于 i--
System.out.println(i.getAndDecrement());

// 获取并加值（i = 0, 结果 i = 5, 返回 0）
System.out.println(i.getAndAdd(5));

// 加值并获取（i = 5, 结果 i = 0, 返回 0）
System.out.println(i.addAndGet(-5));

// 获取并更新（i = 0, p 为 i 的当前值, 结果 i = -2, 返回 0）
// 其中函数中的操作能保证原子，但函数需要无副作用
System.out.println(i.getAndUpdate(p -> p - 2));

// 更新并获取（i = -2, p 为 i 的当前值, 结果 i = 0, 返回 0）
// 其中函数中的操作能保证原子，但函数需要无副作用
System.out.println(i.updateAndGet(p -> p + 2));

// 获取并计算（i = 0, p 为 i 的当前值, x 为参数1, 结果 i = 10, 返回 0）
// 其中函数中的操作能保证原子，但函数需要无副作用
// getAndUpdate 如果在 lambda 中引用了外部的局部变量，要保证该局部变量是 final 的
// getAndAccumulate 可以通过 参数1 来引用外部的局部变量，但因为其不在 lambda 中因此不必是 final
System.out.println(i.getAndAccumulate(10, (p, x) -> p + x));

// 计算并获取（i = 10, p 为 i 的当前值, x 为参数1, 结果 i = 0, 返回 0）
// 其中函数中的操作能保证原子，但函数需要无副作用
System.out.println(i.accumulateAndGet(-10, (p, x) -> p + x));
```

<br>

### 原子引用

原子引用是指在多线程环境下，可以原子性地更新对象引用的操作。在 Java 中，可以使用`AtomicReference`类来实现原子引用的操作。

`AtomicReference`类提供了以下常用的方法：

- `get()`：获取当前的对象引用。
- `set(T newValue)`：设置新的对象引用。
- `getAndSet(T newValue)`：获取当前的对象引用并设置新的对象引用。
- `compareAndSet(T expect, T update)`：如果当前的对象引用与期望值相等，则将其更新为新的对象引用，并返回更新操作的结果（true 表示更新成功，false 表示更新失败）。
- `weakCompareAndSet(T expect, T update)`：与`compareAndSet`类似，但是使用了弱比较和设置，对于某些平台和场景下的性能优化更有效。

以下是一个简单的示例，演示了如何使用`AtomicReference`进行原子引用操作：

```java
import java.util.concurrent.atomic.AtomicReference;

public class AtomicReferenceExample {
    private static AtomicReference<String> message = new AtomicReference<>("Hello");

    public static void main(String[] args) {
        System.out.println("Initial message: " + message.get());

        Thread t1 = new Thread(() -> {
            message.set("Thread 1 updated the message");
        });

        Thread t2 = new Thread(() -> {
            message.set("Thread 2 updated the message");
        });

        t1.start();
        t2.start();

        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("Final message: " + message.get());
    }
}
```

通过使用`AtomicReference`类，我们可以在多线程环境下原子性地更新对象引用，避免了竞态条件和数据不一致的问题。

<br>

### 解决 ABA 问题

ABA 问题是指在多线程环境下，一个共享变量的值从 A 变为 B，然后再变回 A，而在这个过程中有其他线程对该共享变量进行了操作，可能导致出现意外的结果

`AtomicStampedReference` 和 `AtomicMarkableReference`，在原子引用的基础上增加了对引用的版本信息或标记信息的管理，便于根据这些标记判断是否发生 ABA

```java
import java.util.concurrent.atomic.AtomicStampedReference;

public class ABAProblemSolution {
    private static AtomicStampedReference<Integer> value = new AtomicStampedReference<>(100, 0);

    public static void main(String[] args) {
        Thread t1 = new Thread(() -> {
            int stamp = value.getStamp();
            Integer reference = value.getReference();
            System.out.println("Thread 1: Value = " + reference + ", Stamp = " + stamp);

            // 模拟延迟
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            value.compareAndSet(reference, 200, stamp, stamp + 1);
            System.out.println("Thread 1: Value updated to 200");
        });

        Thread t2 = new Thread(() -> {
            int stamp = value.getStamp();
            Integer reference = value.getReference();
            System.out.println("Thread 2: Value = " + reference + ", Stamp = " + stamp);

            value.compareAndSet(reference, 150, stamp, stamp + 1);
            System.out.println("Thread 2: Value updated to 150");

            stamp = value.getStamp();
            reference = value.getReference();
            System.out.println("Thread 2: Value = " + reference + ", Stamp = " + stamp);

            value.compareAndSet(reference, 100, stamp, stamp + 1);
            System.out.println("Thread 2: Value updated to 100");
        });

        t1.start();
        t2.start();

        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        int finalStamp = value.getStamp();
        Integer finalValue = value.getReference();
        System.out.println("Final value: " + finalValue + ", Stamp: " + finalStamp);
    }
}
```

<br>

### 原子数组

原子数组和原子整数的操作差不多，你没必要背这些 API，背了也是忘掉，你会面试就好了懂了吗大哥

下面使用原子数组模拟多线程对计数器自增

```java
import java.util.concurrent.atomic.AtomicIntegerArray;

public class AtomicArrayExample {
    private static AtomicIntegerArray counterArray = new AtomicIntegerArray(5);

    public static void main(String[] args) {
        Thread[] threads = new Thread[5];

        for (int i = 0; i < 5; i++) {
            final int index = i;
            threads[i] = new Thread(() -> {
                for (int j = 0; j < 1000; j++) {
                    counterArray.incrementAndGet(index);
                }
            });
            threads[i].start();
        }

        for (int i = 0; i < 5; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (int i = 0; i < 5; i++) {
            System.out.println("Counter at index " + i + ": " + counterArray.get(i));
        }
    }
}
```

<br>

### 字段更新器

`字段更新器（FieldUpdater）`是 Java 中用于对特定类的字段进行原子更新的工具类。

```java
import java.util.concurrent.atomic.AtomicIntegerFieldUpdater;

public class FieldUpdaterExample {
    private static class Data {
        private volatile int value;
    }

    public static void main(String[] args) {
        AtomicIntegerFieldUpdater<Data> updater = AtomicIntegerFieldUpdater.newUpdater(Data.class, "value");

        Data data = new Data();
        updater.set(data, 10);
        System.out.println("Initial value: " + updater.get(data));

        boolean updated = updater.compareAndSet(data, 10, 20);
        System.out.println("Value updated: " + updated);
        System.out.println("Updated value: " + updater.get(data));

        int newValue = updater.getAndIncrement(data);
        System.out.println("Incremented value: " + newValue);
        System.out.println("Current value: " + updater.get(data));
    }
}
```

<br>

### 原子累加器

`原子累加器（Atomic Accumulator）`是一种用于在多线程环境下进行原子累加操作的数据结构。它提供了一种线程安全的方式来对数值进行累加，并保证了操作的原子性和数据的一致性。

`LongAccumulator`：原子长整型累加器类，用于原子累加长整型数值。

- void accumulate(long x)：将指定的长整型数值累加到当前值上。
- long get()：获取当前累加器的值。
  void reset()：重置累加器的值为初始值。

<br>

### Unsafe

<br>

`Unsafe` 对象提供了非常底层的，操作内存、线程的方法，Unsafe 对象不能直接调用，只能通过反射获得

```java
public class UnsafeAccessor {
    static Unsafe unsafe;

    static {
        try {
            Field theUnsafe = Unsafe.class.getDeclaredField("theUnsafe");
            theUnsafe.setAccessible(true);
            unsafe = (Unsafe) theUnsafe.get(null);
        } catch (NoSuchFieldException | IllegalAccessException e) {
            throw new Error(e);
        }
    }

    static Unsafe getUnsafe() {
        return unsafe;
    }
}
```

<br>

## 共享模型-不可变

<br>
