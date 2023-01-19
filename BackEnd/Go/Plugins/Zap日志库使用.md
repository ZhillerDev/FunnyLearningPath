### 快速上手

> 使用 zap 是因为 gin 官方自带的日志库功能较少且配置麻烦，并不是很实用，所以改用 zap 库

<br>

#### 安装

安装：`go get -u go.uber.org/zap`

<br>

#### 原理简介

zap 库支持两种日志类型：

1. logger 库：大项目使用，内存分配更佳，但只能处理强类型的结构化日志
2. sugarlogger 库：小项目使用，速度快，且支持 printf 格式化输出

<br>

#### 三种无配置构建 logger 方式

1. NewExample，以 JSON 格式输出
2. NewProduction，以 String 格式输出时间+内容
3. NewDevelopment，输出更完整的 JSON 内容

```go
var logger *zap.Logger

func main() {
	router := gin.Default()

	// logger = zap.NewExample
	// logger,_ = zap.NewProduction()
	logger,_ = zap.NewDevelopment()
	logger.Info("信息提示")

	router.Run(":10001")
}
```

可以对任意 `logger` 对象通过方法 `Sugar` 将其变成 `sugarlogger`

```go
logger,_ = zap.NewDevelopment()
sugarLogger := logger.Sugar()
sugarLogger.Info("god")
```

<br>

#### 抽离 logger 初始化

一般的，我们可能需要自定义特定规则的 logger，故最好把 logger 初始化的过程单独写在一个文件内，并在路由构建的过程中调用

初始化一个 logger 需要具备以下三个条件：

1. encoder：写入日志的编码器（决定最终输出的日志格式）
2. writersyncer：日志输出路径，这里需要使用 addsync 进行以下输出格式的转换才可以
3. levelenable：输出日志的等级，最低级为 debuglevel

<br>

新建文件 `logger/logger_init.go`

```go
package logger

import (
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
	"os"
)

// 首字母大写，公开这两个logger实例
var Logger *zap.Logger
var SugarLogger *zap.SugaredLogger

// 对三个条件定义为一个结构体
type loggerConfig struct {
	encoder      zapcore.Encoder
	writerSyncer zapcore.WriteSyncer
	levelEnable  zapcore.Level
}

// logger初始化方法
func InitLogger() {
    // 实例化结构体
	config := loggerConfig{
        // 使用zap自带的NewJSONEncoder定义为一个JSON编码器作为输出
		encoder: zapcore.NewJSONEncoder(zap.NewProductionEncoderConfig()),
        // 匿名函数，返回文件保存位置
        // 注意这里需要使用AddSync对输出流的类型执行转换
		writerSyncer: func() zapcore.WriteSyncer {
			file, _ := os.OpenFile("./src/logger/logger.json", os.O_RDWR|os.O_CREATE|os.O_APPEND, 0644)
			return zapcore.AddSync(file)
		}(),
        // 日志等级配置，使用最低级的debuglevel
		levelEnable: zapcore.DebugLevel,
	}

    // 将所有参数依次填入
	core := zapcore.NewCore(
		config.encoder,
		config.writerSyncer,
		config.levelEnable,
	)

    // 使用new方法实例化logger
	Logger = zap.New(core)
    // 变换为sugarlogger
	SugarLogger = Logger.Sugar()
}
```

<br>

这里对文件保存代码解释一下：  
`os.OpenFile("./src/logger/logger.json", os.O_RDWR|os.O_CREATE|os.O_APPEND, 0644)`

参数一：文件读写流默认相对位置起始点为项目根目录，所以这里一路顺下来就可以了  
参数二：读写模式  
参数三：固定值，不用管

<br>

#### 同时输出到控制台与日志文件

使用 zap 提供的多流输出模式即可

我们仅需修改 core 实例化的代码即可，将第二个参数修改为使用 NewMultiWriteSyncer 方法，将我们定义的文件输出流与控制台输出流合在一起即可

```go
core := zapcore.NewCore(
    config.encoder,

    // 第一个是文件输出流，第二个为控制台输出（注意这里仍然需要AddSync仅需转型）
    zapcore.NewMultiWriteSyncer(
        config.writerSyncer,
        zapcore.AddSync(os.Stdout),
    ),
    config.levelEnable,
)
```

<br>

#### 输出优化

让输出级别大写，且时间格式化易于理解

由于目前添加的属性较多，故将三大条件抽离为单个函数编写，而不用结构体；

此时就可以直接自定义一个 `encoderConfig`，修改时间与输出的类型即可

```go
package logger

import (
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
	"os"
)

var Logger *zap.Logger
var SugarLogger *zap.SugaredLogger

func InitLogger() {

	core := zapcore.NewCore(
		getEncoder(),
		zapcore.NewMultiWriteSyncer(
			getWriterSyncer(),
			zapcore.AddSync(os.Stdout)),
		getLevelEnable(),
	)

	// addcaller是为了显示调用者，该参数可省略掉
	Logger = zap.New(core, zap.AddCaller())
	SugarLogger = Logger.Sugar()
}

func getEncoder() zapcore.Encoder {
	encoderConfig := zap.NewProductionEncoderConfig()       // 获取配置实例
	encoderConfig.EncodeTime = zapcore.ISO8601TimeEncoder   // 规定时间格式
	encoderConfig.EncodeLevel = zapcore.CapitalLevelEncoder // 全部大写
	return zapcore.NewJSONEncoder(encoderConfig)
}

func getWriterSyncer() zapcore.WriteSyncer {
	file, _ := os.OpenFile("./src/logger/logger.json", os.O_RDWR|os.O_CREATE|os.O_APPEND, 0644)
	return zapcore.AddSync(file)
}

func getLevelEnable() zapcore.Level {
	return zapcore.DebugLevel
}
```

<br>

#### 文件分割
