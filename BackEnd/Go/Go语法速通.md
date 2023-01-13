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

```go
func main() {
	// 变量组
	// 可单次或者连续指定
	var(
		_ float32 = 32.12
		_,_,_ int = 1,2,3
		_,_ = 123,0.123
	)
}
```

<br>

变量指定规则

1. 变量值不可使用 :=
2. := 只能定义局部变量
3. := 不能有 var 和数据类型
4. := 同时定义多个变量必须都执行初始化

<br>

#### 类型转换

Golang 只能显式类型转换！

以下包括数值之间强转以及绕弯的数值到字符串之间强转

```go
func main() {
	// num0默认赋值int类型
	// int不等于int32，同样需要显式强转
	num0 := 10
	var _ = int32(num0)

	// 使用strconv.FormatInt进行数值到字符串强转
	// 1.定义一个int32类型的数字num1
	// 2.int64(num1)先将其强转64位整数
	// 3.strconv.FormatInt强转
	var num1 int32 = 100
	// strconv.FormatInt的两个参数：
	// 第一个参数：强转的数字
	// 第二个参数：转换至几进制
	var _ string = strconv.FormatInt(int64(num1), 10)
}
```

<br>

字符串转数值类型

```go
func main() {
	// strconv.ParseInt接收三个参数
	// 参数一：待转换字符串
	// 参数二：转换为几进制
	// 参数三：转换为几位整形
	// 他会返回两个值，其中第二个返回值为err
	// 当数值转换未超出范围或无法转换时，err值为nil，否则不为nil
	str1 := "123"
	num1,err := strconv.ParseInt(str1,10,32)
	if err != nil{
		fmt.Print(err)
	}
	fmt.Print(num1)
}
```

<br>
