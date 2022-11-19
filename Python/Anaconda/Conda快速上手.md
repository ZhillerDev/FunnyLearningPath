### 安装与环境配置

清华大学镜像站下载官方安装包：https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/

下载完文件后安装务必勾选 add anaconda path to system PATH environment  
并且请不要勾选 register anaconda as system python 3.6

安装成功！

<br>

### 必要属性

修改 conda 下载源，下面表示设置清华镜像为下载源

```
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main/
conda config --set show_channel_urls yes

```

<br>

`windows+r` 输入 `%HOMEPATH%` 找到 `.condarc` 文件  
然后直接在文件末尾添加如下内容，修改默认 envs 存储位置

```
envs_dirs:
  - D:\xxx\xxx

```

<br>

### 虚拟环境

创建虚拟环境（不加 python 版本号，则默认安装最新的）  
`conda create -n <name>`  
创建带指定 Python 版本的虚拟环境  
`conda create -n <name> python==<version>`

虚拟环境创建完毕，直接按名称进入指定虚拟环境  
`conda activate <name>`
