### 标准注入法

直接使用 value 注解进行注入，spring 都这么干的

新建 springboot 项目，创建实体类 Person，编写以下代码：

```java
@Component
public class Person {
    @Value("tom")
    private String name;
    @Value("12")
    private int age;

    // 别忘了getter setter 构造函数以及tostring
}
```

<br>

然后整个测试类，使用 Autowired 自动关联实体类，然后输出该类

```java
@SpringBootTest
class Pro2ApplicationTests {

    @Autowired
    private Person person;

    @Test
    public void getPerson() {
        System.out.println(person);
    }

}
```

<br>

### @PropertySource 注入

> 直接使用 properties 配置文件执行注入

#### 配置文件编写

以此路径创建文件 `resource/properties/dog.properties`

编写内容，我们需要注入小狗的名字、年龄、以及爱好

```
dog.name=tom
dog.age=123
dog.habits=eat,run,sleep
```

<br>

#### 实体类

@PropertySource 注解引入外部 properties 文件，请注意路径！  
classpath 默认代表 resource 目录

之后直接使用 value 注入配置文件内属性即可！

请注意小狗爱好 habits 这个值是列表类型，当然也可以是字符串数组类型，都可以被接收！

```java

@Component
@PropertySource(value = "classpath:properties/dog.properties")
public class Dog {

    @Value("${dog.name}")
    private String name;
    @Value("${dog.age}")
    private int age;

    @Value("${dog.habits}")
    private List<String> habits;

    // getter/setter/tostring...
}
```

<br>

注入完毕，直接上手测试类一把梭即可

<br>

### ConfigurationProperties

在使用此注解之前必须要在 pom 中导入如下依赖

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-configuration-processor</artifactId>
    <optional>true</optional>
</dependency>
```

<br>

当 application.yaml 存在时，springboot 会忽略掉 application.properties  
故直接使用 yaml 格式的主配置文件编写我们实体类的属性值

```yaml
fruit:
  name: pear
  price: 100
```

<br>

ConfigurationProperties 使用 prefix 进行注入，注入的内容即我们配置好的对象属性

```java
@Component
@ConfigurationProperties(prefix = "fruit")
public class Fruit {
    private String name;
    private int price;
}
```

<br>

注入完毕，test 一下！

<br>
