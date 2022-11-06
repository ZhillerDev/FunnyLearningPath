### N 种注入方式

#### 常规注入

bean 注入（ref=addr 表示直接拿 addr 这个 bean 注入到 student 这个 bean 的属性里面去）

```xml
 <bean id="addr" class="com.kuang.pojo.Address">
     <property name="address" value="重庆"/>
 </bean>

 <bean id="student" class="com.kuang.pojo.Student">
     <property name="name" value="小明"/>
     <property name="address" ref="addr"/>
 </bean>
```

<br>

数组注入

```xml
<property name="books">
    <array>
        <value>西游记</value>
        <value>红楼梦</value>
        <value>水浒传</value>
    </array>
</property>
```

<br>

list 列表注入

```xml
<property name="hobbys">
    <list>
        <value>听歌</value>
        <value>看电影</value>
        <value>爬山</value>
    </list>
</property>
```

<br>

map 映射表注入

```xml
<property name="card">
    <map>
        <entry key="中国邮政" value="456456456465456"/>
        <entry key="建设" value="1456682255511"/>
    </map>
</property>
```

<br>

set 集合注入

```xml
<property name="games">
    <set>
        <value>LOL</value>
        <value>BOB</value>
        <value>COC</value>
    </set>
</property>
```

<br>

#### p&c 注入

p 注入：即 `属性（property）` 注入，导入头依赖后使用 `p:属性名` 的格式依次注入

属性必须具有 setter

```xml
导入约束 : xmlns:p="http://www.springframework.org/schema/p"
<!--P(属性: properties)命名空间 , 属性依然要设置set方法-->
<bean id="user" class="com.kuang.pojo.User" p:name="狂神" p:age="18"/>
```

<br>

c 注入：即 `构造器（constructor）` 注入，主要针对注入有参构造器的形参，使用格式 `c:形参名`

形参依然必须具有 setter

```xml
导入约束 : xmlns:c="http://www.springframework.org/schema/c"
<!--C(构造: Constructor)命名空间 , 属性依然要设置set方法-->
<bean id="user" class="com.kuang.pojo.User" c:name="狂神" c:age="18"/>
```

<br>
