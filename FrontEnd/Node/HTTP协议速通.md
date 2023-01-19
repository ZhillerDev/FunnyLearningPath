### 请求体格式

> 一般我们使用 POST 请求时都会附带请求体，而常用的比如 formdata，那么它作用的环境是什么？特点又如何？下面将逐一介绍最常用的几种请求体格式，以及如何使用搭配案例介绍

<br>

#### json

JSON 是较为常用且易于分析的请求体格式；

JSON 格式的需求：

1. 整体使用一个大括号包裹
2. 以键值对的形式逐个书写属性，且 key 必须使用双引号括起来
3. 值 value 存在多种形式，比如字符串以及数值、数组类型
4. 每条属性（除去最后一条）末尾必须添加英文逗号作为分隔

```json
{
  "name": "tom",
  "age": 12,
  "habits": ["swim", "other"]
}
```

<br>

JSON 所有支持的属性类型：数值、字符串、对象、数组、布尔、null

JS 中可以使用 `JSON.parse()` 方法将字符串类型数值直接转换为 JSON 格式  
同理，`JSON.stringify(）` 可以吧 JSON 对象转换为字符串类型

<br>

#### multipart/form-data

当前对于此格式的请求体使用概率不高，特别是表单提交时，此格式提供的安全性保证较低

若要上传文件，则推荐使用此格式；

他依然是按照键值对的形式存储数据（下面展示 js 中如何操作 formdata 类型数据）：

1. 使用 `new FormData()` 创建一个新的空 form-data 表单
2. `formData.append("tag","cat")` 使用 append 插入一条新属性
3. `formData.get("tag")` 使用 get 根据键来获取对应值

<br>

#### application/x-www-form-urlencoded

此为 POST 的默认传输格式，也即 form 表单提交时默认使用的编码方式

当使用 `GET` 请求时：`x-www-form-urlencoded` 会将所有键值对转换后添加到 URL 的尾部，即我们常见到的 `?name=123&age=12` 之类的内容；

当使用 `POST` 请求时，`x-www-form-urlencoded` 会把所有键值对切分后封装到请求体里面后在发送请求，当且仅当我们请求时附带文件时才会动用 `multipart/form-data`（上文有提到）

<br>

#### XML

使用场景较少，以类似 HTML 的标签组成的文件结构

这是我们必须要关注到的重要特性：

1. 由一个根标签+N 个子标签组成，且根节点必须存在
2. 标签命名随意
3. 标签命名是大小写敏感的

<br>

这是一个非常简单的 XML 格式文件

文件首行为 XML 声明，它定义了当前 XML 文件的一些属性（比如编码类型）  
同理，你可以和 HTML 一样，为这些节点们添加自定义属性

```xml
<?xml version="1.0" encoding="UTF-8"?>
<root>
    <name>zhiller</name>
    <age tag="demo"></age>
</root>
```

<br>

#### raw

即字符串形式，最简单的一种格式

<br>
