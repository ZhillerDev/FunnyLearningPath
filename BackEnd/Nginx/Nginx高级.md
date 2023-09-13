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

sticky 模块保持

使用 yum 下载第三方模块 sticky，利用它生成具有一定生命周期的 cookie 下发给客户端，客户端再携带此 cookie 请求 nginx

```conf
upstream name{
    sticky name=ck expires=6h;
    server xxx;
    server xxx;
}
```

<br>
