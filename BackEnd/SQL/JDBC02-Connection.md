### 建立链接的方式

> 我们可以使用多种方式与数据库建立连接

建立数据库连接前需要准备三个元素：url，user，password  
分别表示数据库地址、数据库用户名以及用户名密码

<br>

#### 反射法

可以直接使用反射加载驱动而无需再次注册驱动，故直接使用 Class.forName 引入驱动库即可

再调用 drivermanager 来获取链接

```java
@Test
public void jdbc1() {
    // 链接信息三大件
    String user = "root";
    String password = "123456";
    String url = "jdbc:mysql://localhost:3306/springboot";

    try {
        // 反射方式加载driver
        Class.forName("com.mysql.jdbc.Driver");

        // 使用DriverManager建立数据库链接
        Connection connection = DriverManager.getConnection(url, user, password);
        System.out.println(connection);
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```

<br>

#### 注册驱动法

比上一种方式多了俩行，实在是没必要

```java
@Test
public void jdbc2() {
    // 链接信息三大件
    String user = "root";
    String password = "123456";
    String url = "jdbc:mysql://localhost:3306/springboot";

    try {
        // 反射后再次注册（多此一举）
        Class clazz = Class.forName("com.mysql.jdbc.Driver");
        Driver driver = (Driver) clazz.newInstance();
        DriverManager.registerDriver(driver);

        // 使用DriverManager建立数据库链接
        Connection connection = DriverManager.getConnection(url, user, password);
        System.out.println(connection);
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```

<br>
