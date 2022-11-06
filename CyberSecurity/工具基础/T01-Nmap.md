### 基本扫描技术

#### 端口

指定端口扫描 `nmap -p <port> xxx`

指定端口范围扫描 `nmap -p <port>-<port> xxx`

多个非连续端口 `nmap -p <port>,<port>,<port> xxx`

快速扫描预设表中端口 `nmap -F xxx`

扫描开放率最高的 N 个端口 `nmap --top-ports <nums>`(nums 表示次数)  
扫描指定频率以上的端口 `nmap --port-ratio <nums>`

排除某些端口 `nmap --exclude-ports <port>`

打乱端口扫描顺序 `nmap -r xxx`

扫描所有端口 `nmap -sV --allports`

<br>

#### TCP 扫描

半开放式 SYN `nmap -sS xxx`

全连接 TCP `nmap -sT xxx`

ACK 扫描 `nmap -sA xxx`

窗口扫描（返回值为 0 表示端口未开放） `nmap -sW -p<port>-<port>`

<br>

NULL 扫描 `nmap -sN xxx`

FIN 扫描 `nmap -sF xxx`

Xmas 扫描 `nmap -sX xxx`

Maimon 扫描 `nmap -sM xxx`

<br>

空闲扫描：一种极为隐蔽的扫描方式，即伪装成一台数据流量很低的空闲主机，向目标主机发送请求并取回响应值  
`nmap -sI xxx`

<br>

#### UDP 扫描

立即开始 UDP 扫描 `nmap -sU -p <port>`

<br>

#### 系统扫描

扫描目标主机使用的操作系统 `nmap -O xxx`

猜测目标主机系统 `nmap -O --osscan-guess xxx`

<br>
