### JSR303 数据校验

#### 依赖下载

引入 JSR303 数据校验依赖

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-validation</artifactId>
</dependency>
```

<br>

#### 使用数据校验

首先将属性注入，添加 Validated 注解；

下方对指定属性添加 Email 注解，表示以邮箱格式对其进行校验，如果不符合格式，那么返回 message 中的内容

```java
@Component
@ConfigurationProperties(prefix = "form1")
@Validated
public class Form1 {
    @Email(message = "邮箱格式错误！")
    public String emailname;
}
```

<br>

#### 常见其他注解

```java
空检查
@Null       验证对象是否为null
@NotNull    验证对象是否不为null, 无法查检长度为0的字符串
@NotBlank   检查约束字符串是不是Null还有被Trim的长度是否大于0,只对字符串,且会去掉前后空格.
@NotEmpty   检查约束元素是否为NULL或者是EMPTY.

Booelan检查
@AssertTrue     验证 Boolean 对象是否为 true
@AssertFalse    验证 Boolean 对象是否为 false

长度检查
@Size(min=, max=) 验证对象（Array,Collection,Map,String）长度是否在给定的范围之内
@Length(min=, max=) string is between min and max included.

日期检查
@Past       验证 Date 和 Calendar 对象是否在当前时间之前
@Future     验证 Date 和 Calendar 对象是否在当前时间之后
@Pattern    验证 String 对象是否符合正则表达式的规则
```

<br>
