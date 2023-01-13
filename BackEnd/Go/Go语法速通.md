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

快速类型转换

```go
func main() {
	// 数值快速转换字符串
	// Itoa仅接受int类型数值
	var num1 int32 = 32
	var _ string = strconv.Itoa(int(num1))

	// 字符串快速转换数值
	// 返回值的err问题上一节讲过，要注意！！！
	str1 := "shit"
	num2, err := strconv.Atoi(str1)
	if err != nil {
		fmt.Print(err)
	}
	fmt.Print(num2)
}
```

<br>

#### 常量定义

基本常量定义法以及常量组

```go
func main() {

	// 直接定义，使用const修饰
	const _ string = "fuck"
	const _, _ = 123, "asd"

	// 常量组定义，下一行如果没有指定，那么他的值和上一行一样
	const (
		_ = 100
		_ // 和上一个一样，都是100
		_,_ = 20,"shit"
		_,_ // 和上一个一样，注意变量个数必须一致！！！
	)
}
```

<br>

枚举常量

```go
func main() {
	// iota可认为是一个枚举值，第一个iota从0开始递增
	const(
		_ = iota // 0
		_ = iota // 1
		_ = 666 // 没有iota了，中断递增
		_ = iota // 重新递增，0
	)

	// 根据常量值上下继承原则，iota也可以实现递增
	const(
		_ = iota // 0
		_ // 1
		_ // 2
	)

	// 指定类型后，都会按照该类型进行枚举递增
	const(
		_ float32 = iota // 0.0
		_ // 1.0
	)
}
```

<br>

#### IO

格式化输出和 C 差不多

```go
func main() {
	// 格式化字符串，使用printf
	name, num := "shit", 114
	fmt.Printf("%s, %d", name, num)

	// %T可以输出值的类型
	fmt.Printf("%T", name)

	// %v万能输出，直接输出值而无需判断类别
	fmt.Printf("%v", num)
}
```

<br>

scanf 在 go 中的使用

```go
func main() {
	var num int

	// 格式化输入
	fmt.Scanf("%v",&num)
	// 不带格式化输入
	fmt.Scan(&num)
	// 不带格式化输入，仅一行
	fmt.Scanln(&num)
}
```

<br>

#### 运算与条件语句

go 中自增自减符号不可以前置！

go 中的自增自减必须独占一行，即不可以和赋值语句写到一行里面去

<br>

if 判断格式 `if 初始化语句;条件表达式{}`

```go
func main() {
	if age := 10; age < 10 {
		fmt.Printf("shit")
	}

	// switch的case里面不要写break，可以视为go已经为每个case加上了一个break
	// 如果非要消除break的影响，使用fallthrough
	switch num := 10; num {
	case 1:
		fmt.Printf("shit")
	case 2:
		fallthrough
	default:
		fmt.Printf("damn")
	}
}
```

<br>

go 中的 foreach

可以吧这种语法当成解构赋值，i 存储索引，v 存储数值

```go
func main() {
    // 声明一个数组
	arr := [3]int{1, 2, 3}
    // i和v是两个固定的参数，但是参数名可以变
	for i, v := range arr {
		fmt.Printf("%v%v", i, v)
	}
}
```

<br>

### 进阶

> 看到这里你是不是觉得像完全 0 基础学语言？看看这 nt 的语法吧！

#### 调用命令行

> 所有的命令行参数都会被解析为字符串类型的

`os.Args` 接收所有我们传入的参数；

假设该文件编译为了一个 exe，我们执行该 exe 是使用代码： `main.exe asd 123`  
则上面三个元素都会变成参数传递给 `os.Args`

```go
import (
	"fmt"
	"os"
)

func main() {
	// 获取传入参数个数
	num := len(os.Args)
	// 输出所有参数
	for i := 0; i < num; i++ {
		fmt.Printf(os.Args[i])
	}
}
```

<br>

flag 包可以自定义传参名称

则运行 exe 是可以使用命令：`main.exe -name=tom`

```go
// 写法一
func main() {
	// flag对应的三个参数
	// 参数名、参数默认值、参数简介
	name := flag.String("name", "jack", "使用者名称")
	// 命令行参数解析到注册函数
	flag.Parse()
	// flag返回的均为指针类型，所以需要解指针
	fmt.Println("名字是：", *name)
}

// 写法二
func main() {
	var (
		name string
		age  int
	)
	flag.StringVar(&name, "name", "jack", "姓名")
	flag.IntVar(&age, "age", 13, "你的年龄")
	flag.Parse()
}
```

<br>

#### 函数进阶

go 中甚至可以为返回值指定名称，即系统会自动返回该名称变量

```go
// 注意这里不使用:=
func say() (res string) {
	res = "shit"
	return
}
```

<br>

匿名函数

```go
func main() {
	// 匿名函数声明方式一
	a := func() {
		fmt.Printf("shit")
	}
	a()

	// 匿名函数声明方式二
	func(name string) {
		fmt.Println(name)
	}("fuck")
}
```

<br>

匿名函数作为参数传入

```go
func main() {
	work(func() {
		fmt.Println("fuck")
	})
}

func work(f func())  {
	fmt.Println("shit")
	f()
}
```

<br>

#### 闭包
