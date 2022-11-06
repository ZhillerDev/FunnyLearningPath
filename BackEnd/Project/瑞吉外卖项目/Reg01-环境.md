### @Slf4j 注解

安装 lombok 依赖后即可使用此注解

该注解作用是：快速于类内使用全部的 log 方法而无需一一导入！

譬如下方我们使用 `log.info` 输出信息

```java
@Slf4j
@SpringBootApplication
public class ReggieApplication {
    public static void main(String[] args) {
        SpringApplication.run(ReggieApplication.class);

        log.info("项目成功启动啦！");
    }
}
```

<br>

### 数据库与实体类名称冲突问题

由于根据 java 命名习惯，基本遵照小驼峰命名法，而数据表内属性名称基本以下划线分隔单词为准；

在 `application.yml` 文件中引用 mybatis-plus 的设置，快速解决这一问题

```yml
mybatis-plus:
  configuration:
    #在映射实体或者属性时，将数据库中表名和字段名中的下划线去掉，按照驼峰命名法映射
    map-underscore-to-camel-case: true
```
