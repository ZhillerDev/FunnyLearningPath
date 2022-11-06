### 基本使用

#### Get/Post

我们可以通过简单的注释，来简化 GET/POST 请求

代码解释

1. @GetMapping，括号内填 GET 请求的路径，其中的花括号包裹着占位符
2. @PathVariable(value = "id")表示映射当前形参为占位符“id”，即当我们请求 url 是把占位符内容取出填到形参位置上
3. 如果形参名称和占位符名称一致，那么无需设置 value，REST 会自动帮我们映射好

```java
@RestController
public class MyRestController {

    // 可以使用value明确指定填充的内容
    @GetMapping("/std/{id}")
    public String queryStd(@PathVariable(value = "id") Integer id) {
        return "查询到的学生id" + id.toString();
    }

    // 如果形参名和mapping中的变量名一致，就无须设置value
    @PostMapping("/std/{name}/{age}")
    public String setStd(
            @PathVariable String name,
            @PathVariable Integer age
    ) {
        return "添加了新的" + name + " " + age;
    }
}
```

<br>

#### 尝试使用 PUT

因为一般浏览器都会禁止 PUT/DELETE 请求方式的使用，但是我们可以通过球盖 input 标签的代码来绕过这一限制

通过 POST 请求模拟 PUT 请求

在网页键入如下代码，则当我们点击 submit 按钮时，是使用 post 的方法发出了 put 请求；

> 此时我们再在后端编写 PutMapping 也可以正常接收了！！！

```html
<form action="std/put" method="post">
  <!-- 下面一行代码类似于注解，表示下下一行的input发出的信息我put请求 -->
  <input type="hidden" name="_method" value="put" />
  <input type="submit" value="put请求方式" />
</form>
```

<br>
