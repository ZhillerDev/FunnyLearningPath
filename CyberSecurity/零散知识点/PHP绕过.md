### php://filter

> 一种特殊的 php 语法，一般用来窃取源码  
> 通过不同筛选器来读写内容，与此同时还可进行加密解密

#### base64 加密

`php://filter/read=convert.base64-encode/resource=/var/www/html/flag.php`

完整含义：

1. php://filter/read 表示这是一个输入（读取）流
2. convert.base64-encode/resource 表示对内容进行 base64 加密处理
3. /var/www/html/flag.php 表示最终需要读取的文件路径

所以最终读出 flag.php 是加密过后的，需要使用 base64 执行解密

<br>

### php://input

模拟输入，比如我们构造如下载荷：  
`?code=php://input`

使用 apifox 的 POST 请求，请求体选择 raw（或者其他格式），传入内容 `helloworld`

后端 php 接收到的参数 code 的内容就是 “helloworld”

> php 有一个专门的函数读取参数内容，该函数是：file_get_contents

<br>
