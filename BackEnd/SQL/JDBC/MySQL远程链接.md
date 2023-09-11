### 创建数据库

> 演示使用 MySQL 版本为 5.7

<br>

自行去官网下载并安装 MySQL，默认的数据库访问地址为 `localhost:3306`

我们使用 navicat 链接数据库，显示为正常链接；

所有 MySQL 在创建的一刻都会默认新建一个数据库 mysql，里面存储系统以及用户配置信息；  
打开命令提示符，依次输入以下代码查询 user 表中的内容

```sh
# 登陆mysql
mysql -u root -p

# 输入密码并成功链接数据库后，打开数据库mysql
use mysql

# 查询user表中的两个关键属性host和user
select host,user from user;
```

<br>

默认第一行即为我们的 root 用户，此时他的 host 一列数据为 localhost，表示仅支持从 localhost 访问 root 用户；

<br>

### 修改 host

直接接着上一步输入以下代码把 host 修改为%  
`update user set host = '%' where user = 'root';`

然后刷新权限（这里很重要！不刷新就一直连接不上！）  
`flush privileges`

<br>

### 开启 3306 端口

对于 windows 系统，我们需要添加对 3306 端口的规则来让外部 shell 可以连接到远程数据库

依次点击：  
控制面板->windows defender 防火墙->高级设置->入站规则->新建规则  
选择：端口->TCP\特定本地端口 3306->允许连接->全部勾选->名称随便取->完成

此时防火墙就不会拦截 3306 端口了

<br>

### 实践演示

输入 ipconfig 查看本机的本地地址（如果需要使用外网地址，需要配置 NAT，步骤过多，这里不做赘述）

如果没有安装虚拟机，则直接找到唯一的一个 IPV4 地址，使用它；  
如果安装虚拟机，并且开启了桥接模式的话，需要使用“无线适配器 VNET8”下的 IPV4 地址；

直接拿该地址放入 navicat 后使用即可！

<br>
