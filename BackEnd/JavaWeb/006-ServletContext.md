### ServletContext 基础知识

#### 获取 ServletContext 对象

有两种方式可以获取：

1. 使用 servletconfig 对象获取
2. 使用 servlet 上下文获取

```java
// 第一种方式
ServletContext app1 = config.getServletContext();
writer.println("<br>" + app1);
// 第二种方式
ServletContext app2 = this.getServletContext();
writer.println("<br>" + app2);
```

<br>

#### 特性

1. 一个 webapp 只存在一个 ServletContext 对象
2. ServletContext 对象是由 web 服务器启动时创建
3. ServletContext 是一个接口，他依然遵循 servlet 规范
4. ServletContext 在 web 服务器销毁时才会销毁
5. ServletContext 存储了 `整个 web.xml` 的信息

<br>
 
#### context-param

和 servletconfig 一样，他也可以获取与 web.xml 中定义的参数；

只不过 `ServletContext` 定义的参数是写在 servlet 标签外面的；  
每一个 `context-param` 都代表一个 key-value；  
如果需要多个参数就必须要 `分别写` context-param

```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="https://jakarta.ee/xml/ns/jakartaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="https://jakarta.ee/xml/ns/jakartaee https://jakarta.ee/xml/ns/jakartaee/web-app_5_0.xsd"
         version="5.0">

    <!-- 定义了两个外部参数 -->
    <context-param>
        <param-name>pageSize</param-name>
        <param-value>10</param-value>
    </context-param>
    <context-param>
        <param-name>porkPrice</param-name>
        <param-value>999</param-value>
    </context-param>

    ...

</web-app>
```

<br>

同样的，我们可以使用 `Enumeration` 获取所有的 `context-param`，然后再使用迭代器的方法一次获取其中的 `key` 和 `value`

```java
package com.zhiyiyi.javaweb.servlet;

import jakarta.servlet.*;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.Enumeration;

public class AServlet extends GenericServlet {
    @Override
    public void service(ServletRequest servletRequest, ServletResponse servletResponse) throws ServletException, IOException {
        servletResponse.setContentType("text/html");
        PrintWriter writer = servletResponse.getWriter();
        ServletContext servletContext = this.getServletContext();

        // 迭代输出param-name和param-value
        Enumeration<String> initParameterNames = servletContext.getInitParameterNames();
        while (initParameterNames.hasMoreElements()) {
            String key = initParameterNames.nextElement();
            String value = servletContext.getInitParameter(key);
            writer.println(key + " " + value + "<br>");
        }
    }
}
```

> 可见，context-param 定义的是 webapp 内共享的配置信息，而 servlet-name 更适用于仅供单个 servlet 使用

<br>

#### 获取文件路径

获取根路径，即我们配置 web 服务器时设置的路径

```java
// 获取web的根路径
String contextPath = servletContext.getContextPath();
writer.println("<br>" + contextPath);
```

<br>

获取指定文件的绝对路径；

getRealPath 接收一个路径作为参数，这个路径的起始点是 web 文件夹，下方代码指的是 `web/index.html` 这个文件

> 注意注意！getRealPath 的参数别带上根目录路径！！！

```java
// 获取绝对路径
String realPath = servletContext.getRealPath("/index.html");
writer.println("<br>" + realPath);
```

<br>

#### 记录日志

可以使用 log 方法记录日志；

日志保存位置为 `tomcat 根目录下/logs`

`servletContext.log("hellowrodl");`

<br>

#### 参数增删改查

我们可以直接在 servlet 中直接对 `context-param` 进行增添、查找、删除！！！

```java
// 以键值对的方式添加参数
servletContext.setAttribute("name","tom");
// 获取参数
Object name = servletContext.getAttribute("name");
// 删除参数
servletContext.removeAttribute("name");
```

<br>
