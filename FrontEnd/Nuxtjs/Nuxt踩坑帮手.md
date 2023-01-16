### plugin not found

搜集了部分解决方案，以及 github 上的 issue，能解决的办法大体只有这一种：

nuxtjs 项目根目录下执行 `npm i --save glob@7.2.0`  
（资料解释是因为最新版本的 nuxtjs 使用的 glob 会导致路径读取错误，所以需要显式降低 glob 版本）

<br>

但是但是但是！当你发现这样做还是不行的话请检查项目完整路径是否带有一些特殊符号！

比如我的项目完整路径就包含了空格，导致无论如何都报错，但一换到全英文路径（不包括任何符号以及空格），就可以完美运行了！

<br>

### digital envelope routines::unsupported

问题原因：新版 nodejs 变动导致；

解决办法：直接把新版 nodejs 卸掉，然后安装 16 版的（目前最新版的 19，我试过直接报错。。。）

<br>
