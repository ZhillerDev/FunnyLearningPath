## Jedis

### 快速上手

intellij 新建 maven 工程，名字任意取；

为 pom.xml 文件添加依赖

```xml
<dependencies>
    <dependency>
        <groupId>redis.clients</groupId>
        <artifactId>jedis</artifactId>
        <version>3.2.0</version>
    </dependency>

    <dependency>
        <groupId>junit</groupId>
        <artifactId>junit</artifactId>
        <version>4.12</version>
        <scope>test</scope>
    </dependency>

</dependencies>
```

> 注：这里不使用高版本 redis，是因为高版本默认附带的 slf4j 会出现编译错误，而低版本附带的运行没问题

<br>

虚拟机上开启 redis 服务器，并确认链接网络后

编写测试类

```java
public class TestRedis {
    @Test
    public void redis1() {
        // 按照ip与端口链接redis服务器
        Jedis jedis = new Jedis("192.168.113.136", 6379);

        // 执行redis语法
        jedis.hset("hs", "a", "100");
        jedis.hset("hs", "b", "10293");
        jedis.hset("hs", "c", "123");

        Map<String, String> hs = jedis.hgetAll("hs");
        System.out.println(hs);

        // 最后关闭连接
        jedis.close();
    }
}
```

<br>

### Jedis 连接池

由于 jedis 是链接不安全的，所以最好使用 jedis 连接池而非简单的 jedis 链接

```java
package com.zhiller.jedis.util;

import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;

public class JedisUtil {
    private static final JedisPool jedisPool;

    static {
        JedisPoolConfig jedisPoolConfig = new JedisPoolConfig();
        jedisPoolConfig.setMaxIdle(8);
        jedisPoolConfig.setMaxTotal(8);
        jedisPoolConfig.setMinIdle(0);
        jedisPoolConfig.setMaxWaitMillis(200);

        jedisPool = new JedisPool(
                jedisPoolConfig,
                "192.168.113.144",
                8888,
                1000,
                "123456"
        );
    }

    public static Jedis getJedis() {
        return jedisPool.getResource();
    }
}
```

<br>

## SpringDataRedis
