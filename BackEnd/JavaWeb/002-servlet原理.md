### request 和 response 过程

`request` browser 使用 request 请求 tomcat，tomcat 根据 servlet 请求 webapp

`response` webapp 返回给 tomcat，tomcat 再 response 给 browser

<br>

### 简单模拟 servlet

> 以下所有文件都在同一个目录下，目录结构：  
> Servlet.java  
> BankServlet.java
> Tomcat.java
> web.properties

<br>

#### 创建 servlet 规范

因为 tomcat 请求 webapp 时需要遵循 servlet 规范；

每个 webapp 都遵循 `servlet 规范`，代码中演示为所有 webapp 都实现了 servlet 接口！

定义一个 servlet 接口->

```java
package com.zhiyiyi.servlet;

public interface Servlet {
    void service();
}
```

<br>

#### 定义一个 webapp

该 webapp 实现了接口 servlet，并实现方法 service

```java
package com.zhiyiyi.servlet;

public class BankServlet implements Servlet{
    @Override
    public void service() {
        System.out.println("bank servlet");
    }
}

```

<br>

#### 设置路由关系

创建配置文件 web.properties

根据路由映射关系，将路由指向指定的 webapp

> ps: 该配置文件的名称不能变，必须要这样写！

```
/bank=com.zhiyiyi.servlet.BankServlet
```

<br>

#### 简单的 tomcat 服务器

对以下代码做出解释：

1. 获取并存储用户请求的 webapp 路径
2. 根据该路径，通过配置文件映射关系找到 webapp
3. 使用反射，实例化 webapp
4. 开始 webapp 的使用与交互

> 某些情况下 intellij 的 filereader 方法默认的读取路径并不在当前运行的文件的目录下，所以你可能需要使用绝对路径！

```java
package com.zhiyiyi.servlet;

import java.io.FileReader;
import java.util.Properties;
import java.util.Scanner;

public class Tomcat {
    public static void main(String[] args) throws Exception {
        // 服务器开启时输出
        System.out.println("启动tomcat");
        System.out.println("路由名：");

        // 读取用户输入的路由路径
        Scanner scanner = new Scanner(System.in);
        String link = scanner.nextLine();

        // 读取本地配置文件，获取路由指向的webapp
        FileReader reader = new FileReader("web.properties");
        Properties properties = new Properties();
        properties.load(reader);
        reader.close();

        // 反射构造指定的webapp，然后执行对应的方法
        String className = properties.getProperty(link);
        Class clazz = Class.forName(className);
        // 构造实现类，然后执行方法
        Servlet servlet = (Servlet) clazz.getConstructor().newInstance();
        servlet.service();
    }
}

```

<br>

### 定义带 Servlet 的 webapp

#### 一个完整可移植的 webapp 目录结构

在 tomcat 根目录下的 webapps 文件夹内，新建一个 webapp，名字任意取；

这是该 webapps 标准的文件结构

- WEB-INF
- classes 存放编译好的 java 文件字节码
- lib 存放第三方 jar 包，该文件夹不是必须
- web.xml 必须，存放路由映射

> 注意，所有目录名称都必须这么写，不能取别的名字！！！🧐  
> 一般情况下，只需要从别的 webapps 复制照葫芦画瓢就好了

<br>

```
└─WEB-INF
    ├─classes
    └─lib
    |_web.xml
```

#### JavaEE 变革

- 由于被 oracle 收购，JavaEE8 是目前的旧版的最高版本，而以后的版本名称都变成了 JakartaEE
- 也就是说，以后就叫做 JakartaEE9，与此同时对应的 servlet 包名也变了！
- tomcat10+版本都使用 jakarta，而 tomcat9-的版本还是旧版 javaee，注意代码迁移！

<br>
