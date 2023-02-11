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
