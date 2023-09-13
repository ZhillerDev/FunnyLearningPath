## 扩容

<br>

### 扩容方式

单机垂直扩容：增加硬件资源

水平扩容：集群化

颗粒度拆分：分布式

数据异构化：多级缓存机制，如客户端缓存与 CDN 缓存

服务异步化：拆分请求或者使用消息中间件

<br>

### 会话保持

#### ip_hash

nginx 采用负载均衡模式时，可以借助 ip_hash 来进行会话保持

下方代码表示每次 nginx 接收客户端请求时，都会将客户端 IP 进行 `hash` 计算后得出的结果与 server 进行匹配，匹配到哪一个后端服务器，那就让该客户端一直连接到该服务器上

```conf
upstream name{
    ip_hash;
    server xxx;
    server xxx;
}
```

<br>

#### 其他保持法

$request_url 根据请求的 url 转换成 hash 后转发到固定的后端服务器上

```conf
upstream name{
    hash &request_url;
    server xxx;
    server xxx;
}
```

$cookie_jsessionid 或者客户端拿到服务器下发的 cookie 后返回给服务器

```conf
upstream name{
    hash $cookie_jseesionid;
    server xxx;
    server xxx;
}
```

<br>

`sticky` 模块保持

sticky 实现上游集群服务器会话保持（专业术语牢牢记住哈）的流程：

1. 客户端首次请求 nginx，不带 cookie，nginx 轮询方式随机分配一个后端服务器
2. 后端服务器处理请求，返回给 nginx
3. nginx 封装与后端服务器对应的 cookie 字段返回给客户端（使用 sticky 插件）
4. 客户端接收并保存 cookie 字段，下次请求直接携带

```conf
upstream name{
    sticky name=ck expires=6h;
    server xxx;
    server xxx;
}
```

<br>
