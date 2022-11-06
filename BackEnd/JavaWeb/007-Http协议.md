### Http 协议解析

#### 缓存机制

堆内存中的`字符串常量池`：遇到一个字符串首先在字符串常量池中查找；

堆内存当中的`整数型常量池`：超出 -128~127 这个范围的整数，都会从整数型常量池中获取

连接池：提前创建 N 个 connection 连接对象并放到一个集合中，当用户需要链接时就从里面取；

线程池：tomcat 启动时会创建 N 个线程对象并放到集合中，当用户发送一个请求来后就动用其中一个线程来处理；

<br>

### HttpServlet 源码

#### http 包下的接口

`HttpServlet`：HTTP 协议专用的 servlet，他是一个抽象类；  
`HttpServletRequest`：HTTP 专用请求对象，可通过它获取请求协议中的数据；  
`HttpServletResponse`：HTTP 专用响应对象，可通过它获取请求协议中的数据；

<br>

#### HttpServlet 执行全过程

执行无参构造函数；  
执行有参 init 方法，设置 config 变量；  
执行无参 init 方法；

<br>

下面请看 service 方法的官方源码解析

1. 首先在主方法内进行向下转型
2. 调用重载的 service 方法
3. 重载的 service 中仅提供算法框架，而具体的实现则由子类完成！

```java
// 主方法
public void service(ServletRequest req, ServletResponse res) throws ServletException, IOException {
    HttpServletRequest request;
    HttpServletResponse response;
    try {
        // 将标准的Servlet请求响应对象进行向下转型
        request = (HttpServletRequest)req;
        response = (HttpServletResponse)res;
    } catch (ClassCastException var6) {
        throw new ServletException(lStrings.getString("http.non_http"));
    }

    // 调用重载的service方法
    this.service(request, response);
}

// 重载方法
protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
    // 获取请求的方法，可能是GET/POST/PUT/DELETE等等
    String method = req.getMethod();

    long lastModified;

    // 如果请求方法为GET，则执行以下内容（反之执行else if内的内容）
    if (method.equals("GET")) {
        lastModified = this.getLastModified(req);
        if (lastModified == -1L) {
            this.doGet(req, resp);
        } else {
            long ifModifiedSince;
            try {
                ifModifiedSince = req.getDateHeader("If-Modified-Since");
            } catch (IllegalArgumentException var9) {
                ifModifiedSince = -1L;
            }

            if (ifModifiedSince < lastModified / 1000L * 1000L) {
                this.maybeSetLastModified(resp, lastModified);
                this.doGet(req, resp);
            } else {
                resp.setStatus(304);
            }
        }
    } else if (method.equals("HEAD")) {
        lastModified = this.getLastModified(req);
        this.maybeSetLastModified(resp, lastModified);
        this.doHead(req, resp);
    } else if (method.equals("POST")) {
        this.doPost(req, resp);
    } else if (method.equals("PUT")) {
        this.doPut(req, resp);
    } else if (method.equals("DELETE")) {
        this.doDelete(req, resp);
    } else if (method.equals("OPTIONS")) {
        this.doOptions(req, resp);
    } else if (method.equals("TRACE")) {
        this.doTrace(req, resp);
    } else {
        String errMsg = lStrings.getString("http.method_not_implemented");
        Object[] errArgs = new Object[]{method};
        errMsg = MessageFormat.format(errMsg, errArgs);
        resp.sendError(501, errMsg);
    }

}
```

<br>

#### 重写对应请求方法

因为根据上一节所学，重载的 service 会对不同的请求方式调用不同的方法进行处理；  
倘若我们需要针对 GET 或者 POST 请求做出不同响应的话，仅需要重写对应的方法即可！

```java
public class HeyServlet extends HttpServlet {

    // 重写get方法相关实现
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        super.doGet(req, resp);
    }

    // 重写post方法相关实现
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        super.doPost(req, resp);
    }
}
```

<br>

**注意事项**

1. 如果你不对对应请求方法进行重写的话，那么该请求无效！
2. 譬如你想对该 servlet 使用 GET 请求，但没有重新 doGet 方法，那么会返回 405 错误
3. 一般采取一对一方式重写，譬如前端请求 GET 则后端对应重写 doGet；
4. 如果前端仅请求 GET 时，不建议请求同时重写 doGet 和 doPost，这样可能会造成混淆（因为无法显示 405 错误界面）

> 因为父类 HttpServlet 中的默认方法（如 doGet 方法）返回的就是一个错误页面

<br>

### Web 欢迎页面

#### 快速设置一个开始页面

在根目录 web 下新建文件 `login.html`

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
    <h1>123456</h1>
  </body>
</html>
```

<br>

在 web.xml 中注册欢迎页面；

特别注意：`welcome-file` 中的欢迎页路径最前面不要加 `“/”`，也不要加根路径

```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="https://jakarta.ee/xml/ns/jakartaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="https://jakarta.ee/xml/ns/jakartaee https://jakarta.ee/xml/ns/jakartaee/web-app_5_0.xsd"
         version="5.0">

    <welcome-file-list>
        <!-- 路径前面不要加"/"，也不要加根路径！！！ -->
        <welcome-file>login.html</welcome-file>
    </welcome-file-list>

</web-app>
```

<br>

> 重启 tomcat 后，直接访问根目录即可打开该欢迎页面

<br>

#### index.html

`index.html & index.htm & index.jsp` 都是 tomcat 预先全局注册好的默认欢迎页面，所以不需要我们再次在 web.xml 中进行注册；

局部注册：webapp/WEB-INF/web.xml  
全局注册：tomcat 根目录/conf/web.xml

> 欢迎页可以是一个 servlet 或者任意一个静态资源

<br>
