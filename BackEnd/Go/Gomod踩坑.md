### 一次正确的配置

#### 结果文件结构

> 这是操作完毕后的完整文件结构树状图，接下来将带大家一步异步完成它

注意到，入口文件 `main.go` 和 `go.mod` 在同一目录下！这一点必须要保证！

```
└─gindemo.com
    │  go.mod
    │  go.sum
    │  main.go
    │
    ├─.idea
    │      .gitignore
    │      gindemo.com.iml
    │      modules.xml
    │      workspace.xml
    │
    └─src
        └─Chapter1
                C1.go
```

<br>

#### 创建项目

> 项目创建依赖 goland2020

任选一个文件夹创建目录，注意项目的名字最好依照这个格式： `xxx.xxx`  
点号的后缀随便写，只要保证上面这样的格式就好了

我们新建一个项目 `gindemo.com`

此时打开 goland 的设置页面，设置好 `GOROOT` `GOPATH`  
设置过程请参考 gin 相关文章！

<br>

新建文件 main.go （和 go.mod 同目录）

```go
// 入口点入口点！！！必须要main！！！别忘了求求你了
package main

import (
	"github.com/gin-gonic/gin"
	"net/http"
)

func main() {
	app := gin.Default()

	app.GET("/shit", func(context *gin.Context) {
		context.JSON(http.StatusOK, gin.H{
			"error": "shit",
		})
	})

	app.Run(":10086")
}
```

这是一个最简单的 gin 后端，因为我们之前配置好了 `GOPATH` ，所以这里 goland 会为我们自动导包，故不会出现飘红现象！

但实际上 `go.mod` 内部依然没有配置依赖，我们需要在项目目录下打开命令行，输入 `go mod tidy` 为我们自动处理好依赖关系；

此时配置好运行参数直接运行 main 方法就可以开启我们的后端服务了

<br>

#### 不同文件夹下导包

新建文件（大小写不是必要的） `src/Chapter1/C1.go`  
代码不是重点，我们要做的是在 `main.go` 里面使用该文件的 `BindDemo` 方法

方法名需要大写开头！

```go
package Chapter1

import (
	"fmt"
	"github.com/gin-gonic/gin"
	"net/http"
)

...

func BindDemo(c *gin.Context) {
	...
}
```

<br>

来到我们的 go.mod

来分析一下最关键的三行内容

1. 第一行即 gomod 管理的所在包名称
2. 第二行即 go 版本
3. 第三行及剩余行即导入的依赖

```
module gindemo.com

go 1.19

require github.com/gin-gonic/gin v1.8.2
```

<br>

此时记下我们 `go.mod` 管理的包名叫做： `gindemo.com` （其实就是咱们的项目名）

打开 main.go 修改代码为

```go
package main

import (

    // 这表示取别名，用小写字母c代表后面导入的内容
	c "gindemo.com/src/Chapter1"
	"github.com/gin-gonic/gin"
	"net/http"
)

func main() {
	app := gin.Default()

	app.GET("/shit", func(context *gin.Context) {
		context.JSON(http.StatusOK, gin.H{
			"error": "shit",
		})
	})

    // 在这里调用 C1.go 文件内的方法
	app.GET("/c1",c.BindDemo)

	app.Run(":10086")
}
```

我们来分析一下 import 里面这行代码的含义：`c "gindemo.com/src/Chapter1"`

1. 小写字母 c，表示给后面的包取个别名，我们之后直接使用 c 来调用
2. 导包绝不可以使用相对路径，必须要用 go.mod 里面 module 行给出的路径打头，然后依次索引到我们的文件
3. 这里是直接导入 Chapter1 整个包，也即导入其所拥有的所有 go 文件

<br>

此时你发现 goland 直接飘红，但这不是我们的问题！直接运行是可以跑起来的！

由于我们直接导入 Chapter1 整个包，所以按照我们定的别名 c ，直接调用方法即可使用！

<br>
