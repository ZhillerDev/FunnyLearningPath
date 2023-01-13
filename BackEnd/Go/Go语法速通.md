### 基础

#### 主函数定义格式

`package main` 表示当前文件是整个程序的入口点，该文件将会编译为二进制可执行文件！

只要 package 后面不是 main，那么就不会编译为可执行文件

`func main` 是一个入口点

```go
package main

import "fmt"

func main() {
	fmt.Printf("fuck")
}
```

<br>

#### 编码风格

1. 包名全部小写
2. main 函数仅能写在 main 包中，这就表示 main 包不唯一！！！
3. 只要你定义了变量、引入了库没用的话直接报错而不是警告

<br>

#### 变量函数与省略

声明变量：`var 变量名 数据类型 = 值`  
声明常量：`const 变量名 数据类型 = 值`  
快速声明：`变量名 := 值`

声明函数：`func 函数名(形参 形参类型)(返回值类型){}`

```go
package main

import (
	"fmt"
	"strconv"
)

func main() {
    // 显而易见，我们只使用了plus函数返回值的第一个，第二个没用到
    // 下划线可以表示接收数据但不存储，有效规避了go检测变量没用到就报错的弱智特性
	var sum, _ int = plus(100,200)
	fmt.Printf(strconv.Itoa(sum))
}

// 定义一个函数，接收两个int类型的形参
// 返回值为两个int类型的数字
func plus(a, b int) (int, int) {
	return a, b
}
```

<br>

#### 变量进阶

连续定义变量

```go
func main() {
	// 动态类型判断或者强类型
	var _, _ int = 1, 2
	var _, _ = 100, 200

	// :=形式的赋值可没法使用下划线！！！
	num1, num2 := 100, 200
	fmt.Print(num1, num2)
}
```

变量组
