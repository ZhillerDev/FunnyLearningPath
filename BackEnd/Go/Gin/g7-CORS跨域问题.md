### Axios+Gin 跨域一条龙

> 前端使用 axios 请求，后端通过 gin 搭设，详解所有源码实现跨域自由！！！  
> 前端部分 axios 配置是封装好的完整代码，可以直接拿去用

<br>

#### 前端部分

安装 axios 啥的就不用我讲了吧？

首先创建文件 `interceptor.js` 用来配置 axios 拦截器以及初始化

特别特别注意！ `baseURL` 必须写请求协议 `http` ，否则必定报错！

```js
const server = axios.create({
  // 千万不要落了 http://
  baseURL: "http://localhost:10086/",
  timeout: 300 * 1000,
  withCredentials: true, // 这里是为了让cookie通过

  // 配置通用头
  headers: {
    "Content-Type": "application/x-www-form-urlencoded",
  },
});

// 请求拦截器
server.interceptors.request.use(
  (config) => {
    // 如果请求方法为GET，则字符串化params
    if (config.method === "get") {
      config.paramsSerializer = (params) => {
        return qs.stringify(params, {
          arrayFormat: "comma",
        });
      };
    }

    // 获取用户token并附带在headers里传递给后端
    const token = localStorage.token;
    if (token) {
      config.headers["token"] = token;
    }
    return config;
  },
  (err) => {
    Promise.reject(err);
  }
);

// 响应拦截器
server.interceptors.response.use(
  (config) => {
    return config;
  },
  (err) => {
    // 如果错误的响应体存在的话
    if (err.response) {
      // 获取响应值message
      const msg =
        err.response.data === null
          ? "服务端出错，无法获取响应内容"
          : err.response.data.message;

      // 根据响应状态码来返回对应信息
      switch (err.response.status) {
        case STATUS.success.id:
          ElMessage(STATUS.success.msg);
          break;
        default:
          if (msg === "invalid token") {
            ElMessage("登录状态过期，请重新登陆");
          } else {
            ElMessage(msg);
          }
          break;
      }
    }
    return Promise.reject(err);
  }
);

export default server;
```

<br>

然后封装一个 `http.js` 用于后续 axios 请求使用

```js
// 导入我们刚刚编写好的interceptor.js
import server from "@/api/interceptor.js";

const http = {
  // axios执行get请求
  get(url, params) {
    const config = {
      method: "get",
      url: url,
    };
    // 当存在params时，才会将其添加到config并发送给后端
    if (params) config.params = params;
    return server(config);
  },

  // axios执行post请求
  post(url, formdata) {
    const config = {
      method: "post",
      url: url,
    };
    if (formdata) config.data = formdata;
    return server(config);
  },
};

export default http;
```

<br>

#### 后端部分

**cors.go**

编写跨域处理中间件 `cors.go`

注意到 `Access-Control-Allow-Origin` 这里不像是其余帖子一以贯之的都是写 `*` ，是因为当我们在前端自定义 `cookie` 时，发送给后端的 `origin` 是变化的，我们必须取得前端的 origin 然后对其进行放行才可以！

我们在 `Access-Control-Allow-Headers` 这一条里面添加了 `token` ，是因为前端的 axios 请求拦截器会自动插入一个名叫 token 的请求头，如果我们在后端没有允许的话，那么依然请求失败！！！

```go
import (
	"fmt"
	"github.com/gin-gonic/gin"
)

func Cors() gin.HandlerFunc {
	return func(context *gin.Context) {
		// 允许 Origin 字段中的域发送请求
		context.Writer.Header().Add("Access-Control-Allow-Origin", context.GetHeader("origin"))
		// 设置预验请求有效期为 70000 秒
		context.Writer.Header().Set("Access-Control-Max-Age", "70000")
		// 设置允许请求的方法
		context.Writer.Header().Set("Access-Control-Allow-Methods", "POST, GET, OPTIONS, PUT, DELETE, UPDATE, PATCH")
		// 设置允许请求的 Header
		context.Writer.Header().Set("Access-Control-Allow-Headers", "Content-Type, Content-Length,token")
		context.Writer.Header().Set("Access-Control-Expose-Headers", "Content-Length, Access-Control-Allow-Headers,token")
		// 配置是否可以带认证信息
		context.Writer.Header().Set("Access-Control-Allow-Credentials", "true")

		// OPTIONS请求返回200
		if context.Request.Method == "OPTIONS" {
			fmt.Println(context.Request.Header)
			context.AbortWithStatus(200)
		} else {
			context.Next()
		}

		fmt.Println("开始跨域拦截")
	}
}
```

<br>

**main.go**

最后在 router 初始化后直接使用 use 插入中间件即可！

```go
func RouterInit() *gin.Engine {

	Router := gin.Default()

	// 建议在总路由初始化后立即插入cors中间件
	// 最好不要在别的地方添加！
	Router.Use(middleware.Cors())

	...

	return Router
}
```

<br>

#### 愉悦的测试环节

万事俱备只欠东风！

前端使用封装好的 http 快速请求后端接口，成功拿到数据！没有任何跨域报错！

```js
const axiosRequest = () => {
  http
    .get("/bk/test")
    .then((res) => {
      console.log(res);
    })
    .catch((err) => {
      console.log(err);
    });
};
```

<br>

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
