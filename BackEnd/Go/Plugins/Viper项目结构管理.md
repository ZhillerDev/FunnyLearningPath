### 快速上手

安装：`github.com/spf13/viper`

<br>

#### 文件结构

使用 config 文件夹下的 `config_loader.go` 读取配置文件 `config.yaml`

![](../../img/go/plugins/viper/vp1.png)

<br>

#### 读配置文件

我们首先随便在配置文件内写一些配置

配置文件 `config.yaml`

```yaml
port: 10001
desc: "hello viper"
```

<br>

然后编写我们初始化 viper 的代码（此操作会直接把配置文件和 viper 管理，之后在任意文件内都可以直接使用 viper 操纵配置，所以在这里我们不需要全局暴露 viper 对象）

`config/config_loader.go` 代码清单：

```go
package config

import (
	"fmt"
	"github.com/spf13/viper"
)

func ConfigurationInit() {
	viper.SetConfigName("config")   // 设置配置名称
	viper.SetConfigType("yaml")     // 设置配置类型
	viper.AddConfigPath("./")       // 从哪里开始查找配置文件
	err := viper.ReadInConfig()     // 读配置文件

    // 错误处理
	if err != nil {
        // 如果读不出来，直接中断程序
		panic(fmt.Errorf("read config err=%s", err))
	} else {
        // 读到了，就输出其中的一段文字
		fmt.Println(viper.GetString("desc"))
	}
}
```

<br>

好啦！我们就已经把配置文件挂载到全局对象 viper 上面去了，是不是很简单？

最后一步，来到 main.go ，直接使用 viper 全局对象通过配置的 key 获取其 value 即可

```go
func main() {
	router := gin.Default()

	config.ConfigurationInit()  // 初始化config

	fmt.Println(viper.GetString("desc")) // 获取配置文件指定key的value并打印出来

	router.Run(":10001")
}
```

<br>

#### 写配置文件
