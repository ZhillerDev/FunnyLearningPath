### 下载与安装

请先前往官网下载你需要版本的 `tomcat：` https://tomcat.apache.org/  
最好下载压缩包，这样直接解压就可以弄了；

在安装了 JDK 的前提下，配置好 Catalina 的环境变量，之后 startup 启动 tomcat 服务器即可；

环境变量配置方法请查询百度或者其他权威网站，作者时间有限无法赘述；

<br>

### 文件结构

`bin` 命令文件存放目录  
`conf` 配置文件存放目录，（server.xml 配置端口号，默认 8080）  
`lib` 核心目录  
`logs` 日志输出  
`temp` 存放临时文件  
`webapps` 顾名思义，几乎全部的 webapp 都放这  
`work` 存放 jsp 文件翻译过后的 java 文件以及编译后的 class 文件

> `\bin\catalina.bat` 文件中的 `set MAINCLASS=org.apache.catalina.startup.Bootstrap`这一行表示 tomcat 的文件入口点

<br>
