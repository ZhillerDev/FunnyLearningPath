### JSON

> 直接将表单传入以请求后端， 无需指定参数类型

前端代码：

```js
this.axios
  .post("http://localhost:10086/login", this.form)
  .then((res) => {})
  .catch((err) => {});
```

后端代码：

使用 `@RequestBody` 接收即可（User 是一个 entity）

```java
@PostMapping("/login")
public String validateLogin(@RequestBody User user) {
    return userService.validateLoginService(user.getUsername(), user.getPassword());
}
```

<br>

### Form-data

为便于 apifox 或者 postman 的快捷请求，故推荐使用 formdata 的形式来请求后端

前端代码如下

```js
// 构造formdata对象并安装键值对的形式插入内容
// this.form表示收集到的表单内容
let dt = new FormData();
dt.append("username", this.form.username);
dt.append("password", this.form.password);

this.axios
  .post("http://localhost:10086/login", dt, {
    // 注意设置参数类型
    headers: { "Content-Type": "multipart/form-data" },
  })
  .then((res) => {})
  .catch((err) => {});
```

后端代码：

请使用 `@RequestParam` 而非 `@RequestBody`，以免接收不到请求

这里省略了 entity 和 service 具体细节，相信以你的能力处理应该不难

```java
@RestController
@CrossOrigin(originPatterns = "*")
public class UserController {

    @Resource
    private UserService userService;

    @PostMapping("/login")
    public String validateLogin(@RequestParam String username, @RequestParam String password) {
        return userService.validateLoginService(username, password);
    }
}
```

<br>
