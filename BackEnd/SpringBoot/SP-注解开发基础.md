### JavaConfig

#### 配置 maven 工程

新建一个空项目，再创建一个 maven 模块（不要使用 spring 初始化器创建！）

添加如下依赖

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>org.zhiyiyi</groupId>
    <artifactId>prev01</artifactId>
    <version>1.0-SNAPSHOT</version>
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <!-- 插件的版本 -->
                <version>3.8.1</version>
                <!-- 编译级别 -->
                <configuration>
                    <source>1.8</source>
                    <target>1.8</target>
                    <!-- 编码格式 -->
                    <encoding>UTF-8</encoding>
                </configuration>

            </plugin>
        </plugins>
    </build>

    <dependencies>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-aop</artifactId>
            <version>5.3.22</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>5.3.22</version>
        </dependency>

        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.12</version>
            <scope>test</scope>
        </dependency>

    </dependencies>

</project>
```

<br>

#### 创建数据类+bean 注册

在 main 包下新建一个 Student.java 数据类型，添加如下代码

自行添加 getter 和 setter 方法

```java
public class Student {
    private Integer id;
    private String name;
    private Integer age;
}
```

<br>

resource 文件夹下新建 beans.xml

按照标准 bean 格式书写如下代码，进行数据类型的初始化（或者说注册）

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="myStudent" class="com.zhiyiyi.prev.Student">
        <property name="id" value="1001"/>
        <property name="name" value="李强国"/>
        <property name="age" value="20"/>
    </bean>

</beans>
```

<br>

#### ApplicationContext 获取 bean

编写测试类，获取 beans.xml

获取 spring 容器，取出其中的对象并输出

```java
@Test
public void t1() {
    String config = "beans.xml";
    ApplicationContext context = new ClassPathXmlApplicationContext(config);
    Student student = (Student) context.getBean("myStudent");
    System.out.println("容器中对象：" + student);
}
```

<br>

#### SpringConfig 注册

使用此方法直接在 java 中注册 bean，而非编写 xml 文件进行注册

目前介绍两种方式

1. 无参数注册 bean，调用时直接调用方法
2. 有参注册 bean，调用时出入注册参数（即 bean 名称）

```java
package com.zhiyiyi.config;

import com.zhiyiyi.prev.Student;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class SpringConfig {

    // 无参构造bean
    @Bean
    public Student initStudent() {
        Student student = new Student();
        student.setAge(14);
        student.setName("张三");
        return student;
    }

    // 有参构造bean
    @Bean(name = "listStudent")
    public Student createStudent(){
        Student student = new Student();
        student.setAge(14);
        student.setName("张三");
        return student;
    }
}
```

<br>

这里不再从 beans.xml 中获取容器对象，而改为从 springconfig 中获取容器

然后从该容器中的方法 initStudent 中获得 bean

```java
@Test
public void t2() {
    ApplicationContext context = new AnnotationConfigApplicationContext(SpringConfig.class);
    Student student = (Student) context.getBean("initStudent");
    System.out.println("使用springconfig创建的：" + student);
}
```

<br>

### application.properties

> 新创建的 springboot 项目的 resource 目录下默认有一个 `application.properties` 文件，他用来管理整个 springboot 项目的全局属性

<br>

#### 配置文件解析

配置文件可以有两种格式，一种是 `properties` ，一种是 `yml`

在高版本 springboot 中，如果存在两个同名但格式不同的配置文件时，默认`优先选择yml格式`文件

以下为 yml（或者后缀可以改为 yaml，使用起来是一样的）代码：

```yml
server:
  port: 10086
```

<br>

#### 多环境配置

一般的，我们很可能在测试、开发、正式等多个环境下会使用不同的 application 配置文件，应当使用以下解决方法

新建生产环境下 application 文件：application-dev.properties  
在主 application 文件内填入如下配置，即可在服务器运行时改用 dev 配置文件  
`spring.profiles.active=dev`

<br>

#### @Value

使用 `@Value("${xxx}")` 从配置文件中取出配置项

譬如我们在 application.properties 文件中插入 school 对象，该对象包含俩参数

```properties
school.name=university
school.address=beijing
```

则编写 controller 时，直接使用@Value 注解，即可自动填充指定变量（即直接执行 setter 方法）

```java
@Controller
public class SchoolController {

    @Value("${school.name}")
    private String name;

    ...
}
```

<br>

#### ConfigurationProperties

用于将 java 文件映射成为一个对象，适用于配置项极多的情况，能省去@Value 的书写

首先需要添加依赖

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-configuration-processor</artifactId>
    <optional>true</optional>
</dependency>
```

假设我们保持配置项如上一节一样不变

新建 SchoolInfo.java ，在里面添加我们在配置文件中配置的属性

ConfigurationProperties 中属性 prefix 表示提取配置文件中的那一个对象

```java
@Component
@ConfigurationProperties(prefix = "school")
public class SchoolInfo {

    private String name;
    private String address;
    private String website;

    // 在这里使用快捷代码生成的方法自行添加getter、setter
}

```

<br>

提取完毕后，直接在 controller 中使用 Resource 注解，把整个类当做对象使用即可

RequestMapping 表示设置服务端响应路径，我们后面会讲

```java
@Controller
public class SchoolController {
    @Resource
    private SchoolInfo schoolInfo;

    @RequestMapping("/school")
    @ResponseBody
    public String getSchool() {
        return schoolInfo.toString();
    }
}
```

<br>

### Application

#### ApplicationContext

> 通过全局注册服务，并在 ApplicationContext 中通过该服务取得对应 bean

编写服务接口 UserService.java

```java
public interface UserService {
    void sayHello(String name);
}
```

实现服务接口，记得带上注解@Service，该注解接收一个参数表示该 bean 的名称

```java
@Service("userService")
public class UserServiceImpl implements UserService {
    @Override
    public void sayHello(String name) {
        System.out.println(name);
    }
}
```

<br>

在主入口中先获取 ApplicationContext 对象，再通过它获取并构造服务对象

```java
@SpringBootApplication
public class Prev02Application {
    public static void main(String[] args) {
        // ApplicationContext是基于本application构造的，注意run方法赋予的参数值！
        ApplicationContext run = SpringApplication.run(Prev02Application.class, args);
        UserService service = (UserService) run.getBean("userService");
        service.sayHello("tom");
    }
}
```

<br>

#### CommandLineRunner

一个接口，在容器对象构造完毕后执行其中的 run 方法；

我们可以直接在主入口实现该接口，然后重写 run

```java
@SpringBootApplication
public class Prev03Application implements CommandLineRunner {

    public static void main(String[] args) {
        SpringApplication.run(Prev03Application.class, args);
    }

    @Override
    public void run(String... args) throws Exception {
        System.out.println("我将在容器对象构造完毕后执行");
    }
}
```

<br>

#### 拦截器

顾名思义，概念定义懒得说了，自己搜。。。

拦截有三大步骤

1. 定义拦截器类，继承接口 HandlerInterceptor 以实现自定义拦截器
2. 编写带注解@Configuration 的配置类，使其实现接口 WebMvcConfigurer
3. 在配置类中使用 addInterceptors 添加拦截器即可

<br>

首先编写自定义拦截器类 LoginInterceptor，我们实现了一个方法 preHandle，即当拦截到时响应的信息

```java
public class LoginInterceptor implements HandlerInterceptor {
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        System.out.println("login interceptor");
        return true;
    }
}
```

<br>

定义配置项类

addInterceptor 用于注册指定拦截器对象  
addPathPatterns 添加要拦截的路径  
excludePathPatterns 添加不需要拦截的路径

```java
@Configuration
public class AppConfiguration implements WebMvcConfigurer {
    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        HandlerInterceptor interceptor = new LoginInterceptor();
        registry.addInterceptor(interceptor)
                .addPathPatterns("/user/**")
                .excludePathPatterns("/user/login");
    }
}
```

> 至于 controller 这些你自己下去实现就好了

<br>

### Servlet

> 在 springboot 中使用 servlet

首先创建文件 `MyServlet.java` 定义 servlet 对象

```java
public class MyServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        doPost(req, resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        resp.setContentType("text/html;charset=utf-8");
        PrintWriter writer = resp.getWriter();
        writer.println("这是一个servlet");
    }
}
```

<br>

新建 servlet 配置类，使用 `ServletRegistrationBean` 注册 servlet

```java
@Configuration
public class ServletConfiguration {
    @Bean
    public ServletRegistrationBean servletRegistrationBean() {
        ServletRegistrationBean bean = new ServletRegistrationBean(
                new MyServlet(),
                "/myservlet"
        );
        return bean;
    }
}
```

或者使用无参构造来注册 servlet

```java
@Configuration
public class ServletConfiguration {
    @Bean
    public ServletRegistrationBean servletRegistrationBean() {
        ServletRegistrationBean bean = new ServletRegistrationBean();
        bean.setServlet(new MyServlet());
        bean.addUrlMappings("/login", "/test");
        return bean;
    }
}
```

> 之后运行服务器，直接填入 url 即可使用此 bean

<br>

### Filter

#### 标准过滤器

创建自定义的过滤器类，注意导入的过滤器包为 servlet 下属的包！！！

```java
package com.example.prev03.web;

import javax.servlet.*;
import java.io.IOException;

public class MyFilter implements Filter {
    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {
        System.out.println("进入过滤器");
        // 向后链式传递过滤器
        filterChain.doFilter(servletRequest, servletResponse);
    }
}
```

<br>

编写过滤器配置类，这一步和 servlet 注册极其相似或者可以说一模一样

```java
@Configuration
public class FilterConfiguration {
    @Bean
    public FilterRegistrationBean filterRegistrationBean() {
        FilterRegistrationBean bean = new FilterRegistrationBean();
        bean.setFilter(new MyFilter());
        bean.addUrlPatterns("/user/**");
        return bean;
    }
}
```

<br>

#### CharacterEncodingFilter

由于可能存在的乱码问题，通过 CharacterEncodingFilter 来设置过滤器编码方式，从而使其正常显示汉字

```java
@Bean
public FilterRegistrationBean charsetBean() {
    FilterRegistrationBean reg = new FilterRegistrationBean();
    CharacterEncodingFilter encodingFilter = new CharacterEncodingFilter();
    encodingFilter.setEncoding("utf-8");
    encodingFilter.setForceEncoding(true);
    reg.setFilter(encodingFilter);
    reg.addUrlPatterns("/filter");
    return reg;
}
```

#### 配置文件注册字符集过滤器

我们可以完全丢弃上一个方法，直接在 application.properties 文件中添加三行配置项即可达到纠正过滤器乱码的效果

```properties
server.servlet.encoding.enabled=true
server.servlet.encoding.charset=UTF-8
server.servlet.encoding.force=true
```

<br>
