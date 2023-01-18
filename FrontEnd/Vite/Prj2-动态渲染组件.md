### 案例结果

#### 大图演示

实现在指定区域内渲染所有我们需要的全部组件，并且组件会自动撑开卡片的空间，会自动换行布局

![]()

<br>

#### 文件结构

删去了 `router、store、assets` 等和当前案例无关紧要的文件夹

```│ App.vue
│  main.js
│  style.css
│
├─components
│  └─demo
│          file-upload.vue
│          multi-pages.vue
│          pic-selector.vue
├─utils
│      demoImportsDefine.js
│
└─view
        demo.vue
```
