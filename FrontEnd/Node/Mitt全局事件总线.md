### 光速上手

> 由于 vue3 取消了 vue2 中的$on,$off 这些全局事件总线，所以我们需要借助插件 mitt 来实现

先封装好：emitter.js

```js
import mitt from "mitt";

const emitter = mitt();

export default emitter;
```

广播事件

```js
import emitter from "../api/emitter.js";
emitter.emit("presentMode", currentMode.name);
```

接收事件

```js
import emitter from "../api/emitter.js";
let presentMode = ref("");
emitter.on("presentMode", (data) => {
  presentMode.value = data;
  console.log(`centerbroad收到信息：${data}`);
});
```

<br>
