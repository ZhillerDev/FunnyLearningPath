## 安装

### 文件准备

小皮面板（建议使用 V8 最新版，更加方便）：https://www.xp.cn/download.html

pikachu 靶场源文件：https://github.com/zhuifengshaonianhanlu/pikachu

<br>

> 靶场文件可以使用 ssh 快速 clone

### 安装配置

安装小皮面板；

打开面板，直接启动 apache 以及 mysql

![](../imgs/target/pikachu/pk1.png)

> 如果 mysql 无法启动，请查看是不是另一个数据库占用了 3306 端口，如果是，请在管理员模式下运行命令提示符，输入：  
> netstat -ano 找到占用 3306 端口的程序，获取其 PID  
> taskkill -f -pid xxx 根据 PID 强行杀死指定程序（没啥危害，直接杀死即可！）

<br>

此时在浏览器内输入 localhost 如果正确的显示了提示文本，那么表示服务器配置成功！

<br>

### 靶场设置

首先进入 pikachu 靶场根目录下的 inc 文件夹，找到 `config.inc.php` 配置文件  
修改 `DBPW` 字段为默认的数据库密码 root

```php
<?php
//全局session_start
session_start();
//全局居设置时区
date_default_timezone_set('Asia/Shanghai');
//全局设置默认字符
header('Content-type:text/html;charset=utf-8');
//定义数据库连接参数
define('DBHOST', '127.0.0.1');//将localhost或者127.0.0.1修改为数据库服务器的地址
define('DBUSER', 'root');//将root修改为连接mysql的用户名

// 在这里修改为默认的数据库密码root
define('DBPW', 'root');

define('DBNAME', 'pikachu');//自定义，建议不修改
define('DBPORT', '3306');//将3306修改为mysql的连接端口，默认tcp3306

?>
```

打开小皮面板根目录，找到其下 WWW 文件夹，把我们 clone 的 pikachu 靶场直接扔到这里面来！

然后重启 apache 服务器即可

网址输入 localhost/pikachu 访问我们的靶场，之后点击首页内的红字来进行靶场初始化，初始化完毕后即可正常使用我们的靶场啦！

<br>

## 流程全解

### 暴力破解

**基于表单的破解**

burp 拦截请求，修改$，发送到 intruder 里面，网 payloads 添加密码字典；

直接把账户名或者密码撞出来就行；

<br>

**客户端验证码绕过**

查看源码，发现是 js 算法随机生成验证码，然后前端验证输入的码是否匹配，如果匹配才允许发送请求给后端

先乱输一个用户名及密码，填入正确的验证码，使用 burp 拦截登陆请求；  
拦截到后，发送到 repeater

修改请求中的 vcode 字段，这是我们填写的验证码；  
发现无论我们把 vcode 瞎改成多少并请求多少次，都只会返回“用户名或者密码错误”，而没有验证码错误；

可见，后端没有做任何验证；

使用 intrude 进行爆破即可

> 下图可见，每次请求后验证码都会变，但是即便我们请求的验证码不匹配也无验证码报错！

![](../imgs/target/pikachu/pk2.png)

<br>
