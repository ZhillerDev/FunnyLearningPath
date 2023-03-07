### 前端模块化

> 参考文献：https://juejin.cn/post/7147365025047379981/#comment

<br>

#### 初期非模块化时代

普通的 script 标签引用极易造成 `全局变量污染以及环境依赖` 问题  
譬如引用两个 js 文件，俩文件分别定义了 name 变量，以及 name 函数，则调用时会造成混淆（因为名字相同）

可以使用 IIFE 的形式直接把函数包起来执行，这虽然能避免混淆但是仍需记住模块返回命名，此时依然会造成混乱

<br>

#### CommonJS

NodeJS 提供了对 CommonJS 的完整支持，主要使用三大变量：`exports` `module.exports` `require`

```js
// demo1.js
// 使用module.exports导出变量
const a = 100;
module.exports = a;

// demo2.js
// 使用exports.xxx导出变量
exports.b = 200;

// require导入变量
const { a } = require("./demo1.js");
const { b } = require("./demo2.js");
```

<br>

#### Es Module

借鉴导入导出规范

```js
// 导出
export const d1 = 100
export default d2
export default function(){}
export default {d3,d4}

// 导入
import * as n1 from "xx.js"     // 混合导入
import def from "xx.js"         // 默认导入
import {a,b,c} from "xx.js"     // 解构导入
var ip = import("xx.js")        // 动态导入
```

<br>
