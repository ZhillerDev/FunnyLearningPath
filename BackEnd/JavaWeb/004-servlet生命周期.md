### 生命周期速览

#### 优先级

servlet 的声明周期由 tomcat 服务器自行管辖，程序员无法插手；

只要没有通过 url 访问 servlet，那他就永远不会先行实例化；  
除非我们通过在 web.xml 的 servlet 标签下加上以下标签，即可立即实例化：  
`</load-on-startup/>`

该标签内也可以填写整数，整数越小表示优先级越高：  
`<load-on-startup>1</load-on-startup>`

<br>

#### servlet 生命周期完整过程

默认情况，服务器启动后 servlet 对象不会立刻初始化

当用户发送第一次请求时：

1. 执行 servlet 无参构造
2. tomcat 调用 servlet 的 init 方法
3. tomcat 继续调用 service 方法

当用户第二次乃至之后多次请求时：  
因为 servlet 是单例的，所以不会再实例化新的 servlet 对象；  
即 init 方法仅执行一次，而 service 会在每次调用时执行一次；

<br>

### servlet 所有核心方法解析

#### 无参构造方法

首次实例化 servlet 的时候第一个调用的构造方法！

每个 servlet 都必须有一个无参构造方法，且`不能存在有参构造方法`！！！

servlet 规定，不建议随意增删构造方法！

<br>

#### init

init 仅会在首次实例化 servlet 的时候执行一次；

<br>

#### service

处理用户请求的方法；

每次调用 servlet 都会执行一次；

<br>

#### destroy

servlet 销毁前（注意是销毁前！！！）执行的方法；

仅执行一次

<br>

### 适配器模式去除冗余接口

因为在一个 servlet 中，最常用的接口实际上就只有 init 和 service 俩，故需要隐去其他不常用的接口来保证代码的整洁度；

解决方式：构造一个通用的抽象类 NormalServlet，并设置仅 service 为抽象方法，此时所有继承该抽象类的子类就近需要实现唯一的 service 抽象方法了！

> 代码清单：NormalServlet.java

```java
package com.zhiyiyi.javaweb.servlet;

import jakarta.servlet.*;

import java.io.IOException;

// 请注意此类为抽象类
public abstract class NormalServlet implements Servlet {
    @Override
    public void init(ServletConfig servletConfig) throws ServletException {

    }

    @Override
    public ServletConfig getServletConfig() {
        return null;
    }

    // 仅设置service方法为一个抽象方法
    @Override
    public abstract void service(ServletRequest servletRequest, ServletResponse servletResponse)
            throws ServletException, IOException;

    @Override
    public String getServletInfo() {
        return null;
    }

    @Override
    public void destroy() {

    }
}
```

<br>

> 代码清单：CServlet.java

```java
package com.zhiyiyi.javaweb.servlet;

import jakarta.servlet.ServletException;
import jakarta.servlet.ServletRequest;
import jakarta.servlet.ServletResponse;

import java.io.IOException;

public class CServlet extends NormalServlet{
    // 可见，目前就近需要实现唯一的service了，代码简洁了许多！！！
    @Override
    public void service(ServletRequest servletRequest, ServletResponse servletResponse) throws ServletException, IOException {

    }
}
```

<br>

这还没完，我们需要再次对 `NormalServlet` 进行些许优化，好让子类能够获取父类的 `ServletConfig` 对象

首先声明一个私有的 ServletConfig 对象，之后于 init 方法内进行实例化；

之后子类就可以通过调用父类中的 `getServletConfig` 方法来获取 ServletConfig 对象了！！！

```java
private ServletConfig config;

@Override
public void init(ServletConfig servletConfig) throws ServletException {
    this.config = servletConfig;
}

@Override
public ServletConfig getServletConfig() {
    return config;
}
```

<br>

### GenericServlet.java

这个玩意是 servlet 提供的官方版本，用来替换我们刚刚编写的 NormalServlet 的；

它的功能更加齐全，且能应对子类需要重写 init 方法的需求；

以后我们编写 servlet 只需要继承 `GenericServlet` 就好了；

<br>
