### Npm 缺点

不知道你有没有发现，每当创建一个新项目时，都必须手动重新安装依赖（哪怕一秒前我们刚刚在另一个项目中安装了同一依赖）

这无意中导致了项目的 node_modules 占用空间极大，且依赖无法复用，频繁下载浪费大量时间

除此之外，npm 还会引入所谓 `幽灵依赖` ，即会静默安装依赖的依赖；  
举个栗子，你同时安装依赖 A 和依赖 B，二者均需要同一个附加依赖 C 才可以运行，npm 就会死板的将依赖 C 下载两次，每次都单独指向对应支持的依赖，他不会使用引用的方式让依赖 A 和依赖 B 共用同一个依赖 C  
所以最终我们可能只会使用到一个依赖 C 却安装了 N 个他的副本，你说冤不冤？

<br>

### Pnpm

> 官网：https://pnpm.io/zh/

<br>

#### 安装与使用

卸磨杀驴，直接使用 npm 全局安装 pnpm：`npm install -g pnpm`

或者使用官网推荐的，powershell 安装：`iwr https://get.pnpm.io/install.ps1 -useb | iex`

<br>

其余的主要功能和 npm 完全一致  
初始化 package.json：`pnpm init`  
安装依赖：`pnpm install xxx`  
卸载依赖：`pnpm uninstall xxx`  
跑代码：`pnpm run xxx`

<br>
