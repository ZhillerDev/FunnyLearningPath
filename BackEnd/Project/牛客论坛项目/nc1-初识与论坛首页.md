## Spring 入门

### SpringInitializr

IDEA 专业版自带的功能，也可以直接搜索对应网站，通过网站生成包后导入 IDEA 中使用

下面是初始化的参数：

- 选用 SpringBoot2.7.15
- 导入依赖 Web、DevTools、Thymeleaf

之后等待构建完毕即可

<br>

### ApplicationContextAware

在测试函数中使用 ApplicationContextAware 来获取应用上下文

```java
package com.zhiller.community.zhillercommunity;

import com.zhiller.community.zhillercommunity.dao.AlphaDao;
import org.junit.jupiter.api.Test;
import org.springframework.beans.BeansException;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;
import org.springframework.test.context.ContextConfiguration;

@SpringBootTest
// @SpringBootTest 是 Spring Boot 提供的测试注解，用于表示这是一个 Spring Boot 的测试类。
// 它会自动加载 Spring Boot 的配置，并启动 Spring 容器，以便进行集成测试。
@ContextConfiguration(classes = ZhillerCommunityApplication.class)
// @ContextConfiguration 用于指定 Spring 容器的配置信息。
// 这里使用 ZhillerCommunityApplication.class 表示使用该类所在的包作为配置信息。
class ZhillerCommunityApplicationTests implements ApplicationContextAware {

    private ApplicationContext applicationContext;

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        this.applicationContext = applicationContext;
    }
    // 实现 ApplicationContextAware 接口的方法，用于获取 Spring 应用上下文对象。

}
```

<br>

### Controller

创建 controller 文件

```java
package com.zhiller.community.zhillercommunity.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
@RequestMapping("/alpha")
public class AlphaController {

    // 处理"/alpha/helloworld"请求，返回字符串响应
    @RequestMapping("/helloworld")
    // @ResponseBody注解表示将方法的返回值直接作为响应体返回，而不是将其解析为视图名称
    @ResponseBody
    public String hello(){
        return "shit man!";
    }
}
```

<br>

运行 Application，之后打开浏览器，输入 `localhost:10086/alpha/helloworld` 获取请求结果

<br>

### DAO

主应用包下新建两个文件，分别是 DAO 接口以及其对应实现类

代码清单：AlphaDao.java

```java
package com.zhiller.community.zhillercommunity.dao;

public interface AlphaDao {
    String select();
}
```

代码清单：AlphaDaoImpl.java

```java
package com.zhiller.community.zhillercommunity.dao;

import org.springframework.context.annotation.Primary;
import org.springframework.stereotype.Repository;

@Repository
// @Repository 是 Spring 提供的注解，用于表示该类是一个数据访问对象（DAO）。
// 它会被 Spring 自动扫描并将其注册为 Spring 容器中的一个 Bean。
@Primary
// @Primary 是 Spring 提供的注解，用于表示当存在多个同一类型的 Bean 时，优先选择被标记为 @Primary 的 Bean。
// 在自动装配时，如果没有明确指定要注入哪个 Bean，将会选择被标记为 @Primary 的 Bean。
public class AlphaDaoImpl implements AlphaDao {
    // AlphaDao 是一个接口，AlphaDaoImpl 类实现了该接口。

    @Override
    public String select() {
        return "hibernate";
    }
    // 实现了 AlphaDao 接口中的 select() 方法，返回字符串 "hibernate"。
}
```

<br>

最后在主测试类中对这个 DAO 进行测试

```java
@SpringBootTest
@ContextConfiguration(classes = ZhillerCommunityApplication.class)
class ZhillerCommunityApplicationTests implements ApplicationContextAware {

    private ApplicationContext applicationContext;

    @Override
    public void setApplicationContext(ApplicationContext applicationContext) throws BeansException {
        this.applicationContext = applicationContext;
    }

    @Test
    public void testContext(){
        System.out.println(applicationContext);
        AlphaDao alphaDao = applicationContext.getBean(AlphaDao.class);
        System.out.println(alphaDao.select());
    }
}
```

<br>

### DAO 名称索引

对 DAO 实现类的注解`@Repository`添加一段字符串描述，即可直接通过该字符串获取该 DAO

```java
@Repository("alpha_hibernate")
@Primary
public class AlphaDaoImpl implements AlphaDao {
    @Override
    public String select() {
        return "hibernate";
    }
}
```

故对应的测试类可以这么写

```java
@Test
public void testContext(){
    System.out.println(applicationContext);
    AlphaDao alphaDao = applicationContext.getBean("alpha_hibernate",AlphaDao.class);
    System.out.println(alphaDao.select());
}
```

<br>

### Service

创建 service 包，下含一个 Service 类

代码清单 `AlphaService.java`

```java
package com.zhiller.community.zhillercommunity.service;

import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;

@Service
// @Service 是 Spring 提供的注解，用于表示该类是一个服务类。
// 它会被 Spring 自动扫描并将其注册为 Spring 容器中的一个 Bean。
@Scope("prototype")
// @Scope 是 Spring 提供的注解，用于指定 Bean 的作用域。
// 这里的 "prototype" 表示每次请求该 Bean 都会创建一个新的实例。
public class AlphaService {
    public AlphaService() {
        System.out.println("实例化service");
        // 在构造函数中打印一条实例化的信息。
    }

    @PostConstruct
    // @PostConstruct 是 Java 的注解，用于指定在构造函数执行后执行的方法。
    // 在该方法上添加 @PostConstruct 注解后，Spring 会在构造函数执行完毕后调用该方法。
    public void init(){
        System.out.println("初始化service");
        // 在 init() 方法中打印一条初始化的信息。
    }

    @PreDestroy
    // @PreDestroy 是 Java 的注解，用于指定在 Bean 销毁之前执行的方法。
    // 在该方法上添加 @PreDestroy 注解后，Spring 会在销毁 Bean 之前调用该方法。
    public void destory(){
        System.out.println("销毁service");
        // 在 destory() 方法中打印一条销毁的信息。
    }
}
```

对应的测试文件这里不在给出，使用方式和 DAO 完全一致

<br>

### Config

配置 Config 文件和 service、dao 如出一辙

代码清单 AlphaConfig.java

```java
package com.zhiller.community.zhillercommunity.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import java.text.SimpleDateFormat;

@Configuration
public class AlphaConfig {

    @Bean
    public SimpleDateFormat simpleDateFormat(){
        return new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    }
}
```

<br>

### 自动装配

springboot 提供了自动装配，我们无需获取 applicationcontext 就可以直接使用对应的实例了

```java
// 测试类中使用@Autowired和@Qualifier注解来自动装配依赖关系
@Autowired
// @Qualifier("alpha_hibernate")用于指定要注入的alphaDao的Bean名称为"alpha_hibernate"
@Qualifier("alpha_hibernate")
private AlphaDao alphaDao;

@Autowired
private AlphaService alphaService;
```

<br>

## 初识 SpringMVC

### Http 请求

为 `AlphaService.java` 修改代码

添加一个 http 请求，并通过该请求获取指定参数

```java
package com.zhiller.community.zhillercommunity.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Enumeration;

@Controller
@RequestMapping("/alpha")
public class AlphaController {

    /**
     * 处理映射到 "/alpha/http" 路径的请求
     * 打印请求的 HTTP 方法、请求头信息和请求参数
     * 设置响应内容类型为 text/html;charset=utf-8
     * 获取响应输出流并进行异常处理
     */
    @RequestMapping("/http")
    public void http(HttpServletRequest request, HttpServletResponse response) {
        // 打印请求的 HTTP 方法
        System.out.println(request.getMethod());

        // 获取所有请求头的名称并打印
        Enumeration<String> enumeration = request.getHeaderNames();
        while (enumeration.hasMoreElements()) {
            String name = enumeration.nextElement();
            String value = request.getHeader(name);
            System.out.println(name + ":" + value);
        }

        // 打印请求参数 "code"
        System.out.println(request.getParameter("code"));

        // 设置响应内容类型为 text/html;charset=utf-8
        response.setContentType("text/html;charset=utf-8");
        try (PrintWriter writer = response.getWriter();)
        {
            writer.write("<h1>zhiller labs</h1>");
        }catch(IOException e) {
            e.printStackTrace();
        }
    }
}
```

<br>

### GET

```java
package com.zhiller.community.zhillercommunity.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
@RequestMapping("/alpha")
public class AlphaController {
    /**
     * 处理映射到 "/alpha/getage" 路径的 GET 请求
     * 接受名为 "age" 的请求参数，如果参数不存在则使用默认值 1
     * 打印参数值并返回字符串 "age"
     */
    @GetMapping("/getage")
    @ResponseBody
    public String getAge(
            @RequestParam(name = "age", required = false, defaultValue = "1") int age
    ) {
        System.out.println(age);
        return "age";
    }

    /**
     * 处理映射到 "/alpha/getid/{id}" 路径的 GET 请求
     * 接受路径变量 "id"
     * 打印路径变量值并返回字符串 "get the id"
     */
    @GetMapping("/getid/{id}")
    @ResponseBody
    public String getId(
            @PathVariable("id") int id
    ) {
        System.out.println(id);
        return "get the id";
    }
}
```

<br>

### POST

随便写一个登录页面，存放位置：`resource/static/html/student.html`

```html
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<title>oh my god</title>
	</head>
	<body>
		<div class="main">look this big shit!!!</div>

		<!-- 要注意这里的请求方法为POST，请求url要和springboot里面定义的一致 -->
		<form
			method="post"
			action="/alpha/student"
		>
			<p>
				姓名：<input
					type="text"
					name="name"
					placeholder="请输入您的姓名"
				/>
			</p>
			<p>
				密码：<input
					type="password"
					name="pwd"
					placeholder="******"
				/>
			</p>
			<p>
				<input
					type="submit"
					value="保存"
				/>
			</p>
		</form>
	</body>
</html>

<style>
	.main {
		font-size: large;
		font-weight: bold;
	}
</style>
```

<br>

然后就是我们的 controller 方法了

```java
@PostMapping("/student")
public void postStudent(String name, String password) {
    System.out.println(name);
    System.out.println(password);
    return;
}
```

<br>

### HTML 渲染

thymeleaf 的作用是帮助我们后端渲染简单的前端界面

但是很明显，在现在前后端分类的大趋势下，这玩意就和当年的 JSP 一样没有前景

建议不学

<br>

### 响应 JSON 数据

```java
package com.zhiller.community.zhillercommunity.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.HashMap;
import java.util.Map;

@Controller
@RequestMapping("/alpha")
public class AlphaController {
    /**
     * 处理映射到 "/alpha/emp" 路径的 GET 请求
     * 返回一个包含员工信息的 Map 对象
     */
    @GetMapping("/emp")
    @ResponseBody
    public Map<String, Object> getEmp() {
        // 创建一个 Map 对象用于存储员工信息
        Map<String, Object> map = new HashMap<>();
        map.put("name", "张三");
        map.put("age", 123);
        return map;
    }
}
```

<br>

## MyBatis 入门

### 数据库配置

安装 mysql5.7 或者 mysql8 数据库都可以无所谓

然后找到牛客提供的数据库 sql 文件  
在该文件目录下打开命令行

依次输入：  
`mysql -u root -p`  
`create database community`  
`use community`  
`source ./init-schema.sql`

此时就自动把所有要用到的数据表导进去了，但是目前表中仍然没有任何数据，故需要添加对应数据

开启 navicat，连接到 community 数据库，再把 init_data.sql 拖拽进去就可以导入所有数据了

<br>

### 连接配置

你需要关注的就是下面几个配置项：

- spring.datasource.url
- spring.datasource.username
- spring.datasource.password
- spring.datasource.driver-class-name

代码清单：`application.properties`

```properties
server.port=10086
# 指定应用的端口号为 10086

server.servlet.context-path=/community
# 指定应用的上下文路径为 "/community"，即应用的根路径为 "/community"

# ThymeleafProperties
spring.thymeleaf.cache=false
# 配置 Thymeleaf 模板引擎的缓存为关闭状态，方便开发阶段的模板修改和调试

# DataSourceProperties
spring.datasource.driver-class-name=com.mysql.jdbc.Driver
# 数据库驱动类

spring.datasource.url=jdbc:mysql://localhost:3306/community?characterEncoding=utf-8&useSSL=false&serverTimezone=Hongkong
# 数据库连接URL，指定数据库为localhost的3306端口上的community数据库，使用UTF-8字符编码，禁用SSL，时区设置为Hongkong

spring.datasource.username=root
# 数据库用户名

spring.datasource.password=123456
# 数据库密码

spring.datasource.type=com.zaxxer.hikari.HikariDataSource
# 数据源类型为HikariDataSource，使用HikariCP连接池

spring.datasource.hikari.maximum-pool-size=15
# 连接池的最大连接数为15

spring.datasource.hikari.minimum-idle=5
# 连接池的最小空闲连接数为5

spring.datasource.hikari.idle-timeout=30000
# 连接的最大空闲时间为30秒

# MybatisProperties
mybatis.mapper-locations=classpath:mapper/*.xml
# MyBatis映射文件的位置，使用classpath:mapper/*.xml表示在类路径下的mapper目录中查找映射文件

mybatis.type-aliases-package=com.zhiller.community.zhillercommunity.entity
# MyBatis实体类的包名，用于自动扫描实体类的别名

mybatis.configuration.useGeneratedKeys=true
# 配置MyBatis使用自动生成的主键

mybatis.configuration.mapUnderscoreToCamelCase=true
# 配置MyBatis将下划线命名的数据库字段映射为驼峰命名的Java属性
```

<br>

注意，如果你使用的是 mysql5，那么对应的数据连接池就要写  
`spring.datasource.driver-class-name=com.mysql.jdbc.Driver`  
如果是 mysql8，就写  
`spring.datasource.driver-class-name=com.mysql.cj.jdbc.Driver`

<br>

### User Model

定义查询对象

对象是一个 User 实体，它对应 community 数据库里的数据表 user

代码清单：`entity/User.java`

```java
package com.zhiller.community.zhillercommunity.entity;

import java.util.Date;

public class User {

    private int id;
    private String username;
    private String password;
    private String salt;
    private String email;
    private int type;
    private int status;
    private String activationCode;
    private String headerUrl;
    private Date createTime;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getSalt() {
        return salt;
    }

    public void setSalt(String salt) {
        this.salt = salt;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public String getActivationCode() {
        return activationCode;
    }

    public void setActivationCode(String activationCode) {
        this.activationCode = activationCode;
    }

    public String getHeaderUrl() {
        return headerUrl;
    }

    public void setHeaderUrl(String headerUrl) {
        this.headerUrl = headerUrl;
    }

    public Date getCreateTime() {
        return createTime;
    }

    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }

    @Override
    public String toString() {
        return "User{" +
                "id=" + id +
                ", username='" + username + '\'' +
                ", password='" + password + '\'' +
                ", salt='" + salt + '\'' +
                ", email='" + email + '\'' +
                ", type=" + type +
                ", status=" + status +
                ", activationCode='" + activationCode + '\'' +
                ", headerUrl='" + headerUrl + '\'' +
                ", createTime=" + createTime +
                '}';
    }

}
```

<br>

### UserMapper

配置 mybatis 的第一步就是需要定义 Mapper

mapper 用于设置操作数据库的方法

代码清单：`/dao/UserMapper.java`

```java
package com.zhiller.community.zhillercommunity.dao;

import com.zhiller.community.zhillercommunity.entity.User;
import org.apache.ibatis.annotations.Mapper;

@Mapper
public interface UserMapper {
    User selectById(int id);

    User selectByName(String username);

    User selectByEmail(String email);

    int insertUser(User user);

    int updateStatus(int id, int status);

    int updateHeader(int id, String headerUrl);

    int updatePassword(int id, String password);
}

```

<br>

### Mapper xml

既然设置了方法，那么就需要配置方法对应的 SQL 查询语句

mybatis 简化了这一操作，使用 xml 来配置所有的 SQL 语句  
一个 XML 配置文件对应一个 mapper 文件

<br>

代码清单：`resource/mapper/user-mapper.xml`

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
        PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.zhiller.community.zhillercommunity.dao.UserMapper">
    <!-- 定义插入字段 -->
    <sql id="insertFields">
        username, password, salt, email, type, status, activation_code, header_url, create_time
    </sql>

    <!-- 定义查询字段 -->
    <sql id="selectFields">
        id, username, password, salt, email, type, status, activation_code, header_url, create_time
    </sql>

    <!-- 根据id查询用户 -->
    <select id="selectById" resultType="User">
        select <include refid="selectFields"></include>
        from user
        where id = #{id}
    </select>

    <!-- 根据用户名查询用户 -->
    <select id="selectByName" resultType="User">
        select <include refid="selectFields"></include>
        from user
        where username = #{username}
    </select>

    <!-- 根据邮箱查询用户 -->
    <select id="selectByEmail" resultType="User">
        select <include refid="selectFields"></include>
        from user
        where email = #{email}
    </select>

    <!-- 插入用户 -->
    <insert id="insertUser" parameterType="User" keyProperty="id">
        insert into user (<include refid="insertFields"></include>)
        values(#{username}, #{password}, #{salt}, #{email}, #{type}, #{status}, #{activationCode}, #{headerUrl}, #{createTime})
    </insert>

    <!-- 更新用户状态 -->
    <update id="updateStatus">
        update user set status = #{status} where id = #{id}
    </update>

    <!-- 更新用户头像 -->
    <update id="updateHeader">
        update user set header_url = #{headerUrl} where id = #{id}
    </update>

    <!-- 更新用户密码 -->
    <update id="updatePassword">
        update user set password = #{password} where id = #{id}
    </update>
</mapper>
```

### 测试 MyBatis

依据主测试类编写专属于 mybatis 的测试类

如果下方代码运行成功，则证明数据库链接正确！

```java
package com.zhiller.community.zhillercommunity;

import com.zhiller.community.zhillercommunity.dao.UserMapper;
import com.zhiller.community.zhillercommunity.entity.User;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.Date;

@RunWith(SpringRunner.class)
@SpringBootTest
@ContextConfiguration(classes = ZhillerCommunityApplication.class)
public class MapperTest {
    @Autowired
    private UserMapper userMapper;

    @Test
    public void testSelectUser() {
        // 根据id查询用户
        User user = userMapper.selectById(101);
        System.out.println(user);

        // 根据用户名查询用户
        user = userMapper.selectByName("liubei");
        System.out.println(user);

        // 根据邮箱查询用户
        user = userMapper.selectByEmail("nowcoder101@sina.com");
        System.out.println(user);
    }

    @Test
    public void testInsertUser() {
        // 创建一个新用户对象
        User user = new User();
        user.setUsername("test");
        user.setPassword("123456");
        user.setSalt("abc");
        user.setEmail("test@qq.com");
        user.setHeaderUrl("http://www.nowcoder.com/101.png");
        user.setCreateTime(new Date());

        // 插入用户
        int rows = userMapper.insertUser(user);
        System.out.println(rows);
        System.out.println(user.getId());
    }

    @Test
    public void updateUser() {
        // 更新用户状态
        int rows = userMapper.updateStatus(150, 1);
        System.out.println(rows);

        // 更新用户头像
        rows = userMapper.updateHeader(150, "http://www.nowcoder.com/102.png");
        System.out.println(rows);

        // 更新用户密码
        rows = userMapper.updatePassword(150, "hello");
        System.out.println(rows);
    }
}
```

<br>

## 首页制作

### DiscussPost

根据上一节所述经验，我们应该能很快根据数据库中结构构建对应的 `entity` 类

故依葫芦画瓢，写出 `DiscussPost` 数据表的 `entity` 类

代码清单：`DiscussPost.java`

```java
package com.zhiller.community.zhillercommunity.entity;

import java.sql.Date;

public class DiscussPost {
    private int id;
    private int userId;
    private String title;
    private String content;
    private int type;
    private int status;
    private Date createTime;
    private int commentCount;
    private double score;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public Date getCreateTime() {
        return createTime;
    }

    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }

    public int getCommentCount() {
        return commentCount;
    }

    public void setCommentCount(int commentCount) {
        this.commentCount = commentCount;
    }

    public double getScore() {
        return score;
    }

    public void setScore(double score) {
        this.score = score;
    }

    @Override
    public String toString() {
        return "DiscussPost{" +
                "id=" + id +
                ", userId=" + userId +
                ", title='" + title + '\'' +
                ", content='" + content + '\'' +
                ", type=" + type +
                ", status=" + status +
                ", createTime=" + createTime +
                ", commentCount=" + commentCount +
                ", score=" + score +
                '}';
    }
}
```

<br>

既然我们要查询，则必然需要调用 mybatis

所以再创建一个 mapper 接口来表示我们将要使用到的 SQL 方法

代码清单：`/dao/DiscussPostMapper.java`

```java
package com.zhiller.community.zhillercommunity.dao;

import com.zhiller.community.zhillercommunity.entity.DiscussPost;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

@Mapper
public interface DiscussPostMapper {
    // 查询所有的DiscussPost
    List<DiscussPost> selectDiscussPosts(int userId, int offset, int limit);

    // @Param注解用于给参数取别名,
    // 如果只有一个参数,并且在<if>里使用,则必须加别名.
    int selectDiscussPostRows(@Param("userId") int userId);
}
```

<br>

别忘了在 resource 文件下注册对应的 mapper 配置文件

代码清单：`resource/mapper/discusspost-mapper.xml`

```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
        PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.zhiller.community.zhillercommunity.dao.DiscussPostMapper">
    <sql id="selectFields">
        id, user_id, title, content, type, status, create_time, comment_count, score
    </sql>

    <select id="selectDiscussPosts" resultType="DiscussPost">
        select
        <include refid="selectFields"/>
        from discuss_post
        where status != 2

        <!-- 注意这里的if用于检测是否填写了userId参数 -->
        <!-- 当我们在mapper接口中使用@Param定义自定义名称参数时，一定要使用if标签进行判断！ -->
        <if test="userId!=0">
            and user_id = #{userId}
        </if>
        order by type desc, create_time desc
        limit #{offset}, #{limit}
    </select>

    <select id="selectDiscussPostRows" resultType="int">
        select count(id)
        from discuss_post
        where status != 2

        <!-- 这里也是一样的，使用if标签判断 -->
        <if test="userId!=0">
            and user_id = #{userId}
        </if>
    </select>
</mapper>
```

<br>

编写对应测试方法

```java
package com.zhiller.community.zhillercommunity;

import com.zhiller.community.zhillercommunity.dao.DiscussPostMapper;
import com.zhiller.community.zhillercommunity.dao.UserMapper;
import com.zhiller.community.zhillercommunity.entity.DiscussPost;
import com.zhiller.community.zhillercommunity.entity.User;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.Date;
import java.util.List;

@RunWith(SpringRunner.class)
@SpringBootTest
@ContextConfiguration(classes = ZhillerCommunityApplication.class)
public class MapperTest {

    // 自动装配别忘了！！！
    @Autowired
    private DiscussPostMapper discussPostMapper;

    @Test
    public void testDiscussPost() {
        List<DiscussPost> discussPostList = discussPostMapper.selectDiscussPosts(0, 0, 10);
        for (DiscussPost discussPost : discussPostList) {
            System.out.println(discussPost);
        }

        int rows = discussPostMapper.selectDiscussPostRows(149);
        System.out.println(rows);
    }
}
```

<br>

### DiscussPostService

对应的 Service 实际上很简单，就和我们的测试方法一样，也是两步走：

1. autowired 自动装配 mapper
2. 调用 mapper 中的方法实现查询

代码清单：`/service/DiscussPostService.java`

```java
package com.zhiller.community.zhillercommunity.service;

import com.zhiller.community.zhillercommunity.dao.DiscussPostMapper;
import com.zhiller.community.zhillercommunity.entity.DiscussPost;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class DiscussPostService {
    @Autowired
    private DiscussPostMapper discussPostMapper;

    public List<DiscussPost> findDiscussPosts(int userId, int offset, int limit) {
        return discussPostMapper.selectDiscussPosts(userId, offset, limit);
    }

    public int findDiscussPostRows(int userId) {
        return discussPostMapper.selectDiscussPostRows(userId);
    }
}
```

<br>

### Controller

定制了完善的 entity、service，接下来就是使用 controller 控制界面的显示了

由于我们是后端开发，无需理会前端如何，除非你是全栈；  
前端部分由 thymeleaf 借助模板搞定，虽然没有做到前后端分离，但是也还是可以的；

<br>

代码清单：`/controller/HomeController.java`

```java
@Controller
public class HomeController {
    @Autowired
    private DiscussPostService discussPostService;
    @Autowired
    private UserService userService;

    @GetMapping("/index")
    public String getIndexPage(Model model, Page page) {
        // 方法调用钱,SpringMVC会自动实例化Model和Page,并将Page注入Model.
        // 所以,在thymeleaf中可以直接访问Page对象中的数据.

        // 分页查询实体Page我们还没有定义，此刻会飘红，先不管
        page.setRows(discussPostService.findDiscussPostRows(0));
        page.setPath("/index");

        // 获取所有DiscussPost
        List<DiscussPost> list = discussPostService.findDiscussPosts(0, page.getOffset(), page.getLimit());

        // discussPosts将会被用作列表渲染
        List<Map<String, Object>> discussPosts = new ArrayList<>();
        if (list != null) {
            for (DiscussPost post : list) {
                Map<String, Object> map = new HashMap<>();
                // post和user为对应参数，后续html文件里面会用到
                map.put("post", post);
                User user = userService.findUserById(post.getUserId());
                map.put("user", user);
                discussPosts.add(map);
            }
        }

        // 向html模板提供一个可用参数，参数指向discussPosts
        model.addAttribute("discussPosts", discussPosts);
        return "/index";
    }
}
```

<br>

### thymeleaf 模板参数介绍

下面是一些常见的模板介绍

`th:each="map:${discussPosts}`

- th:each 类似于 v-for，列表渲染
- map 为渲染子项的别名
- discussPosts 为被渲染的项

<br>

`th:src="${map.user.headerUrl}"`  
th:src 用于动态绑定图片地址  
既然我们已经确定了子项别名，故可以直接通过该别名取出对应数据

<br>

`th:utext="${map.post.title}`  
th:utext 是 Thymeleaf 的内联表达式，用于将表达式的结果作为不经过 HTML 转义的文本输出

<br>

`th:if="${map.post.type==1}`  
可以执行判断操作，动态显示或隐藏部分标签

<br>

`th:text="${#dates.format(map.post.createTime,'yyyy-MM-dd HH:mm:ss')}"`  
可以内含指令，来改变输出格式

<br>

### Page

> 开始制作分页查询模块

创建对应的 entity 类（注意！并不是说所有的 entity 都必须对应数据表，比如这个 page 就是为了索引用的）

代码清单：`entity/page.java`

```java
package com.zhiller.community.zhillercommunity.entity;

public class Page {
    // 当前页码
    private int current = 1;
    // 显示上限
    private int limit = 10;
    // 数据总数(用于计算总页数)
    private int rows;
    // 查询路径(用于复用分页链接)
    private String path;

    public int getCurrent() {
        return current;
    }

    public void setCurrent(int current) {
        if (current >= 1) {
            this.current = current;
        }
    }

    public int getLimit() {
        return limit;
    }

    public void setLimit(int limit) {
        if (limit >= 1 && limit <= 100) {
            this.limit = limit;
        }
    }

    public int getRows() {
        return rows;
    }

    public void setRows(int rows) {
        if (rows >= 0) {
            this.rows = rows;
        }
    }

    public String getPath() {
        return path;
    }

    public void setPath(String path) {
        this.path = path;
    }

    /**
     * 获取当前页的起始行
     *
     * @return
     */
    public int getOffset() {
        // current * limit - limit
        return (current - 1) * limit;
    }

    /**
     * 获取总页数
     *
     * @return
     */
    public int getTotal() {
        // rows / limit [+1]
        if (rows % limit == 0) {
            return rows / limit;
        } else {
            return rows / limit + 1;
        }
    }

    /**
     * 获取起始页码
     *
     * @return int
     */
    public int getFrom() {
        int from = current - 2;
        return from < 1 ? 1 : from;
    }

    /**
     * 获取结束页码
     *
     * @return int
     */
    public int getTo() {
        int to = current + 2;
        int total = getTotal();
        return to > total ? total : to;
    }
}
```

对应的就是分页查询的 thymeleaf 模板代码

```html
<!-- 分页 -->
<nav
	class="mt-5"
	th:if="${page.rows>0}"
>
	<ul class="pagination justify-content-center">
		<li class="page-item">
			<a
				class="page-link"
				th:href="@{${page.path}(current=1)}"
				>首页</a
			>
		</li>
		<li th:class="|page-item ${page.current==1?'disabled':''}|">
			<a
				class="page-link"
				th:href="@{${page.path}(current=${page.current-1})}"
				>上一页</a
			>
		</li>
		<li
			th:class="|page-item ${i==page.current?'active':''}|"
			th:each="i:${#numbers.sequence(page.from,page.to)}"
		>
			<a
				class="page-link"
				th:href="@{${page.path}(current=${i})}"
				th:text="${i}"
			></a>
		</li>
		<li th:class="|page-item ${page.current==page.total?'disabled':''}|">
			<a
				class="page-link"
				th:href="@{${page.path}(current=${page.current+1})}"
				>下一页</a
			>
		</li>
		<li class="page-item">
			<a
				class="page-link"
				th:href="@{${page.path}(current=${page.total})}"
				>末页</a
			>
		</li>
	</ul>
</nav>
```

<br>

## 项目调试

### slf4j 调试

首先在 `application.properties` 中注册 logger

需要确定 logger 的等级，以及 log 文件的输出位置

```properties
# Debug
logging.level.com.zhiller.community.zhillercommunity=debug
logging.file.name=E:/[1]project/[1]learn_write/java/FuckJava/ZhillerCommunity/logger.log
```

<br>

新建一个测试类，直接对该 logger 进行测试

代码清单：`LoggerTest.java`

```java
@RunWith(SpringRunner.class)
@SpringBootTest
@ContextConfiguration(classes = ZhillerCommunityApplication.class)
public class LoggerTest {

    public static final Logger logger = LoggerFactory.getLogger(LoggerTest.class);

    @Test
    public void testLogger(){
        System.out.println(logger.getName());
        logger.debug("debug");
        logger.warn("warning");
        logger.info("info");
        logger.error("error");
    }
}
```

<br>
