### Cookie

#### 基础设置

使用 setcookie 设置 cookie，这是他的七个参数的介绍：

1. 参数一：key
2. 参数二：value
3. 参数三：过期时间（单位为 s），如果填入的值小于 0 则表示删除该 cookie
4. 参数四：允许访问该 cookie 的 url，填单斜杠表示任意路径均可访问到
5. 参数五：允许访问的地址
6. 参数六：是否开启强制 https 才可访问 cookie，设置为 false 表示 http 也可访问
7. 参数七：是否开 httponly

```go
func (d DefaultController) Index(c *gin.Context) {
	c.SetCookie("username", "tom", 3600, "/", "localhost", false, true)
	d.success(c)
}
```

<br>

在其他中间件里面，直接使用 cookie 并提供 key 值就可以取出来了

```go
func (d DefaultController) Index(c *gin.Context) {
	username, _ := c.Cookie("username")
	fmt.Println(username)
}
```

<br>

#### 设置过期

很简单，仅需设置过期时间小于 0 即可！

<br>

#### samesite 问题

> 参考 MDN：https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Headers/Set-Cookie/SameSite

对于目前较新的浏览器，会针对 `set-cookie` 额外添加一个限定条件 `samesite`，目的是以此来限制跨域问题，阻止 CSRF

这是三种不同的 samesite 值及其对应的功能：

1. Lax：Cookies 允许与顶级导航一起发送，并将与第三方网站发起的 GET 请求一起发送
2. Strict：Cookies 只会在第一方上下文中发送，不会与第三方网站发起的请求一起发送
3. None：Cookie 将在所有上下文中发送，即允许跨站发送

<br>

当 samesite 为 lax 模式下，可以不用管 secure；  
当 samesite 为非 lax 模式，就必须设置 secure 的值为 true，此时仅允许 https 访问；

<br>

所以我们可以吧 setcookie 代码调整至以下最优化：

```go
func (d DefaultController) SetCookie(c *gin.Context) {
	// 开启samesite的None模式
	c.SetSameSite(http.SameSiteNoneMode)
	// 注意这里需要设置secure为true
	c.SetCookie("user", string(js), 3600, "/", "http://localhost:10001/", true, true)
	d.success(c)
}
```

<br>

#### cookie 传入 JSON

很简单，首先构建一个结构体并初始化它，然后把结构体转换为 JSON 格式就 ok 了

```go
type User struct {
	Name string `json:"name"`
	Age  int    `json:"age"`
}

func (d DefaultController) SetCookie(c *gin.Context) {
    // 实例化结构体
	u := User{
		Name: "tom",
		Age:  100,
	}
    // 结构体转换为JSON的内置方法，注意值传递
	js, _ := json.Marshal(&u)

	c.SetSameSite(http.SameSiteNoneMode)
	c.SetCookie("user", string(js), 3600, "/", "http://localhost:10001/", false, true)
	d.success(c)
}
```

<br>

#### 二级域名共享 cookie

譬如我现在注册了两个域名：`a.ikun.com` 以及 `b.ikun.com`  
并将他们都解析到同一个服务器 IP 上：`127.0.0.1`

我们要实现的目的是，在前一个域名下设置的 cookie 在另一个域名下依然可以访问，这个时候可以通过修改 `setcookie` 的 `domain` 属性值，来配置二级域名共享

```go
// 注意这里的domain值为.ikun.com
// 表示前面一个值是可变的，所以可以在以上展示的两个域名之间共享cookie
c.SetCookie("user", "demo", 3600, "/", ".ikun.com", false, true)
```

<br>

### Session

#### 单 session

> 可以去看 github 上对于该 session 依赖的详细介绍与安装步骤 https://github.com/gin-contrib/sessions

安装依赖 `go get github.com/gin-contrib/sessions`

一个最简单的 session 实例

```go
package main

import (
    // 必须引入的两个session包
  "github.com/gin-contrib/sessions"
  "github.com/gin-contrib/sessions/cookie"
  "github.com/gin-gonic/gin"
)

func main() {
  r := gin.Default()

  // 新建一个cookie类型的存储库
  store := cookie.NewStore([]byte("secret"))
  // 总路由插入中间件，注册存储库
  // 参数一为自定义的存储库名字，参数二为欲构建的存储库
  r.Use(sessions.Sessions("mysession", store))

  r.GET("/hello", func(c *gin.Context) {
    // 首先通过上下文获取默认存储库对象
    session := sessions.Default(c)

    // 使用get和set方法一键值对的方式操作session库中的数据
    // 特别注意，set数值完毕后一定要save保存，否则设置是无效的
    if session.Get("hello") != "world" {
      session.Set("hello", "world")
      session.Save()    // 记得保存
    }

    c.JSON(200, gin.H{"hello": session.Get("hello")})
  })
  r.Run(":8000")
}
```

> 现在基本不用 session 了，改用 JWT 进行验证，所以这一块直接丢了也没啥

<br>
