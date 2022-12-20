### 第 4 章 APK

#### APK 逆向工具

JEB：最优的反编译器  
IDA：本地服务器下推荐使用的反编译器，可加速 android native 层逆向速度  
XposedHook：不修改源码的情况下影响程序运行的 androidhook 框架

<br>

### 第 5 章 逆向工程

<br>

### 第 11 章 AWD 线下赛

<br>

### 第 12 章 渗透环境

#### metasploit

主要由以下五大模块组成：

1.  Auxiliary（辅助模块）：负责执行扫描、嗅探、指纹识别
2.  Exploits（漏洞利用）：支持攻击者利用系统、应用或者服务中的安全漏洞进行攻击
3.  Payloads（攻击载荷模块）：支持攻击者在目标系统执行完成漏洞利用后实现实际攻击功能的代码
4.  Post-Exp（后渗透模块）：用于取得目标控制权后，进行系列的后渗透攻击行为，如获取敏感信息
5.  Encoders（编码器模块）：用于规避杀毒软件、防火墙等防护

<br>

#### Nmap

经典指令

1. 基础扫描命令：nmap 192.168.1.1
2. 主机发现：nmap -sP -n 192.168.1.2/24 -T5 --open
3. 资产扫描：nmap -sS -A --version-all 192.168.1.2/24 -T4 --open
4. 三大端口端口扫描：nmap -sT -p80,443,8080 192.168.1.2/24 --open

<br>

#### 端口转发与代理

> 获取目标主机控制权后，可通过本地网络访问目标主机网络的端口来实现远程管理

主动模式：服务端监听端口并等待客户端访问  
被动模式：客户端监听端口并等待服务器链接

<br>

本地端口转发：把本地端口转发至目标节点，目标通过该端口访问本地 web 资源  
远程端口转发：目标节点端口转发至本地，本地通过端口直接连接远程目标

<br>

#### 认证凭证

LSA（Windows 身份验证体系），存储所有用户和系统的敏感数据，管理员权限访问  
注册表位置：HKEY_LOCAL_MACHINE/Security/Policy/Secrets

<br>

LSASS（本地安全性授权服务），支持 WDigest 和 SSP 身份认证，并使用明文存储用户身份凭证

WDigest 注册表位置：HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\SecurityProviders\WDigest

> UseLogonCredential 的值设置为 0，则内存中不会存放明文密码

<br>

转储工具：

1. mimikatz：可攻破 LSASS 的保护机制，亦可直接获取 credential manager 中的数据，但被多数杀软列入黑名单
2. procdump：在 1 无法使用时可以考虑

<br>

#### 横向移动

**hash 传递**

1. LM Hash：基本淘汰，老版本采用
2. NTLM Hash：vista 版本后主流
3. Net NTLM Hash：用于各种网络认证，钓鱼窃取

未知明文，可用已知的 NTLM Hash 快速爆破

<br>

**票据传递**

Kerberos 协议是一个简单的身份认证协议  
票据传递原理：拿到票据，并将其导入内存，就可以仿冒该用户获得其权限

金票据：用 krbtgt 的 Hash 和 mimikatz 生成任意用户的票据  
银票据：伪造 TGS Ticket 来访问服务，但是只能访问特定服务器上的任意服务

<br>
