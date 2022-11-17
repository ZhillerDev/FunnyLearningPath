### 依赖注入

> 使用 vue 中的 provider 可以轻松的实现依赖注入特性！！！

<br>

#### 基本示例

在根组件中使用 provider 提供数据  
请按照键值对的形式插入数据

```html
<script setup>
  import { provide } from "vue";
  // key-value 的形式提供数据
  provide("message", "hello!");
</script>
```

<br>

任意一个下属子组件内，使用 inject 注入父组件传递过来的属性

> 注：provider 传递数据不受层数限制，即根组件和子组件哪怕之间继承了 100 个中间组件，子组件依然可以通过注入得到数据

```html
<script setup>
  import { inject } from "vue";
  const message = inject("message");
</script>
```

<br>

provider 中的 value 添加 readonly 属性来使子组件无法修改值  
`provide('read-only-count', readonly(count))`

<br>

#### 带方法传递

当需要对 provider 中的数据执行修改时，最好编写一个专用的值修改方法

```html
<!-- 在供给方组件内 -->
<script setup>
  import { provide, ref } from "vue";
  const location = ref("North Pole");

  function updateLocation() {
    location.value = "South Pole";
  }

  provide("location", {
    location,
    updateLocation,
  });
</script>
```

<br>

### 组合函数

> 组合函数命名规定：调用者必须对模块使用 `use+模块名` 的方式命名方法

#### 封装 fetch

在外部编写一个模块 fetch 并将其暴露出去

```js
// fetch.js
import { ref } from "vue";

export function useFetch(url) {
  const data = ref(null);
  const error = ref(null);

  fetch(url)
    .then((res) => res.json())
    .then((json) => (data.value = json))
    .catch((err) => (error.value = err));

  // 返回值
  return { data, error };
}
```

调用者直接引入该模块，使用解构的方法就能顺利进行完整的请求并获取值了

```html
<script setup>
  import { useFetch } from "./fetch.js";
  const { data, error } = useFetch("...");
</script>
```

<br>
