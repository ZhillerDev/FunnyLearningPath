### 数值问题

#### .env 环境变量配置

项目根目录下（不是 src 文件夹下）创建环境变量文件 `.env` 以及正式环境文件 `.env.pro`

对于使用 vite 框架，必须要在所有环境变量前加上 `VITE` 前缀，否则 vite 无法识别！！！  
譬如这是一个正确的变量：`VITE_DEMO = "demo"`

之后就可以在 js 文件直接使用该代码调用环境变量：  
`import.meta.env.VITE_DEMO`

<br>
