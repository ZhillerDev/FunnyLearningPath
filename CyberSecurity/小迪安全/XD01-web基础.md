### 访问网站的方式

访问网站有两种方式：

1. URL 访问：直接定位到服务器指定文件夹内部
2. IP 访问：定位到服务器文件夹的根目录

<br>

譬如我们实验 phpstudy 搭建服务器，则网站文件均存储在 WWW 文件夹下，如果直接使用 IP 地址访问，当前目录即为 WWW 文件夹；而使用 URL 访问，那么就会定位到指定的文件夹内！

<br>

### 常见的 DNS 安全攻击有哪些？

1. 缓存投毒：它是利用虚假 Internet 地址替换掉域名系统表中的地址，进而制造破坏。
2. DNS 劫持：是指在劫持的网络范围内拦截域名解析的请求，分析请求的域名，把审查范围以外的请求放行，否则返回假的 IP 地址或者什么都不做使请求失去响应，其效果就是对特定的网络不能访问或访问的是假网址。（针对面较广）
3. 域名劫持：域名劫持就是在劫持的网络范围内拦截域名解析的请求，分析请求的域名，把审查范围以外的请求放行，否则直接返回假的 IP 地址或者什么也不做使得请求失去响应，其效果就是对特定的网址不能访问或访问的是假网址。（针对面窄一点）
4. DNS DDOS 攻击：通过控制大批僵尸网络利用真实 DNS 协议栈发起大量域名查询请求，利用工具软件伪造源 IP 发送海量 DNS 查询，发送海量 DNS 查询报文导致网络带宽耗尽而无法传送正常 DNS 查询请求。

<br>

### request 请求数据包格式

#### request headers

```
POST /adduser HTTP/1.1
Host: localhost:8030
Connection: keep-alive
Content-Length: 16
Pragma: no-cache
Cache-Control: no-cache
Origin: chrome-extension://fdmmgilgnpjigdojojpjoooidkmcomcm
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko)
Chrome/66.0.3359.181 Safari/537.36
Content-Type: application/x-www-form-urlencoded
Accept: */*
Accept-Encoding: gzip, deflate, br
Accept-Language: zh-CN,zh;q=0.9
```

<br>

#### 请求行

```
HTTP 规划定义了 8 种可能的请求方法：
GET：检索 URL 中标识资源的一个简单请求
HEAD：与 GET 方法相同，服务器只返回状态行和头标，并不返回请求文档
POST：服务器接受被写入客户端输出流中的数据的请求
PUT：服务器保存请求数据作为指定 URL 新内容的请求
DELETE：服务器删除 URL 中命令的资源的请求
OPTIONS：关于服务器支持的请求方法信息的请求
TRACE：web 服务器反馈 Http 请求和其头标的请求
CONNECT ：已文档化，但当前未实现的一个方法，预留做隧道处理
```

<br>

#### 请求头

```
HOST: 主机或域名地址
Accept：指浏览器或其他客户可以接爱的 MIME 文件格式。Servlet 可以根据它判断并返回适当的文件格
式。
User-Agent：是客户浏览器名称
Host：对应网址 URL 中的 Web 名称和端口号。
Accept-Langeuage：指出浏览器可以接受的语言种类，如 en 或 en-us，指英语。
connection：用来告诉服务器是否可以维持固定的 HTTP 连接。http 是无连接的，HTTP/1.1 使用 Keep-Alive
为默认值
Cookie：浏览器用这个属性向服务器发送 Cookie。Cookie 是在浏览器中寄存的小型数据体，它可以记载
和服务器相关的用户信息，也可以用来实现会话功能。
Referer ： 表 明 产 生 请 求 的 网 页 URL 。 如 比 从 网 页 /icconcept/index.jsp 中 点 击 一 个 链 接 到 网 页
Content-Type：用来表名 request 的内容类型。可以用 HttpServletRequest 的 getContentType()方法取得。
Accept-Charset：指出浏览器可以接受的字符编码。英文浏览器的默认值是 ISO-8859-1.
Accept-Encoding：指出浏览器可以接受的编码方式。编码方式不同于文件格式，它是为了压缩文件并加
速文件传递速度。浏览器在接收到 Web 响应之后先解码，然后再检查文件格式。
```

<br>

#### HTTP 响应码

```
1xx：信息，请求收到，继续处理
2xx：成功，行为被成功地接受、理解和采纳
3xx：重定向，为了完成请求，必须进一步执行的动作
4xx：客户端错误
5xx：服务器错
```

<br>

### 操作系统与数据库

识别操作系统常见方法

1. 看字母大小写，windows 对大小写不敏感，Linux 敏感
2. ping 对方主机后看 TTL 的值：  
   UNIX TTL=256  
   LINUX TTL=64  
   WINDOWS98 TTL=32  
   WINDOWS7 TTL=64
3. nmap -O ip

<br>

识别数据库类型常见方法

1. nmap -O ip
2. nmap ip -p 端口，通过端口开放反推数据库

<br>

服务器端口

1. 关系型数据库 --MySQL:3306 --SqlServer:1433 --Oracle:1521
2. NOSQL 数据库 --MongoDB:27017 --Redis:6379 --memcached:11211

<br>

### CDN 相关

几种基本判断方式

1. 站长工具看响应时间：http://tool.chinaz.com/speedtest
2. nslookup ip：运行此代码后观察是否出现多个域名，如果有多个则表示存在 CDN
3. ping ip：如果时间跳变很大，则表示有 CDN

<br>

绕过 CDN 方法

1. 子域名查询：有的网站主域名会做 CDN，但是子域名可能不会做
2. 邮件服务查询：我们访问别人，可能通过 CND，但别人访问我们通常不会走 CDN
3. 国外地址请求：国外没有 cdn 节点的话，可能直接走原 ip
4. dns 历史记录，以量打量：CDN 节点是有流量上限的，用光之后就会直通原机，这也是一种流量攻击

<br>

### C 端与旁注

旁注：同一服务器下有 N 多个站点，渗透其中的一个获取 webshell，一举攻破所有的

c 端：不同服务器下的不同站点

<br>
