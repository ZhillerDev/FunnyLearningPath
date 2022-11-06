### LambdaQueryWrapper

`LambdaQueryWrapper` 本质上是使用一系列的方法来添加 SQL 查询语句中的限制条件；  
这里以下方的分页代码作为示例；

```java
@GetMapping("/page")
public R<Page> page(int page, int pageSize, String name) {
    log.info("{},{},{}", page, pageSize, name);

    // 设置查询点
    Page<Employee> pageInfo = new Page<Employee>(page, pageSize);

    // 设置查询规则
    LambdaQueryWrapper<Employee> wrapper = new LambdaQueryWrapper<>();
    wrapper.like(!StringUtils.isEmpty(name), Employee::getName, name);
    wrapper.orderByDesc(Employee::getUpdateTime);

    // 开始执行查询
    employeeService.page(pageInfo, wrapper);

    // 返回查询结果
    return R.success(pageInfo);
}
```

首先使用 Page（这是一个插件） 定义我们需要查询的范围，它接收两个参数：起始页码、查询页数

LambdaQueryWrapper 相当于为 SQL 查询语句添加限制条件  
譬如 like 表示添加模糊查询条件；  
orderByDesc 一行代码表示查询结果需要按照更新时间的降序排序；

employeeService.page 这里就可以直接调用 service 接口进行查询，并将查询结果直接存入 pageInfo 里面去

<br>

简而言之，LambdaQueryWrapper 使用按一下三步走：

1. 获取或者自定义限制条件
2. LambdaQueryWrapper 设置限制条件
3. 查询过程传入 wrapper 对查询结果进行限制！

<br>

### 管理员操作动态显示隐藏

说人话就是，前端判断当前用户是否为管理员，如果是管理员，那么在后台管理界面就可以出现“编辑”等 root 用户才可以操作的按钮

实现思路很简单：

使用 v-if 判断取出当前用户 session，并判断其类型是否为 admin 即可动态渲染

<br>

### 前端 API 处理方法

我们定义一个查询列表页的接口，很简单的使用 axios 请求

```js
// 查询列表页接口
const getOrderDetailPage = (params) => {
  return $axios({
    url: "/order/page",
    method: "get",
    params,
  });
};
```

<br>

而在主页面中我们可以直接调用此 API，并将其作为一个 `promise` 方法进行链式处理

如下我们即通过调用 API，请求后端接口，取回数据并在 `then` 内进行判断

```js
getOrderDetailPage({
  page: this.page,
  pageSize: this.pageSize,
  number: this.input || undefined,
  beginTime: this.beginTime || undefined,
  endTime: this.endTime || undefined,
})
  .then((res) => {
    if (String(res.code) === "1") {
      this.tableData = res.data.records || [];
      this.counts = res.data.total;
    }
  })
  .catch((err) => {
    this.$message.error("请求出错了：" + err);
  });
```

<br>

### Jackson

> 简介：Jackson 是一个进行 JSON 和 Java 对象（或者其他类型的对象）之间转换的包

这里不进行详细介绍，如果想要深入了解，请异步根目录下的 `spring-plugins` 模块

这是使用 Jackson 中的 ObjectMapper 构建的序列化 JSON 的工具类

```java
package com.zhiyiyi.reggie.common;

import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.module.SimpleModule;
import com.fasterxml.jackson.databind.ser.std.ToStringSerializer;
import com.fasterxml.jackson.datatype.jsr310.deser.LocalDateDeserializer;
import com.fasterxml.jackson.datatype.jsr310.deser.LocalDateTimeDeserializer;
import com.fasterxml.jackson.datatype.jsr310.deser.LocalTimeDeserializer;
import com.fasterxml.jackson.datatype.jsr310.ser.LocalDateSerializer;
import com.fasterxml.jackson.datatype.jsr310.ser.LocalDateTimeSerializer;
import com.fasterxml.jackson.datatype.jsr310.ser.LocalTimeSerializer;

import java.math.BigInteger;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;

import static com.fasterxml.jackson.databind.DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES;

/**
 * 对象映射器:基于jackson将Java对象转为json，或者将json转为Java对象
 * 将JSON解析为Java对象的过程称为 [从JSON反序列化Java对象]
 * 从Java对象生成JSON的过程称为 [序列化Java对象到JSON]
 */
public class JacksonObjectMapper extends ObjectMapper {

    public static final String DEFAULT_DATE_FORMAT = "yyyy-MM-dd";
    public static final String DEFAULT_DATE_TIME_FORMAT = "yyyy-MM-dd HH:mm:ss";
    public static final String DEFAULT_TIME_FORMAT = "HH:mm:ss";

    public JacksonObjectMapper() {
        super();
        //收到未知属性时不报异常
        this.configure(FAIL_ON_UNKNOWN_PROPERTIES, false);

        //反序列化时，属性不存在的兼容处理
        this.getDeserializationConfig().withoutFeatures(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);


        SimpleModule simpleModule = new SimpleModule()
                .addDeserializer(LocalDateTime.class, new LocalDateTimeDeserializer(DateTimeFormatter.ofPattern(DEFAULT_DATE_TIME_FORMAT)))
                .addDeserializer(LocalDate.class, new LocalDateDeserializer(DateTimeFormatter.ofPattern(DEFAULT_DATE_FORMAT)))
                .addDeserializer(LocalTime.class, new LocalTimeDeserializer(DateTimeFormatter.ofPattern(DEFAULT_TIME_FORMAT)))

                .addSerializer(BigInteger.class, ToStringSerializer.instance)
                .addSerializer(Long.class, ToStringSerializer.instance)
                .addSerializer(LocalDateTime.class, new LocalDateTimeSerializer(DateTimeFormatter.ofPattern(DEFAULT_DATE_TIME_FORMAT)))
                .addSerializer(LocalDate.class, new LocalDateSerializer(DateTimeFormatter.ofPattern(DEFAULT_DATE_FORMAT)))
                .addSerializer(LocalTime.class, new LocalTimeSerializer(DateTimeFormatter.ofPattern(DEFAULT_TIME_FORMAT)));

        //注册功能模块 例如，可以添加自定义序列化器和反序列化器
        this.registerModule(simpleModule);
    }
}

```

<br>

之后我们只需要在 WebMvcConfig 中的 `extendMessageConverters` 方法进行导入就好了

```java
@Slf4j
@Configuration
public class WebMvcConfig implements WebMvcConfigurer {
    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        log.info("开启静态资源映射！");
        registry.addResourceHandler("/backend/**").addResourceLocations("classpath:/backend/");
        registry.addResourceHandler("/front/**").addResourceLocations("classpath:/front/");
    }

    @Override
    public void extendMessageConverters(List<HttpMessageConverter<?>> converters) {
        MappingJackson2HttpMessageConverter messageConverter = new MappingJackson2HttpMessageConverter();

        // 手动设置objectmapper对象，否则就会自动使用默认的对象！
        messageConverter.setObjectMapper(new JacksonObjectMapper());
        converters.add(0, messageConverter);
    }
}
```

<br>

### 分析 URL 的 params

下面的代码展示了如何取出 url 中的所有 params

适用场景：点击编辑用户信息按钮时，会将该用户的 id 放入 URL 中传递给编辑页面，编辑页面获取该 param 并进行对应的 update 操作！

> 这是前端 js，别写魔怔了不知道这是啥语言

```js
function requestUrlParam(argname) {
  // 获取URL
  var url = location.href;

  // 采用split的方式获取所有params
  var arrStr = url.substring(url.indexOf("?") + 1).split("&");

  // 遍历字符串数组
  for (var i = 0; i < arrStr.length; i++) {
    // 假设我们要找的param为id，这一步就是检测是否存在指定名称的param
    var loc = arrStr[i].indexOf(argname + "=");
    // 如果找到，就掐掉头部，即把"id=123"替换为"123"，这样就成功拿到值了！
    if (loc != -1) {
      return arrStr[i].replace(argname + "=", "").replace("?", "");
    }
  }

  // 找不到，返回空
  return "";
}
```

<br>
