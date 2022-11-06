### 基本概念

#### servlet 关系

servlet 和 servletconfig 是一对一的关系；

<br>

#### servletconfig 作用

它存储 web.xml 内的 servlet 标签内的所有信息；

tomcat 在解析 web.xml 中的标签时，就会自动把 servlet 标签包装到 servletconfig 里面去；

<br>

### 使用

#### 获取 config 基础信息

首先我们手动创建一个 ConfigServlet，注册流程这里省略了；

这里为了方便，直接选择继承 GenericServlet；

```java
public class ConfigServlet extends GenericServlet {
    @Override
    public void service(ServletRequest servletRequest, ServletResponse servletResponse) throws ServletException, IOException {
        // 设置请求头
        servletResponse.setContentType("text/html");
        PrintWriter writer = servletResponse.getWriter();

        // 获取本servlet的servletconfig
        ServletConfig config = this.getServletConfig();

        // 打印本config的真实对象名称
        writer.println("servletconfig: " + config.toString() + "<br>");

        // 获取当前servletconfig所管辖的servlet的名称
        String configName = config.getServletName();
        writer.println("名字是：" + configName);
    }
}
```

<br>

#### 携带信息

我们可以在 web.xml 中的 `servlet` 标签下，使用 `init-param` 夹带信息

所夹带的信息可以通过 servletconfig 进行获取，夹带信息数量不限；

```xml
<servlet>
    <servlet-name>configServlet</servlet-name>
    <servlet-class>com.zhiyiyi.javaweb.servlet.ConfigServlet</servlet-class>

    <init-param>
        <param-name>user</param-name>
        <param-value>root</param-value>
    </init-param>
</servlet>
```

<br>

使用 `servletconfig` 获取信息：

目前获取 `init-param` 中的信息有以下两种方式：

1. 使用 getInitParameterNames 获取所有参数名称后再一一拿出 value
2. 使用 getInitParameter 精准指定参数名后获取 value

```java
// 使用枚举的方法获取到所有参数名称
Enumeration<String> initParameterNames = config.getInitParameterNames();
// 迭代输出获取到的参数名称
while (initParameterNames.hasMoreElements()) {
    String name = initParameterNames.nextElement();
    writer.println(name + "<br>");
}

// 直接指定需要获取的参数名称，精准拿到value
String userName = config.getInitParameter("user");
writer.println(userName + "<br>");
```

<br>
