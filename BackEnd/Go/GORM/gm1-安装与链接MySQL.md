### 安装依赖

> 可以看官网介绍 https://gorm.io/zh_CN/docs/

go 安装依赖，分别是 gorm 核心以及 mysql 支持依赖：  
`go get -u gorm.io/gorm`  
`go get -u gorm.io/driver/mysql`

<br>

### 设置数据库链接

新建文件 `models/DbInit.go`

我们把链接数据库封装，每次调用时直接取出变量即可  
如下图代码，每次使用数据库查询时只需调用变量 DB 即可！！！

```go
package model

import (
	"fmt"
	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

// 全局DB对象
var DB *gorm.DB

var err error

// init()是go中的特殊方法，会在初始化执行，无需显式调用！
func init() {
    // 数据库连接配置
    // 格式： 用户名:密码@tcp(链接地址)/数据库名?charset=utf8mb4&parseTime=True&loc=Local
	dsn := "root:123456@tcp(127.0.0.1:3306)/golang?charset=utf8mb4&parseTime=True&loc=Local"

    // 这是默认的链接数据库的代码，err进行可否链接判断
	DB, err = gorm.Open(mysql.Open(dsn), &gorm.Config{})
	if err != nil {
		fmt.Println(err)
	}
}
```

<br>

### 设置实体与链接

#### 创建一个实体

新建文件 `models/user.go`

编辑实体时需要注意以下几点哦

1. 结构体属性全部开头大写，这样才能外部调用
2. 属性的类型需要和数据库中类型一一匹配
3. 结构体名称也必须大写，最好直接和数据表名字一致
4. 根据 GORM 特性，经过解析后默认的表名为结构体名称全小写后加上 s，即 users，所以我们可使用 TableName 对表名执行自定义！

```go
package model

// 定义结构体，代表我们要操作的每一条数据
// 注意大写
type User struct {
	Id       int
	Username string
	Age      int
	Email    string
	AddTime  int
}

// TableName方法可以显式指定当前表名称，避免默认值导致错误
func (User) TableName() string {
	return "user"
}
```

<br>

#### 链接数据库并查询

注册一个新的 controller： `controller.UserController.md`

新建方法写业务逻辑即可

```go
func QueryAllUsers(c *gin.Context) {
    // 定义实体数组
	var userList []model.User

    // 调用数据库并执行方法Find查询
    // 这里的会根据参数的类型执行查询并将查到的结果全部返回到参数内，所以最终变量userList获得结果
    // 注意传递方式
	model.DB.Find(&userList)
	c.JSON(200, gin.H{
		"res": userList,
	})
}
```

<br>
