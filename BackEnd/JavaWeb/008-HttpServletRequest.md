### 接口详解

#### 搭设基本测试环境

web 下新建 `reg.html` 文件，用作注册网页；

这里使用了 form 表单，注意提交的 action 是 `根目录 + servlet的url`；  
请求方式我们使用 `post`

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta
      name="viewport"
      content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0"
    />
    <meta http-equiv="X-UA-Compatible" content="ie=edge" />
    <title>Document</title>
  </head>
  <body>
    <h1>用户注册</h1>
    <form action="/05/rds" method="post">
      username: <input type="text" name="username" /><br />
      password: <input type="password" name="password" /><br />
      <input type="submit" value="reg" />
    </form>
  </body>
</html>
```

<br>

在 web.xml 中把注册页面设置为欢迎页

```xml
<welcome-file-list>
    <welcome-file>reg.html</welcome-file>
</welcome-file-list>
```

<br>

新建测试 servlet，然后记得在 web.xml 中注册

```java
package com.zhiyiyi.javaweb.servlet;

...

// 依旧使用HttpServlet接口
public class RequestDemoServlet extends HttpServlet {
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

    }
}
```

```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="https://jakarta.ee/xml/ns/jakartaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="https://jakarta.ee/xml/ns/jakartaee https://jakarta.ee/xml/ns/jakartaee/web-app_5_0.xsd"
         version="5.0">

    <!-- 注册servlet -->
    <servlet>
        <servlet-name>requestDemoServlet</servlet-name>
        <servlet-class>com.zhiyiyi.javaweb.servlet.RequestDemoServlet</servlet-class>
    </servlet>
    <servlet-mapping>
        <servlet-name>requestDemoServlet</servlet-name>
        <url-pattern>/rds</url-pattern>
    </servlet-mapping>

    ...

</web-app>
```

<br>

#### 取回 keys

因为我们在 reg.html 中使用 post 请求后端，携带的参数将以键值对的形式存在；

后端我们仅需重写 doPost 方法即可对前端请求作出响应；

代码内容和之前所学的一致，使用 `getParameterNames` 获取所有参数的 keys；  
之后遍历以下输出所有 keys

```java
public class RequestDemoServlet extends HttpServlet {
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        Enumeration<String> names = req.getParameterNames();
        while (names.hasMoreElements()) {
            String name = names.nextElement();
            System.out.println(name);
        }

    }
}
```

<br>

#### 取回 values

因为绝大多数情况下，我们均会知晓前端传入参数的 keys，即可以直接使用 key 获取对应的 value；

getParameter 如果一个 key 仅对应一个 value，使用此方法；  
getParameterValues 若一个 key 对应多个 values，使用此方法返回一个字符串数组；

> 注意：无论你在前端传入的是什么样的数据类型，在后端所有的 keys 和 values 均为字符串形式！

```java
@Override
protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

    String username = req.getParameter("username");
    String[] password = req.getParameterValues("password");

    System.out.println(username);
    System.out.println(Arrays.toString(password));

}
```

<br>

### 请求域与应用域

#### 应用域对象

1. servletcontext 对象
2. 缓存技术，如常量池、线程池、链接池

<br>

#### 请求域对象

请求域的生命周期很短，作用范围仅一次请求；

请求结束后，请求域就会销毁；

请尽量控制对象的大小，以便适配请求域和应用域；

<br>

### 跳转与转发

#### 转发一次请求

我们目前要实现的效果：

1. AServlet 把当前时间封装到 request 内并发送给 BServlet
2. BServlet 获取 AServlet 传递过来的 request，那到时间并输出

<br>

首先我们处理 BServlet 的代码：

使用 `getAttribute` 方法获取到 request 中存储的参数

```java
public class BServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        // 获取传递过来的参数
        Object time = req.getAttribute("systime");

        // 打印出来
        resp.setContentType("text/html");
        PrintWriter writer = resp.getWriter();
        writer.println(time);
    }
}
```

<br>

之后处理 AServlet：

想要把当前 Servlet 中的 request 传递给下一个 Servlet 请按两步走：

1. 获取下一 Servlet 请求转发器对象 RequestDispatcher（getRequestDispatcher 中的参数填写下一 Servlet 的 url）
2. 调用 RequestDispatcher 的 forward 方法将 request 传递下去

```java
public class AServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        Date time = new Date();

        // 按照键值对的方式设置存储到request中的参数值
        req.setAttribute("systime", time);

        // 第一步：获取请求转发器对象RequestDispatcher
        RequestDispatcher requestDispatcher = req.getRequestDispatcher("/bs");
        // 第二步：调用forward方法将request传递下去
        requestDispatcher.forward(req, resp);
    }
}
```

一般我们会把转发过程浓缩为一行代码：

`req.getRequestDispatcher("/bs").forward(req, resp);`

<br>

> 事实上，你可以吧 AServlet 理解为一个中间件，而 forward 方法可以等同于 express.js 中的 next 方法

<br>

#### 转发要求

转发目标不一定是 servlet，他可以是任意一个 tomcat 所承认的资源（譬如 html）；

但请注意转发路径不可以包含项目名称！

譬如我在 web 下新建 login.html ，那么转发路径就是 `/login.html`

<br>

#### 转发区别

getParameter 方法，获取的是由浏览器提交的表单的数据；

getAttribute 方法，获取的是请求域中绑定的数据；

<br>

### request 常见方法

#### 设置字符集

在 tomcat10 之前，默认字符集并非 UTF-8，直接使用 GET 或者 POST 获取到的数据都是乱码，所以需要手动设置；

修改请求 request 乱码问题：  
`req.setCharacterEncoding("UTF-8");`

修改响应 response 乱码问题：  
`resp.setContentType("text/html;charset=UTF-8");`

<br>

#### 几种常见属性获取方式

```java
// 动态获取应用根路径
String contextPath = req.getContextPath();

// 获取请求方式
String method = req.getMethod();

// 获取请求的URI
String requestURI = req.getRequestURI();

// 获取servlet路径
String servletPath = req.getServletPath();
```

<br>
