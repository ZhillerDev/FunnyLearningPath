## Sentinel

<br>

### 介绍

Sentinel 是一个开源的分布式系统，用于监控和管理其他系统的可用性和状态。

Sentinel 主要用于监控 Redis 实例的健康状况，并在主从复制架构中自动进行故障检测和故障转移。当主节点发生故障或不可用时，Sentinel 会自动选举新的主节点，并将客户端的请求重定向到新的主节点，以确保系统的可用性。

<br>

雪崩：微服务集群中的任意一个服务器挂了，导致依赖它的服务器全部挂掉（链式反应，一条链路上的所有服务器统统挂掉）

解决雪崩问题：

1. 超时处理：设定超时时间，一旦超时无响应就返回错误信息，不会阻塞等待
2. 仓壁模式（线程隔离模式）：限定每个业务可调用线程数，避免耗尽 tomcat
3. 熔断降级：断路器统计业务中的异常比例，超出阈值直接断开，拦截一切请求
4. 流量控制：限制服务器访问的 QPS

<br>

### 限流准则

![](../img/cloud-hm-senior/s1.png)

簇点链路：链路监控对应的接口，而这些接口就称为一个资源；可以对这些接口执行限流，比如限制 QPS

<br>

![](../img/cloud-hm-senior/s2.png)

流控模式及其使用场景

关联模式：用户支付会触发修改和查询行为，二者同时竞争数据库锁，容易发生竞态条件，所以触发修改业务时需要关联的对查询业务进行限流  
关联模式使用场景：两个竞争关系的资源；一个高优先级，一个低优先级；

链路模式：查询订单和创建订单均需要 URL 跳转到查询商品业务，很显然我们需要优先保证创建订单任务，故对查询订单跳转执行限流

<brs>

![](../img/cloud-hm-senior/s3.png)

流控效果

warm-up 启动方式可以缓解冷启动效果  
初始 QPS 阈值会逐渐在指定时间内增加到最大 QPS 阈值，然后超出此 QPS 的请求均被丢弃

<br>

热点参数限流：针对统一参数的请求数量进行统计，以便执行限流

<br>

### FeignClient 限流处理

springcloud 使用 feign 进行限流和降级操作，故客户端需要配置 feign 依赖  
在配置文件内使用下方代码来开启 sentinel

```yaml
feign:
  sentinel:
    enable: true
```

FeignClient 失败后存在以下两种可选处理模式：

1. Fallbackclass：无法处理远程调用异常
2. FallbackFactory：可处理远程调用异常

<br>

下面是使用 FallbackFactory 的一般步骤：

1. 创建一个实现 FallbackFactory 接口的类，该类将负责创建回退逻辑。例如：

```java
import feign.hystrix.FallbackFactory;

public class MyFallbackFactory implements FallbackFactory<MyServiceClient> {

    @Override
    public MyServiceClient create(Throwable cause) {
        return new MyServiceClientFallback(cause);
    }
}
```

2. 创建一个实现远程服务接口的回退类，该类将实现在发生错误或超时时执行的回退逻辑。例如：

```java
public class MyServiceClientFallback implements MyServiceClient {

    private final Throwable cause;

    public MyServiceClientFallback(Throwable cause) {
        this.cause = cause;
    }

    @Override
    public String remoteServiceMethod() {
        // 执行回退逻辑，返回默认值或进行其他处理
        return "Fallback value";
    }
}
```

3. 在使用 Feign 客户端的地方，将 FallbackFactory 与 Feign 客户端接口关联起来。例如：

```java
@FeignClient(name = "my-service", fallbackFactory = MyFallbackFactory.class)
public interface MyServiceClient {

    @GetMapping("/remote-service-method")
    String remoteServiceMethod();
}
```

在上述示例中，当远程服务调用发生错误或超时时，将使用 MyFallbackFactory 创建的回退逻辑执行。

<br>

### 线程隔离

![](../img/cloud-hm-senior/s4.png)

线程隔离分为两种，分别对应上图左侧和右侧模式

1. 线程池：不同请求分配到不同的 feignclient
2. 信号量模式：每次处理一次请求就消耗一个信号量，消耗完毕就阻塞拦截，直到新的信号量被补充进去

信号量模式虽然为轻量级，且支持高频与高扇出，但不支持异步

执行流控时选择了“线程数”，实际上时开启了舱壁隔离的方式

<br>

### 熔断器

熔断器熔断触发由以下三种情况

1. 慢调用：业务响应时长超过阈值的被称为慢调用请求；若一定时间慢调用占比过高超出阈值，即可触发熔断
2. 异常比率与异常数：统计单位时间内调用数，若调用数超过指定数，且异常比例高于阈值，则触发熔断

<br>

### 授权规则

授权规则用来控制调用方来源，分为白名单和黑名单两个

我们可以把网关（gateway）地址放到白名单，只有从网关访问的来源才可被放行，此功能可以这样实现：

- 编写方法实现 RequestOriginParser 接口，并实现方法 parserOrigin 来设置请求头
- gateway 配置文件内设置 default-filters 中请求头为 origin

<br>

`BlockExceptionHandler` 是 Spring Cloud Gateway 提供的一个接口，用于处理由于限流或熔断而阻塞的请求。

该接口下有以下几个重要的场景，我们可以先实现 BlockExceptionHandler 接口，然后一次借助 instanceof 来判断这些场景，从而能对应的处理发送自定义异常

![](../img/cloud-hm-senior/s5.png)

<br>

### 规则持久化

> `Sentinel` 规则持久化是指将 Sentinel 的流控规则、降级规则、系统保护规则等配置信息保存到持久化存储中，以便在应用重启或动态更新规则时能够加载和使用这些规则。

sentinel 三种主要配置管理模式为

1. `原始模式`：直接存到内存

2. `pull 管理模式`：sentinel dashboard 更新规则后将其发送给 sentinel 客户端，客户端将规则存储到本地，每次按照此流程定时查询或者更新本地规则文件

3. `push 管理模式`：
   ![](../img/cloud-hm-senior/s6.png)

<br>

## 分布式事务

<br>

### 关键定理

**经典 ACID 原则**

ACID 是数据库事务的四个基本特性，它代表了一组重要的属性，确保事务的可靠性和一致性。下面是 ACID 原则的解释：

`原子性（Atomicity）`：事务是一个原子操作单元，要么全部执行成功，要么全部失败回滚。如果事务中的任何一个操作失败，整个事务将被回滚到初始状态，保证数据的一致性。

`一致性（Consistency）`：事务在执行前和执行后都必须保持数据库的一致性状态。这意味着事务的执行不会破坏数据库的完整性约束，如主键、外键、唯一性等。

`隔离性（Isolation）`：多个并发的事务在执行过程中彼此隔离，互不干扰。每个事务都应该感觉到它是在独立执行的，即使有其他事务在同时运行。隔离性防止了事务间的相互干扰，确保每个事务独立执行，避免数据的不一致性。

`持久性（Durability）`：一旦事务提交成功，对数据库的修改将永久保存，即使出现系统故障或断电等情况。持久性确保数据的持久性，并防止数据丢失。

<br>

**CAP 定理**

CAP 定理是一个关于分布式系统的理论定理，它指出在一个分布式系统中，不可能同时满足以下三个特性：

`一致性（Consistency）`：所有节点在同一时间具有相同的数据副本。在分布式系统中，一致性要求所有节点在任何给定时间点都具有相同的数据视图，即数据的更新操作需要在整个系统中立即生效。

`可用性（Availability）`：每个非故障节点在合理的时间范围内都能对请求做出响应。可用性要求系统能够在用户发起请求时始终保持响应，即系统对外部请求具有高可用性。

`分区容错性（Partition tolerance）`：系统在面对网络分区（节点之间的通信故障）时仍然能够继续运行。分区容错性是指系统能够处理节点之间的通信失败或网络分区的情况，保证系统的正常运行。

<br>

**BASE 定理**

BASE 是指以下三个特性：

`基本可用（Basically Available）`：系统保证在面对异常情况下仍然保持基本的可用性，即系统仍能够对外提供服务并响应请求，尽管可能会发生部分功能的降级。

`软状态（Soft State）`：系统中的状态可能随时发生变化，这种状态的变化并不是瞬间完成的，而是经过一段时间的过渡阶段。在分布式系统中，由于网络延迟、异步复制等因素，不同节点之间的数据可能存在稍微的不一致。

`最终一致性（Eventual Consistency）`：系统在一段时间后，最终会达到一致的状态。即系统保证最终所有副本或节点的数据是一致的，但在一段时间内可能存在部分不一致的情况。

<br>

**两种模式**

AP 模式：各事务各自为政，分别提交，结果不一致后逐渐弥补，最终实现一致  
CP 模式：各事务相互等待，同时提交，同时回滚，达成强一致（但等待过程事务属于弱可用状态）

<br>
