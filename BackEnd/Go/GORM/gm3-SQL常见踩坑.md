### 主键自递增乱序问题

一般的，我们都会设置一个主键 id，它是自递增、非空的

但当我们删除该表中的某一条记录后，后续插入新的记录后，主键递增值会从删除前最后一条记录开始算起，这就导致了主键断层

<br>

很简单，我们借助 GORM 的原生 SQL 操作方法，直接输入以下这三行代码，先删除主键，再重新设置主键，此时的主键就会重新按照当前的顺序进行正确的排列了

`user` 改为你需要操纵的表名，`id` 改为你的主键名称

```go
preinit.DB.Exec("ALTER TABLE `user` DROP `id`;")
preinit.DB.Exec("ALTER TABLE `user` ADD `id` int NOT NULL FIRST;")
preinit.DB.Exec("ALTER TABLE `user` MODIFY COLUMN `id` int NOT NULL AUTO_INCREMENT,ADD PRIMARY KEY(id);")
```

<br>

### 数据类型释疑

#### map\[string\]interface{}

取出其中的值，可以使用：`xxx['key']` 通过 key 取出  
取出值为 interface{}类型的

<br>

#### interface{}类型转 string 类型

使用函数 `Sprintf` 进行转换

```go
fmt.Sprintf("%v",openid)
```
