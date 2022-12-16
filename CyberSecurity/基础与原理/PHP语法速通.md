### 变量

php 是弱类型语言，所以无需指定变量类型，他会自动推断

```php
<?php
$pig = "tom";
$age = 12;
echo $pig;
```

<br>

可变变量：`$$helloworld = 123`

<br>

外部变量：即 PHP 定义好的变量

```php
$_POST
$_GET
$_COOKIE
$_SESSION
```

<br>

### 常量与数据类型

使用键值对的形式定义，调用时直接写即可，无需使用美元符号；

```php
<?php
define('ham','香肠');
echo ham;
echo '这是一根：'.ham; // 字符串链接一个变量并输出的正确方式
```

<br>

除了单引号和双引号能声明字符串外，php 还有一个所谓“字界符”的概念：  
第一行使用三个小于号+任意大写英文字母；  
中间部分填任何字段；  
结尾定格写对应的大写英文字母并加上分号；

> 这是一种格式化输出的方式

```php
$zone = <<<WHAT
    this is a
    <b>ZONE</b>
    for
    text
WHAT;
echo $zone;
```

<br>

数据类型咨询：

1. gettype() 获取变量类型
2. var_dump() 获取变量类型及值
3. is_xxx 判断指定变量是否属于指定类型

```php
var_dump($zone);        // 直接输出
echo gettype($zone);    // 需要借助echo才能输出
echo is_array($zone);   // 判断数据类型
```

<br>

### 数组

定义数组非常简单，不限制任何数据类型，你甚至可以套娃

```php
<?php
$arr = array(123, 'github', false, 123.123, array(1, 2, 3));
echo $arr[0];       // 正常取值
echo $arr[4][0];    // 套娃
```

<br>

可以为指定值指派一个键，类似于键值对，懂吧？

```php
$arr2 = array(100=>'github');
echo $arr2[100];
```

<br>

### 文件操作
