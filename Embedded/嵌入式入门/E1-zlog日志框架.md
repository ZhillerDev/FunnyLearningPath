### C 变参宏

#### 输出全部参数小案例

> 案例目的：设计一个函数，第一个形参表示欲输出的参数数量，根据该形参输出所有可变长形参（太绕了我也表达不出来呜呜呜）

比较复杂，请看分解：

下面代码函数 d2demo 包含一个主索引 ix，以及三个点表示的可变长参数

`va_list` 一个指针类型，用于指向参数  
`va_start` 将指针指向参数的指定位置，参数一为 va_list 类型指针，参数二为令该指针指向的参数  
`va_arg` 返回当前指针指向的参数值，参数一为 va_list 指针，参数二为该参数的类型  
`va_end` 接收一个 va_list 指针，是一个结束标识符，必须存在！

```cpp
#include <stdio.h>
#include <stdarg.h>

// 该函数有一个主索引ix，后面的三个点表示可变长参数
void d2demo(int ix, ...)
{
    // 定义参数指针，这一步必须要存在！
    va_list arg;

    // 将参数指针指向函数形参的第一个参数ix
    va_start(arg, ix);

    // 每次循环都让ix递减，直到打印出所有的参数
    while (ix-- > 0)
    {
        // 打印形参，并且每打印一次，参数指针arg都会自动前进一步
        printf("%d \n", va_arg(arg, int));
    }

    // 结束标志，必须要有！
    va_end(arg);
}

int main(int argc, char const *argv[])
{
    d2demo(3, 100, 200, 300); // 输出100 200 300
    return 0;
}
```

<br>

### zlog

#### 低配手写模拟版

这里借助我们刚刚学到的变参宏手写一个最简单的 zlog 日志框架，可以实现指定 LOG 等级输出内容，低于该等级的无法输出

不难发现宏 LOG_LEVEL 定义了整个文档的 LOG 等级，只要我们选择输出的 LOG 等级大于或等于该等级，那么就可以输出

```c
#include <stdarg.h>
#include <stdio.h>

#define OPEN_LOG 1
#define LOG_LEVEL LOG_DEBUG

// 定义所有LOG等级
typedef enum
{
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARN = 2,
    LOG_ERROR = 3
} E_LOGLEVEL;

// 获取当前的LOG等级
char *EM_LOGLevelGet(const int level)
{
    switch (level)
    {
    case LOG_DEBUG:
        return "DEBUG";
    case LOG_INFO:
        return "INFO";
    case LOG_WARN:
        return "WARNING";
    case LOG_ERROR:
        return "ERROR";

    default:
        return "UNKNOWN";
    }
}

// 解析参数
void EM_LOG(const int level, const char *fmt, ...)
{
#ifdef OPEN_LOG
    va_list arg;
    va_start(arg, fmt);

    // vsnprintf用于将文本塞到buff缓冲区
    char buf[1 + vsnprintf(NULL, 0, fmt, arg)];
    vsnprintf(buf, sizeof(buf), fmt, arg);
    va_end(arg);

    // 若当前形参LOG等级大于全局LOG等级，那么就允许输出
    if (level >= LOG_LEVEL)
    {
        printf("[%s] %s\n", EM_LOGLevelGet(level), buf);
    }
#endif
}

int main(int argc, char const *argv[])
{
    EM_LOG(LOG_DEBUG, "app start");
    return 0;
}
```

<br>
