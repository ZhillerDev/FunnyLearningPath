### 基本 bean 使用

> bean 一般直接放在 resource 根目录下

新建实体类 hello

```java
public class Hello {
    private String name;

    @Override
    public String toString() {
        return "Hello{" +
                "name='" + name + '\'' +
                '}';
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
```

<br>

resource 文件夹下新增 beans.xml，用来对 hello 进行注入

1. id 表示调用该 bean 时应当使用的名称
2. class 表示实体类所在位置
3. property 设置单个变量
4. name 被设置的变量的名称
5. value 被设置的变量的值

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="hello" class="com.zhiyiyi.sp1.entity.Hello">
        <property name="name" value="tom"/>
    </bean>

</beans>
```

<br>

之后在测试类里面构建应用上下文，直接对其测试即可！

```java
@Test
public void mainSpring() {
    ApplicationContext context = new ClassPathXmlApplicationContext("beans.xml");
    Hello hello = (Hello) context.getBean("hello");
    System.out.println(hello);
}
```

<br>

### bean 其他用法

#### 实体类含有参构造时 bean 书写规范

假设目前有一个 user 实体类，他存在一个单参构造

```java
package com.zhiyiyi.sp1.entity;

public class User {
    private String name;

    public User(String name) {
        this.name = name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void show() {
        System.out.println("name=" + name);
    }

}

```

编写新的 bean，我们命名其为 beans2.xml

我们可以使用以下三种办法对有参构造实施注入

```xml
<!-- 按照形参顺序注入，0表示形参第一位 -->
<bean id="user" class="com.zhiyiyi.sp1.entity.User">
    <constructor-arg index="0" value="tom"/>
</bean>

<!-- 按照形参名称注入 -->
<bean id="user" class="com.zhiyiyi.sp1.entity.User">
    <constructor-arg name="name" value="tom"/>
</bean>

<!-- 按照形参类型注入 -->
<bean id="user" class="com.zhiyiyi.sp1.entity.User">
    <constructor-arg type="java.lang.String" value="tom"/>
</bean>
```

<br>

### Bean 作用域

> 主要有四个作用域，其中 singleton 是默认的作用域

#### singleton

该作用域为默认作用域，可以缺省不写，他表示单例模式，仅会创建一个 bean 实例  
` <bean id="ServiceImpl" class="cn.csdn.service.ServiceImpl" scope="singleton">`

<br>

#### prototype

多实例对象，prototype 表示原型类型，仅会在我们请求 bean 时创建该对象

```xml
 <bean id="account" class="com.foo.DefaultAccount" scope="prototype"/>
  或者
 <bean id="account" class="com.foo.DefaultAccount" singleton="false"/>
```

<br>

#### request

不常用，先不介绍

<br>

#### session

不常用，也不介绍
