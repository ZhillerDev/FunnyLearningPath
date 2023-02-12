### css.preprocessorOptions

设置 CSS 预处理器的配置

如下代码使用 additionalData 为 SCSS 添加了一个全局变量，你也可以直接传入一个外部文件的 URL

```js
import { defineConfig } from "vite"; // 使用 defineConfig 工具函数获取类型提示：

export default defineConfig({
  css: {
    preprocessorOptions: {
      scss: {
        additionalData: `$injectedColor: orange;`, // 全局变量
      },
    },
  },
});
```

<br>

### resolve.alias

为文件夹路径指定别名，方便直接调用

```js
// vite.config.js
import { defineConfig } from "vite";
import path from "path";

export default defineConfig({
  resolve: {
    alias: {
      "@": path.resolve(__dirname, "./src"), // 路径别名
    },
  },
});
```

<br>

或者安装 vitealiases 插件实现自动添加别名

```js
// vite.config.js
import { defineConfig } from "vite";
import { ViteAliases } from "./node_modules/vite-aliases"; // 通过名称引入会报错，可能是插件问题

export default defineConfig({
  plugins: [ViteAliases()],
});
```

<br>

### resolve.extensions

导入时想要省略的扩展名列表

```js
// vite.config.js
import { defineConfig } from "vite";
import path from "path";

export default defineConfig({
  resolve: {
    extensions: [".js", ".ts", ".json"], // 导入时想要省略的扩展名列表
  },
});
```

<br>

### optimizeDeps.force

强制依赖构建，当依赖被构建一次后就会缓存到文件夹内，下次构建依然会首先使用缓存文件；  
开启强制依赖构建，则每次构建都重新构建依赖而不使用缓存

```js
// vite.config.js
import { defineConfig } from "vite";

export default defineConfig({
  optimizeDeps: {
    force: true, // 开启强制依赖构建
  },
});
```

<br>

### base

设定生产环境的公共基础路径

```js
// vite.config.js
import { defineConfig } from "vite";

export default defineConfig({
  base: "/foo/", // 开发或生产环境服务的公共基础路径
});
```

<br>

### server.host

设定服务器监听的地址；  
默认仅监听 127.0.0.1，若设置 host 为 true，则监听任意 IP 的地址（比如手机请求也可）

```js
// vite.config.js
import { defineConfig } from "vite";

export default defineConfig({
  server: {
    host: true,
  },
});
```

<br>

### server.proxy

CORS 常用

```js
// vite.config.js
import { defineConfig } from "vite";

export default defineConfig({
  server: {
    proxy: {
      // 直接指定
      "/demo": "http://localhost:10086",
      // 单个选项
      "/api": {
        target: "http://xxx.com",
        changeOrigin: true,
        rewrite: (path) => path.replace(/^\/api/, ""),
      },
      // 正则表达式
      "^/fallback/.*": {
        target: "http://xxx.com",
        changeOrigin: true,
        rewrite: (path) => path.replace(/^\/fallback/, ""),
      },
    },
  },
});
```

<br>

### build

build 下的三个重要属性

```js
// vite.config.js
import { defineConfig } from 'vite'

export default defineConfig({
  build: {
    outDir: 'build' // 打包文件的输出目录
    assetsDir: 'static' // 静态资源的存放目录
    assetsInlineLimit: 4096 // 图片转 base64 编码的阈值
  }
})
```

<br>

### plugins

顾名思义，添加插件都在这里（比如你用 elementplus 是自动导入就要在这里注册插件）

```js
// vite.config.js
import { defineConfig } from "vite";
import vue from "@vitejs/plugin-vue";
import { viteMockServe } from "vite-plugin-mock";

export default defineConfig({
  plugins: [vue(), viteMockServe()],
});
```

<br>
