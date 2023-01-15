### Goroutine

> 协程，和 C#中的 coroutine 差不多一个意思

协程是运行于单个线程上的轻量级线程，一次可并发上万个协程

协程具有：独立栈空间、共享堆空间、自由调度等多种特性

协程必须依附于主线程，主线程一旦死掉协程也就不存在了

协程启动的格式：`go xxx()`

<br>

这是一个最简单的协程示例：

运行结果是协程和主线程交替出现，每次打印的个数随机，不是你想象的一次打印一条然后交替出现！！！

```go
package main

import "fmt"

// 协程方法
func Say() {
	for i := 0; i < 1000; i++ {
		fmt.Println("goroutine")
	}
}

func main() {
    // 启动协程
	go Say()
    // 主线程执行for，协程交替穿插执行for
	for i := 0; i < 1000; i++ {
		fmt.Println("main")
	}
}
```

<br>

### 锁

#### 重型锁

使用方法和 java 中的重型锁 synchronized 差不多

```go
package main

import (
	"fmt"
	"sync"
	"time"
)

// 定义一把锁
var lock sync.Mutex

// 协程方法
func printer(msg string) {
    // 获得锁
	lock.Lock()
    // 每延迟0.5s输出一个字符
	for _, s := range msg {
		fmt.Printf("%c", s)
		time.Sleep(time.Millisecond * 500)
	}
    // for循环完毕后释放锁，让下一个协程获得锁
	lock.Unlock()
}

func main() {
    // 定义两个匿名函数使用go协程，先后获得锁，先后输出
	go func() {
		printer("shit")
	}()
	go func() {
		printer("damn")
	}()

    // 空for使主线程保活
	for {
	}

}
```
