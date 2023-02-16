### Compose 布局

<br>

#### LayoutNode

在 compose 的视图树内全部的组件都可通称为 layoutnode；

1. 父 layoutnode 约束子 layoutnode 的大小
2. 不允许重复约束，即父约束子后子不能二次约束，否则报错！（即不需重复测量）

<br>
