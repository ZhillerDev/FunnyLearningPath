### 配置流程

#### 注意区分

`go-swagger` != `swaggo`

二者功能差不多，都是生成接口文档用的；  
前者构建速度极慢且官方文档极不友好；  
后者构建速度快且上手简单，故我们这里采用它；

<br>

#### 下载 swaggo

项目根目录下（即与 go.mod 同级的文件夹）命令行执行此代码，下载 swaggo  
`go get -u github.com/swaggo/swag/cmd/swag`

<br>

之后就是关键的 swag 环境变量配置！

1. 进入你之前设置的 GOPATH 目录下
2. 进入该文件夹：`pkg\mod\github.com\swaggo\swag@v1.8.9\cmd\swag`，发现里面有一个文件 `main.go`
3. 在当前文件夹下打开命令行，输入 `go build` 执行构建过程
4. 构建完毕得到 `swag.exe` ，复制他
5. 丢到 golang 根目录下的 bin 文件夹内即可（前提是你之前在系统变量 path 里面添加了%GOROOT/bin 这个路径值，否则你依然需要重新配置一遍系统变量）

> 更新：对于版本 go1.18+，可以直接使用 go init 代替 go build，这样做的好处是可以直接省去后面繁琐的变量配置了

<br>

此时在命令行输入 `swag -v` 后可以看见版本号则表示环境变量配置完毕

<br>

#### 初始化

进入项目根目录，保证当前目录下存在至少一个 go 文件（比如主入口文件 `main.go`）

当前目录下打开命令行，输入 `swag init` 即可根据 go 文件自动构建 swagger 文档了；  
新生成的文档存放在 docs 文件夹下

<br>

### 快速上手

#### gin-swagger

> 官方参考文档https://github.com/swaggo/gin-swagger

项目根目录执行代码安装依赖：

```
go get -u github.com/swaggo/gin-swagger
go get -u github.com/swaggo/files
```

<br>

#### 简单测试

在测试前务必先执行 `swag init` 生成对应 docs 文件！

import 内需要手动导入 docs，其余的两个包 goland 会自动处理

通过注释编写接口信息，然后于 main 方法注册 swagger-ui 的界面路由就完毕了

```go
package main

import (
    // 这里需要我们手动导入docs，不然报错
    // 因为该包我们没有在代码中实际引用到，故别名需要取名为下划线
	_ "ginoj/docs"

	"github.com/gin-gonic/gin"

    // 这两个是固定的，不用修改直接复制黏贴导包
	swaggerfiles "github.com/swaggo/files"
	ginSwagger "github.com/swaggo/gin-swagger"

	"net/http"
)

// @Summary ping example
// @Schemes
// @Description do ping
// @Tags example
// @Accept json
// @Produce json
// @Success 200 {string} Helloworld
// @Router /example/helloworld [get]
func Helloworld(g *gin.Context) {
	g.JSON(http.StatusOK, "helloworld")
}

func main() {
	router := gin.Default()

    // 注册swagger-ui界面路由
	router.GET("/swagger/*any", ginSwagger.WrapHandler(swaggerfiles.Handler))

	router.Run(":10001")
}

```

此时进入 `localhost:8080/swagger/index.html` 就可以看见接口文档了（注意这里的端口修改为你使用的那一个！）

<br>

#### 详细配置

通过上述操作我们发现依然会提示缺失了 spec 文件，这是我们没有执行 init 的缘故；

注意，每次修改一次文档都必须执行 `swag init` 一次，并且重启服务器才可以正常渲染；

我们把 main.go 改造为这个样子，为 main 添加了一些全局注释

```go
package main

import (
	_ "ginoj/docs"
	"github.com/gin-gonic/gin"
	swaggerfiles "github.com/swaggo/files"
	ginSwagger "github.com/swaggo/gin-swagger"
	"net/http"
)

// Helloworld
// @Description a simple example
// @Tags example
// @Accept json
// @Produce json
// @Param id path int true "shit"
// @Success 200 {string} json "{"status":"success"}"
// @Router /helloworld [get]
func Helloworld(g *gin.Context) {
	g.JSON(http.StatusOK, "helloworld")
}

// @title           Swagger Example API
// @version         1.0
// @description     This is a sample server celler server.
// @termsOfService  http://swagger.io/terms/
// @contact.name   API Support
// @contact.url    http://www.swagger.io/support
// @contact.email  support@swagger.io
// @license.name  MIT
// @license.url   http://www.apache.org/licenses/LICENSE-2.0.html
// @host      localhost:10001
func main() {
	router := gin.Default()
	router.GET("/swagger/*any", ginSwagger.WrapHandler(swaggerfiles.Handler))
	router.Run(":10001")
}

```

回到根目录下执行 `swag init`，之后重启服务器即可看到文档了

<br>
