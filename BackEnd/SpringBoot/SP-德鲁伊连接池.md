### druid 基本使用

#### 导入依赖

这个版本可以去阿里官网看

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-validation</artifactId>
</dependency>

<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>druid</artifactId>
    <version>1.1.21</version>
</dependency>
```

<br>

#### 使用 druid

首先进行设置好数据源，在 application.yaml 中配置

type 指定的是连接池类型，我们改用 druid

最后的四个参数时 druid 中自带的属性，我们需要将这些属性注入到 druid 数据源里面去！

```yaml
spring:
  datasource:
    username: root
    password: 123456
    url: jdbc:mysql://localhost:3306/springboot?serverTimezone=UTC&useUnicode=true&characterEncoding=utf-8
    driver-class-name: com.mysql.cj.jdbc.Driver
    type: com.alibaba.druid.pool.DruidDataSource

    initialSize: 5
    minIdle: 5
    maxActive: 20
    maxWait: 60000
```

<br>

创建配置类，将 application.yaml 中数据源配置注入到德鲁伊数据源

```java
@Configuration
public class DruidConfig {

    @ConfigurationProperties(prefix = "spring.datasource")
    @Bean
    public DataSource druidDB() {
        return new DruidDataSource();
    }
}
```

<br>

在测试类中我们先自动关联数据源类型 DataSource

使用向下转型，获取到我们的德鲁伊数据源

```java
@SpringBootTest
public class DatabaseTest {

    @Autowired
    private DataSource dataSource;

    @Test
    public void druidTest() throws Exception {
        System.out.println(dataSource.getClass());
        //获得连接
        Connection connection = dataSource.getConnection();
        System.out.println(connection);

        // 向下转型，获取我们刚刚注入的德鲁伊数据源
        DruidDataSource druidDataSource = (DruidDataSource) dataSource;

        System.out.println("druidDataSource 数据源最大连接数：" + druidDataSource.getMaxActive());

        //关闭连接
        connection.close();
    }
}
```

> 后续的 JDBC 操作不再赘述

<br>

### druid 后台监控

#### 基础监控

根目录下新增文件夹 bean，在其中新建类 `DruidRegBean.java`

为其添加组件注解，方便直接注入到 application

ServletRegistrationBean 是 springboot 专属的注册 servlet 的方法！

```java
@Component
public class DruidRegBean {
    // springboot专有的servlet注入方法
    @Bean
    public ServletRegistrationBean statViewServlet() {

        // 使用StatViewServlet构建后台监控系统
        ServletRegistrationBean bean = new ServletRegistrationBean(new StatViewServlet(), "/druid/*");

        // 在这里配置后天监控系统的登录设置
        Map<String, String> initParams = new HashMap<>();
        initParams.put("loginUsername", "admin"); //后台管理界面的登录账号
        initParams.put("loginPassword", "123456"); //后台管理界面的登录密码

        //后台允许谁可以访问
        //initParams.put("allow", "localhost")：表示只有本机可以访问
        //initParams.put("allow", "")：为空或者为null时，表示允许所有访问
        initParams.put("allow", "");

        //设置初始化参数
        bean.setInitParameters(initParams);
        return bean;
    }
}
```

<br>
