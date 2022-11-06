### 概述

有些情况下我们需要在 unity 中借助命令行来执行某些特殊操作；

当然，不建议把这种方法用在其他奇怪的地方；

实现方式是使用 c#自带的 `System.Diagnostics` 库来实现运行 cmd 命令行

<br>

### Process 方法

#### ProcessStartInfo

ProcessStartInfo 使用它来设置进程参数，为后续启动进程做准备；

以下是一些重要的属性

| 属性名                 | 作用                                        |
| ---------------------- | ------------------------------------------- |
| Arguments              | 第二参数                                    |
| CreateNoWindow         | 是否显示程序运行的窗口，true 为不显示       |
| UseShellExecute        | 使用 shell 执行，这里必须填 false，不然报错 |
| RedirectStandardError  | 是否重定向标准错误输出                      |
| RedirectStandardInput  | 是否重定向标准输入                          |
| RedirectStandardOutput | 是否重定向标准输出                          |
| StandardErrorEncoding  | 标准错误输出时使用的编码                    |
| StandardOutputEncoding | 标准输出时使用的编码                        |

> System.Text.Encoding 设置编码，请注意系统是需要 UTF-8 编码还是 gb2312，我这边使用 gb2312 才能正常显示否则乱码！  
> 注意：以下代码都是导入 System.Diagnostics 库后编写！！！

```cs
// cmd 表示第一个参数
// args 表示第二个参数，可以置空
// dir 为命令行运行的目录，默认置空
private static Process createCmdProcess(string cmd, string args, string dir = null)
{
    // 设置进程参数
    var p = new ProcessStartInfo(cmd);

    p.Arguments = args;
    p.CreateNoWindow = true;
    p.UseShellExecute = false;
    p.RedirectStandardError = true;
    p.RedirectStandardInput = true;
    p.RedirectStandardOutput = true;
    p.StandardErrorEncoding = System.Text.Encoding.GetEncoding("gb2312");
    p.StandardOutputEncoding = System.Text.Encoding.GetEncoding("gb2312");

    // 判断工作目录是否为空，如果非空那么就设置工作目录
    if (!string.IsNullOrEmpty(dir))
    {
        p.WorkingDirectory = dir;
    }

    // 一切就绪，启动进程！
    return Process.Start(p);
}
```

<br>

#### 封装 Process

我们使用一个字符串数组获取标准输出以及标准错误输出，即可拿到我们命令行执行的结果；

```cs
// 执行cmd命令行的方法
public static string[] runCmd(string cmd, string args, string dir = null)
{
    string[] res = new string[2];
    var p = createCmdProcess(cmd, args, dir);

    // 获取标准输出与错误输出，存到string里面
    res[0] = p.StandardOutput.ReadToEnd();
    res[1] = p.StandardError.ReadToEnd();

    // 进程执行完毕记得关掉
    p.Close();
    return res;
}
```

<br>

#### 完整封装代码

将以上两块代码封装成为一个文件，便于我们后续调用！

`CmdRunner.cs` 代码清单

```cs
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using UnityEngine;

public class CmdRunner : MonoBehaviour
{
    public static string[] runCmd(string cmd, string args, string dir = null)
    {
        string[] res = new string[2];
        var p = createCmdProcess(cmd, args, dir);
        res[0] = p.StandardOutput.ReadToEnd();
        res[1] = p.StandardError.ReadToEnd();
        p.Close();
        return res;
    }

    private static Process createCmdProcess(string cmd, string args, string dir = null)
    {
        var p = new ProcessStartInfo(cmd);

        p.Arguments = args;
        p.CreateNoWindow = true;
        p.UseShellExecute = false;
        p.RedirectStandardError = true;
        p.RedirectStandardInput = true;
        p.RedirectStandardOutput = true;
        p.StandardErrorEncoding = System.Text.Encoding.GetEncoding("gb2312");
        p.StandardOutputEncoding = System.Text.Encoding.GetEncoding("gb2312");

        if (!string.IsNullOrEmpty(dir))
        {
            p.WorkingDirectory = dir;
        }

        return Process.Start(p);
    }
}
```

<br>

### 实践案例

> 本案例使用上一节编写的封装代码 `CmdRunner.cs`

<br>

#### 正则获取地址

众所周知，ipconfig 可以获取很多地址，或者子网掩码之类的玩意；

因为获取的标准输出在 `res[0]` 里面，我们直接使用 `Regex.Matches` 进行正则匹配

然后 foreach 一把梭就拿到所有地址了

> 至于怎么通过字符串还是二次正则获取 ipv4/ipv6 地址，你们就再试试看吧，作者能力不行就不献丑了~😥😥😥

```cs
public void openCMD()
{
    string[] res = new string[2];
    res = CmdRunner.runCmd("ipconfig", "");

    // 匹配地址的正则，你抄过去就行！
    string pattern =
        "((25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))";

    // 循环输出
    foreach (Match match in Regex.Matches(res[0], pattern))
    {
        Debug.Log(match.Value);
    }
}
```
