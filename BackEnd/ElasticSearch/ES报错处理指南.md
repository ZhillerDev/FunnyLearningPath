### JDK 版本过低

运行 `elasticsearch.bat` 出现报错信息
`warning: ignoring JAVA_HOME=D:\program_language\JDK20; using bundled JDK`

表示当前 JDK 版本过低

ES8+需要 JDK17 及以上版本；  
ES7.17.3 及以下，仅需 JDK1.8 即可；

解决办法：

1. 可以降低 ES 版本或者直接下载 JDK20 一劳永逸
2. 环境变量 Path，把旧的 JDK 路径切换为 ES8 内的 jdk 文件夹下的 JDK（这个是适配当前 ES 所自带的 JDK 运行环境）

<br>

### 内存不够

默认 ES 启动需要消耗的内存为 8GB，很显然对于绝大多数电脑来说太过分了点

进入 ES 根目录，打开文件：`config/jvm.options`

找到以下这两行代码，其中的数字就是内存使用量，我们把它改成 1g 即可  
（注意！默认这两行代码是被注释掉的，你要删掉注释符号才可以用！！！）

```
-Xms1g
-Xmx1g
```

<br>

### 选择正确 JDK 版本后仍然失败

由于我们之前配置过低版本 JDK，故在系统变量里面存在一个`CLASSPATH`变量，请把它删掉

对于高版本 JDK 来说，ES 因为`CLASSPATH`的阻挠而无法正确的找到文件`dt.jar`

TIPS:如果你使用的是低版本的 ES，可以忽略掉这一点

<br>
