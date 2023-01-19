### 快速配置跨域

定义：当域名、端口、协议三个中任意一个不一致，则为跨域；

<br>

#### 配置中间件

新建文件 `middleware/cors.go`

```go
package middleware

import (
	"github.com/gin-gonic/gin"
)

func CORSSetting() gin.HandlerFunc {
	return func(context *gin.Context) {
		// 不检测跨域（因为设置origin值为*，则表示运行所有网页之间的请求）
		context.Writer.Header().Add("Access-Control-Allow-Origin", "*")
		// 设置预先检验的有效期为36000秒
		context.Writer.Header().Set("Access-Control-Max-Age", "36000")
		// 设置允许请求的方法
		context.Writer.Header().Set("Access-Control-Allow-Methods", "POST, GET, OPTIONS, PUT, DELETE, UPDATE, PATCH")
		// 设置允许请求的 Header
		context.Writer.Header().Set("Access-Control-Allow-Headers", "Content-Type, Content-Length")
		// 设置允许通过的其他额外的，即我们自定义的方法，比如我们这里自定义的 Habit属性
		context.Writer.Header().Set("Access-Control-Expose-Headers", "Habits")
		// 设置是否可以带认证信息
		context.Writer.Header().Set("Access-Control-Allow-Credentials", "true")

        // 由于OPTIONS请求方法的特性，我们需要对其进行额外判定并拦截掉
		if context.Request.Method == "OPTIONS" {
            // 返回header
			context.JSON(200, context.Request.Header)
            // abort拦截掉后续的请求
			context.Abort()
		} else {
			context.Next()
		}
	}
}
```

<br>

#### 引用中间件

```go
func main() {
    // 默认中间件形式实例化路由
	router := gin.Default()

    // 使用中间件
	router.Use(middleware.CORSSetting())

    // 配置一个空的（留给中间件内处理），请求方式为OPTIONS的请求，
	router.OPTIONS("/opt", func(context *gin.Context) {})

    // 后端服务器运行端口
	router.Run(":10001")
}
```

<br>

### 跨域细分

END 目前还没想到更好的细分方法。。。
