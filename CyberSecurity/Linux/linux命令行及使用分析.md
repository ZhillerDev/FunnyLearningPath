### 文件和目录操作指令

`pwd` 显示当前文件夹路径  
`echo $PWD` 显示带逻辑的当前文件夹路径

`cd ~` 切换到家目录  
`cd ..` 切换到上一级目录  
`cd -` 返回上一次所在目录

`mkdir -p data/test` 递归创建目录（即父目录不存在时，系统会自动新增一个）  
`mkdir -v xxx` 显示目录创建的过程  
`mkdir -m 333 demo` 为新增的 demo 目录设置权限为 333  
`mkdir -pv data/{dir1,dir2},{dir3,dir4}` 使用大括号实现多层次多级别目录新增

`touch demo{01..05}` 批量新建空文件（即新建文件 demo01,demo02...demo05）  
`touch -d 19990101 demo.txt` 修改文件创建时间为 1999.01.01  
`touch -r a.txt b.txt` 将 b.txt 的时间设置为 a.txt 的时间

<br>

`ls -a` 列出包括隐藏文件在内的所有文件  
`ls -l`或`ll` 列出文件包括其详细信息  
`ls -l --time-style=long-iso` 列出文件详细信息及时间戳  
`ls -ld xxx` 查看目录本身的信息而非文件信息  
`ls -R xxx` 递归列出目录

`ls -lrt /etc/` 按时间倒序排列文件  
`ls -F xxx` 在列出的每个文件加一个后缀来表示该文件的类型

> 文件类型符号：\*普通文件， /目录，=套接字，| FIFOS，@符号链接

<br>

`cp -a a.txt b.txt` 复制 a 文件为 b 文件，并且保留时间戳  
`cp -r dir1 dir2/` 复制目录

`\cp xxx` 屏蔽系统别名，让复制文件不询问是否覆盖

<br>

`mv a.txt dir/` 把文件 a 移动到目录 dir1 中  
`mv dir1 dir2` 移动目录

<br>

`sudo rm -rf xxx` 删库跑路

<br>

`ln a.txt hardlink` 把当前目录下的 a 文件映射成一个硬链接 hardlink

此时，即便我们删除了源文件，依然可以再次通过把硬链接映射回源文件的方式取回误删的源文件：`in hardlink a.txt`

> 硬链接和源文件只要存在其一，删掉其中的哪一个都可以恢复源文件！  
> 硬链接相当于源文件的克隆体

`ln -s 源文件 目标文件` 创建软连接，类似 windows 快捷方式，删掉源文件后软连接立刻失效

<br>
