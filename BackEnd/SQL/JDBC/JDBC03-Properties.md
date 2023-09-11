### 输入流获取配置项

首先我们需要在工程目录下的 src 文件夹内新建一个配置文件，用来存储我们对于数据库基本配置

```
user = root
password = 123456
url = jdbc:mysql://localhost:3306/springboot
driver = com.mysql.jdbc.Driver
```

FileInputStream 读取配置项并获取配置

```java
@Test
public void jdbc3() throws Exception {
    // 输入流获取src目录下的配置文件
    InputStream is = new FileInputStream("src\\jdbc.properties");
    Properties pro = new Properties();
    pro.load(is);
    // 读入指定配置项
    String url = pro.getProperty("url");
    String user = pro.getProperty("user");
    String password = pro.getProperty("password");
    String driverClass = pro.getProperty("driver");

    // 注册与链接获取
    Class.forName(driverClass);
    Connection connection = DriverManager.getConnection(url, user, password);
    System.out.println(connection);
}
```

<br>
