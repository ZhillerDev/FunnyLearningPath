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

### volatile

`volatile` 是一个关键字，用于声明变量。它的主要作用是确保多个线程之间对该变量的访问具有可见性和有序性。

当一个变量被声明为 volatile 时，它具有以下特性：

1. 可见性（Visibility）：当一个线程修改了一个 volatile 变量的值时，该变量的新值会立即被写入主内存（共享内存），而不是只保存在线程的本地缓存中。其他线程在读取该变量时，会从主内存中获取最新的值，而不是使用本地缓存的旧值。这确保了对该变量的修改对所有线程都是可见的。
2. 有序性（Ordering）：volatile 变量的读写操作会按照其在代码中出现的顺序进行排序，即保证了有序性。这意味着在一个线程中，对 volatile 变量的写操作先于后续的读操作，不会发生重排序。

> TIPS:请注意，volatile 不确保原子性，复杂环境下仍有可能发生竞态条件问题，需要利用 synchronized 或 lock 解决

<br>

###
