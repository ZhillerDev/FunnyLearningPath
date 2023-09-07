## 设计逻辑

### 全局唯一 ID

用户抢购优惠券时，会生成一个订单，但我们必须保证订单 ID 的生成并不具有很明显的规律，否则会泄露商品出售量信息；

ID 的组成部分：

- 符号位：1bit，永远为 0
- 时间戳：31bit，以秒为单位，可以使用 69 年
- 序列号：32bit，秒内的计数器，支持每秒产生 2^32 个不同 ID

<br>

故借助 redis 实现全局唯一 ID

```java
@Component
public class RedisIdWorker {
    /**
     * 开始时间戳
     */
    private static final long BEGIN_TIMESTAMP = 1640995200L;
    /**
     * 序列号的位数
     */
    private static final int COUNT_BITS = 32;

    private StringRedisTemplate stringRedisTemplate;

    public RedisIdWorker(StringRedisTemplate stringRedisTemplate) {
        this.stringRedisTemplate = stringRedisTemplate;
    }

    public long nextId(String keyPrefix) {
        // 1.生成时间戳
        LocalDateTime now = LocalDateTime.now();
        long nowSecond = now.toEpochSecond(ZoneOffset.UTC);
        long timestamp = nowSecond - BEGIN_TIMESTAMP;

        // 2.生成序列号
        // 2.1.获取当前日期，精确到天
        String date = now.format(DateTimeFormatter.ofPattern("yyyy:MM:dd"));
        // 2.2.自增长
        long count = stringRedisTemplate.opsForValue().increment("icr:" + keyPrefix + ":" + date);

        // 3.拼接并返回
        return timestamp << COUNT_BITS | count;
    }
}
```

### CountDownLatch

CountDownLatch 是 Java 编程语言中的一种同步辅助工具。它允许一个或多个线程等待一组操作完成，然后再继续执行。CountDownLatch 通过一个计数器来实现，计数器的初始值可以设定为一个正整数，表示需要等待的操作数量。

CountDownLatch 的主要方法是`countDown()`和`await()`。当一个线程完成了一个操作，可以调用`countDown()`方法将计数器减 1。而其他线程可以调用`await()`方法来等待计数器的值达到零，一旦计数器为零，所有等待的线程将被释放，可以继续执行。

下面是一个简单的示例，说明了 CountDownLatch 的使用方式：

```java
import java.util.concurrent.CountDownLatch;

public class CountDownLatchExample {
    public static void main(String[] args) throws InterruptedException {
        // 创建一个计数器，初始值为3
        CountDownLatch latch = new CountDownLatch(3);

        // 创建三个线程进行操作
        Worker worker1 = new Worker(latch, "Worker 1");
        Worker worker2 = new Worker(latch, "Worker 2");
        Worker worker3 = new Worker(latch, "Worker 3");

        // 启动三个线程
        worker1.start();
        worker2.start();
        worker3.start();

        // 等待所有操作完成
        latch.await();

        // 所有操作完成后执行其他操作
        System.out.println("All workers have completed their tasks.");
    }
}

class Worker extends Thread {
    private final CountDownLatch latch;

    public Worker(CountDownLatch latch, String name) {
        super(name);
        this.latch = latch;
    }

    public void run() {
        System.out.println(getName() + " is performing task.");
        // 模拟任务执行时间
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println(getName() + " has completed task.");
        latch.countDown(); // 计数器减1
    }
}
```

在上面的示例中，创建了一个初始值为 3 的 CountDownLatch 对象。然后创建了三个 Worker 线程，每个线程执行一个任务，并在任务完成后调用`countDown()`方法减少计数器的值。最后，主线程调用`await()`方法等待所有操作完成。

<br>

### 特价券

对于普通券，不需要任何限制，随时随地都可以领取而且没有数量限制；

而特价券需要添加两个限制：活动时间段和券数量

用户点击抢购领券时，需要检测活动是否结束，以及库存量是否足够；

<br>

### 库存超售问题

解决逻辑：
设置一个版本号 version=1 存到数据库；  
线程 1 操作后获取锁，令 version++；  
线程 2 发现 version=2，无法获取锁，只能作罢；
