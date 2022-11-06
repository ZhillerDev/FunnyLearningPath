### 快速开启应用

services.msc 服务

regedit.exe 注册表管理器

devmgmt.msc 设备管理器

mmc 控制台

taskmgr 任务管理器

explorer 资源管理器

mspaint 画板

mstsc 远程桌面连接

write 写字板

winmsd 系统信息

whoami 当前用户

<br>

### netstat

netstat -a # 列出所有端口

netstat -at # 列出所有 TCP 端口

netstat -au # 列出所有 UDP 端口

netstat -ax # 列出所有 unix 端口

netstat -atnlp # 直接使用 ip 地址列出所有处理监听状态的 TCP 端口，且加上程序名

<br>

netstat -s # 显示所有端口的统计信息

netstat -st # 显示所有 TCP 的统计信息

netstat -su # 显示所有 UDP 的统计信息

netstat -r # 显示所有端口的统计信息

netstat -rn # 显示所有 TCP 的统计信息

> flags 标志说明：  
> U Up 表示此路由当前为启动状态  
> H Host，表示此网关为一主机  
> G Gateway，表示此网关为一路由器  
> R Reinstate Route，使用动态路由重新初始化的路由  
> D Dynamically,此路由是动态性地写入  
> M Modified，此路由是由路由守护程序或导向器动态修改  
> ! 表示此路由当前为关闭状态

<br>

### net

#### net user

net user 用户名 密码 /add - 增加一个用户

net user 用户名 /add 或 net user 用户名 “” /add - 增加一个密码为空的用户

net user 用户名 /del - 删除某个用户名

net user 用户名 /active:yes(no) - 设置某个用户的状态为启用(禁用)

<br>

#### net localgroup

net localgroup administrators - 查看管理员组里的用户(即权限为管理员的用户)

net localgroup administrators 用户名 /add - 把某个用户增加到管理员组里

net localgroup administrators 用户名 /del - 从管理员组里删除某个用户

<br>
