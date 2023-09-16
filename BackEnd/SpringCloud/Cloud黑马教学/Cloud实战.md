## 远程服务调用

### RestTemplate

RestTemplate 可以模拟客户端来向另外一个后端执行请求

黑马给出的微服务项目中，有两个 boot 项目，分别是 order 与 user

我们想要在 order 中的 service 阶段对 user 给出的接口执行请求，此时就需要 RestTemplate 来进行解决

<br>

首先需要在 OrderApplication，也就是项目入口点注册一个全局的 RestTemplate  
后续可以使用自动装配直接调用该对象

```java
@MapperScan("cn.itcast.order.mapper")
@SpringBootApplication
public class OrderApplication {

    public static void main(String[] args) {
        SpringApplication.run(OrderApplication.class, args);
    }

    @Bean
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }
}
```

<br>

修改 `OrderService` 的代码，借助 `RestTemplate` 请求拿到 user 后，存储到对应的 orde 对象中去

```java
@Service
public class OrderService {

    @Autowired
    private OrderMapper orderMapper;

    // 自动装配引入resttemplate
    @Autowired
    private RestTemplate restTemplate;

    public Order queryOrderById(Long orderId) {
        // 1.查询订单
        Order order = orderMapper.findById(orderId);

        // 设置后端服务器请求的URL
        String url = "http://localhost:10087/user/" + order.getUserId();
        // 请求后端服务器
        User forObject = restTemplate.getForObject(url, User.class);
        // 将请求结果保存到现有的order对象中去
        order.setUser(forObject);

        // 4.返回
        return order;
    }
}
```

<br>

## Eureka

### 简要概念

服务调用关系

- 服务提供者：暴露接口给其他微服务调用
- 服务消费者：使用其他微服务提供的接口
- 提供者和消费者角色是相对的，一个对象可以同时兼具这两种状态

<br>

![](../img/cloud-hm/h1.png)

Eureka 完整工作流程图及其简要步骤

Eureka 由两部分组成：`Eureka-server` 与 `Eureka-client`
Eureka-client 又可划分为两个类别：服务消费者（consumer）与服务提供者（provider）

工作流程：

1. 服务提供者 URL 发送给注册中心进行注册
2. 注册中心来者不拒，一一把服务提供者的 URL 记录到表内
3. 服务消费者从注册中心获取对应提供者 URL，进行接口请求
4. 每隔 30s，服务提供者就会向注册中心发送心跳包来证明自己活着，否则隔一段时间不发的话就会被注册中心删除
5. 服务消费者具备负载均衡的能力，可以选择最适合的那一个后端服务执行请求

<br>

### 配置 Eureka 环境

> 注意，请确保所有项目的编译环境均为 JDK1.8，千万不要拉的太高，不然直接编译错误恶心死你，实测 17 及以上 JDK 直接崩溃

新建空的 maven 项目

pom 添加 eureka 依赖

```xml
<dependencies>
    <dependency>
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-netflix-eureka-server</artifactId>
    </dependency>
</dependencies>
```

配置入口文件

代码清单：`EurekaApplication.java`

```java
@EnableEurekaServer
@SpringBootApplication
public class EurekaApplication {
    public static void main(String[] args) {
        SpringApplication.run(EurekaApplication.class,args);
    }
}
```

编写配置文件（配置文件直接放到 resource 文件夹下面）

特别注意！！！配置项 `defaultZone` 必须是以小驼峰的形式书写，不能写成蛇形 `default-zone` 否则永远运行不了！！！

代码清单：`application.yaml`

```yaml
# eureka服务器执行的端口
server:
  port: 10001

# spring名称
spring:
  application:
    name: eurekaserver

# 设置eureka服务器的可视化管理平台的默认URL
eureka:
  client:
    service-url:
      defaultZone: http://localhost:10001/eureka
```

<br>

打开控制面板：`http://localhost:10001/`  
即可看到下方的效果

![](../img/cloud-hm/h2.png)

<br>

### 设置 Eureka Client

随意选择一个 springboot 项目（版本 2.7.15 及以下）

pom 内添加 `eureka-client` 依赖

```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
</dependency>
```

和设置 `eureka-server` 不同的是，我们无需再为入口文件顶部添加注解 `@EnableEurekaClient`，因为目前对于高版本 springboot 来说，eureka 已经自动帮我们完成了这一步骤，我们仅需设置配置文件就可以了！

配置文件设置好 spring 的名称，以及 eureka 配置即可

```yaml
server:
  port: 10087

spring:
  application:
    name: userserver

eureka:
  client:
    # 同样的，这里指向的是我们eureka-server的地址！
    service-url:
      defaultZone: http://127.0.0.1:10001/eureka
```

<br>

此时先重新构建 `eureka-server`，然后再构建当前项目，打开控制面板，即可看见目前存在了两个 `eureka-client`

<br>

### Eureka 服务发现

之前设置的远程服务调用 RestTemplate，是直接使用了硬编码，把 URL 写死了，现在我们配置好了 Eureka 服务器，那就可以使用负载均衡的原理，直接调用对应的后端服务器

打开 `OrderApplication.java` ，为对应的 RestTemplate 添加负载均衡注解

```java
@Bean
@LoadBalanced
public RestTemplate restTemplate() {
    return new RestTemplate();
}
```

然后修改之前的硬编码为我们设置的`服务提供者 user-server`对应的 URL 即可

```java
String url = "http://user-server/user/" + order.getUserId();
```

> 与此同时，这就完成了一个完整的 Eureka 工作流程
>
> 1. user-server 发送服务到注册中心 eureka-server
> 2. order-server 从注册中心发现服务，并取回使用

<br>

## Ribbon

### 工作流程

![](../img/cloud-hm/h3.png)

在配置完毕 eureka 后，我们发现，虽然可以在代码中使用诸如 `http://user-server/user/xxx` 的形式来访问到对应的后端服务器，但是当我们直接把这段 URL 填入浏览器时，发现无法访问

而 Ribbon 的作用，简单来说，就是帮助我们把浏览器 URL 转义为对应的地址，让我们顺利访问到后端服务器（有点 DNS 那味了）

上图展示了 Ribbon 工作的基本流程

1. 获取原始 URL，从中提取出 eureka-client 的 id
2. 以此 id 请求 eureka-server，拿到所有相关的服务器原地址
3. 借助 ribbon 负载均衡措施，选择一个地址并使用
4. 转译该地址，返回

<br>

### 配置与使用 Ribbon

![](../img/cloud-hm/h4.png)

上图给出了可供选择的 Ribbon 负载均衡策略

<br>

Ribbon 自身通过接口 IRule 来自定义对应的负载均衡策略

在 `OrderApplication.java` 随便注入一个规则

```java
@Bean
public IRule randomRule() {
    return new RandomRule();
}
```

> 定义负载均衡的方式也可以直接写在配置文件里面，这里不做演示，还是建议写在入口类，更加直观

> 入口类定义 IRule：全局作用；配置文件定义 IRule：局部作用

<br>

### 饥饿加载

ribbon 默认使用懒加载，所以当首次启用负载均衡时，会浪费很多时间实例化对象

在配置文件里面设置 ribbon 为饥饿式加载方法

```yaml
ribbon:
  eager-load:
    # 当需要对多个eureka-client实现同类型加载方式的话，使用数组的写法
    clients:
      - user-server
      - order-server
    enabled: true
```

<br>

###

## Nacos

### 下载 Nacos 并运行

首先下载对应的 release 包，主要要选择已经打包编译好的 nacos-server 文件！
https://github.com/alibaba/nacos/releases/tag/1.4.1

将其解压到任意一个不包含中文路径的文件夹下  
进入 bin，在此文件夹下打开命令行，输入 `startup.cmd -m standalone` 即可开启 nacos 服务器

默认服务器端口 8848，上述指令的`standalone`表示为单体，非集群模式

<br>

等待命令行运行一段时间后，进入 nacos 给出的 URL：`http://192.168.113.1:8848/nacos/index.html`

可见这是一个管理界面，用户名和密码默认都是 `nacos` 登录进去即可

<br>

### 配置 Nacos

由于我们目前的项目设置是父子类型，首先需要在父项目 pom 导入 alibaba 工具库

```xml
<dependency>
    <groupId>com.alibaba.cloud</groupId>
    <artifactId>spring-cloud-alibaba-dependencies</artifactId>
    <version>2.2.5.RELEASE</version>
    <type>pom</type>
    <scope>import</scope>
</dependency>
```

再在各自的 server 内导入 nacos-discovery 依赖

```xml
<dependency>
    <groupId>com.alibaba.cloud</groupId>
    <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
</dependency>
```

<br>

非常简单吧？

最后仅需在配置文件内设置好 nacos 服务器的地址即可  
和 eureka 不一样的是，这里的注册中心实际上就是我们刚刚运行的 nacos 服务器，所以无需额外创建项目来跑服务器了

配置文件书写 `application.yaml` ：

```yaml
spring:
  cloud:
    nacos:
      server-addr: localhost:8848
```

<br>

重新构建对应模块

回到 nacos 控制面板，就可以看见我们生成的服务器啦！

![](../img/cloud-hm/h5.png)

###

<br>

## Feign

## Gateway

## Docker
