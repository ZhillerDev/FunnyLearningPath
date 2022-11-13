### 三大常用 ref 介绍

#### 标准 ref()

ref 用来定义响应式数据，注意这里使用按需导入，留意导入了什么！

1. ref != Ref，我们用 Ref 指定类型，而 ref 是构造对象的
2. ref 被传递给函数或是从一般对象上被解构时，不会丢失响应性
3. ref() 定义的值必须通过 xxx.value 的方式取出来，直接使用的话他会返回一个 ref 对象！
4. ref 一样具有响应特性
5. ref 忽略嵌套深度
6. ref 在模板中使用时会自动解包，即无需使用 `.value` 取出值

> ref() 允许我们创建可以使用任何值类型的响应式 ref

```html
<script setup lang="ts">
  import { ref } from "vue";
  let message = ref({
    tags: "helloworld",
  });
  const changeTag = () => {
    message.value.tags = "goodbyeworld";
  };
</script>

<template>
  <div>
    <!-- 自动解包 -->
    <div>{{ message }}</div>

    <!-- 点击相应 -->
    <button @click="changeTag">点击换名</button>
  </div>
</template>
```

reactive 接收一个 ref 类型的定义的数组或者 map，依然需要使用 value 取值

```js
const books = reactive([ref("Vue 3 Guide")]);
// 这里需要 .value
console.log(books[0].value);
```

<br>

#### shallowRef

适用方法和 ref 一致，但他响应特性的波及范围只是 xxx.value 这一层，更深层的就不具有响应特性！

<br>

#### triggerRef

配合 shallowRef 一起使用；  
可以使用 triggerRef(xxx)强制使括号内属性进行同步（即触发一次响应）

```js
import { triggerRef, shallowRef } from "vue";
let message = shallowRef({
  tags: "helloworld",
});
const changeTag = () => {
  message.value.tags = "goodbyeworld";
  triggerRef(message); // 强制触发响应
};
```

<br>

### customRef 自定义

自定义 ref，需要首先到 vue 里面按需引入；

- 在函数中 return 自定义 ref，传入的参数也是一个带俩形参的箭头函数
- 箭头函数里面又 return 一个 getter 方法和一个 setter 方法
- get 和 set 方法分别用 trank 和 trigger 方法分析

```html
<script setup lang="ts">
  import { customRef } from "vue";
  function ARef<T>(value: T) {
    return customRef((trank, trigger) => {
      return {
        get() {
          trank();
          return value;
        },
        set(newVal: T) {
          console.log("set");
          value = newVal;
          trigger();
        },
      };
    });
  }
</script>
```

<br>

### Reactive

#### 基本使用

Reactive 同样也是解决响应式问题，相关规定如下：

1. 只能接受一个数组或一个对象作为参数，若传入数组则会被转换为 proxy 对象
2. 调用时无需添加 value，如代码中直接 obj.name，而使用 ref 方法那应该写作 obj.value.name

```html
<script setup lang="ts">
  import { reactive } from "vue";

  const arr = reactive([1, 2, 3]); // 接受数组参数
  const obj = reactive({
    // 接受对象参数
    name: "helloworld",
  });

  const changeObj = () => {
    console.log(obj.name); // 调用免去了value
  };
</script>
```

<br>

在某方法内直接对 reactive 中对象进行赋值会破坏响应式结构，即不会执行对应的更新！  
解决办法：

1. push 方法向 reactive 数组内压入属性或者方法（注意这里 reactive 的参数是一个数组而不是对象！），其中 reactive 必须显式指定数据类型（针对 typescript）
2. type 设置自定义数据类型后直接赋值（没多少人用，这里省略不介绍了。。。）

```html
<!-- push方法解决响应式破坏问题 -->
<script setup lang="ts">
  import { reactive } from "vue";

  let obj = reactive<number[]>([]); // 显式指定数据类型

  const addObj = () => {
    let arr = [1, 2, 3, 4]; // 欲插入的数组
    obj.push(...arr); // 解构赋值
  };
</script>
```

<br>

**shallowReactive**
定义：仅对第一层具有响应式，不处理深层次对象（和 reactive 是反着来的）  
如下代码，单击按钮触发方法改变深层对象属性后，不会触发渲染更新（即失去了响应式特性）

```html
<script setup lang="ts">
  import { shallowReactive } from "vue";
  const sr = shallowReactive({
    surface: "hello", // 没有嵌套，表层对象
    deep: {
      deepname: "hey", // 两层嵌套，深层对象
    },
  });
  const changeDeepen = () => {
    sr.surface = "goodbye"; // 可以渲染更新
    sr.deep.deepname = "say"; // 不进行渲染更新
  };
</script>

<template>
  <div>
    <!-- 因为没有渲染更新，按钮名称不会发生改变 -->
    <button @click="changeDeepen">{{sr.deep.deepname}}</button>
  </div>
</template>
```

<br>

#### 代理与原值

reactive 指定一个现存的变量，即可将其变成 `响应式代理类型`

修改 `原值` 不会改变响应式代理的值

```js
const raw = {};
const proxy = reactive(raw);

// 代理对象和原始对象不是全等的
console.log(proxy === raw); // false
```

<br>

### toRef

定义：将对象转变为 ref 的引用；

- 格式 toRef(a,'b')其中 a 表示欲转换的对象，b 则为需要被赋予响应性质的属性
- toRef 后就把对象变成了 ref 对象，此时可以通过 value 获取对应值了
- toRef 接收的第一个参数可以是一个普通的对象，也可以是 reactive 对象
- （根据下方代码）因 foo 没有被纳入，所以转换后的 ref 对象内它不具有响应式特性

```html
<script setup lang="ts">
  import { toRef } from "vue";
  const obj = {
    foo: 1,
    boo: 2,
  };
  const state = toRef(obj, "boo");
  const change = () => {
    state.value++;
  };
</script>
```

<br>

### toRefs

基本功效和 toRef 一样，只不过这里使用解构的思想，让原对象内的属性转换为 ref 对象后各自使用；  
如下代码将原 obj 对象内两个属性 foo boo 直接转变为 ref 对象来使用！

```html
<script setup lang="ts">
  import { toRefs, reactive } from "vue";
  let obj = reactive({
    foo: 1,
    boo: 2,
  });
  let { foo, boo } = toRefs(obj);
  const change = () => {
    foo.value++;
    boo.value++;
  };
</script>
```

> 还有一个 toRaw()方法，它的作用是把响应式对象（如 reactive）变回原始对象

<br>

### watch 监听属性

单个 watch 接收三个属性

1. 参数一：具有响应式的对象，如 reactive 对象
2. 参数二：关于对象内部属性变化的回调函数，有新旧两个形参
3. 参数三：监听属性控制，deep 表示深度监听，immediate 表示一启动页面立即开始监听

```html
<script setup lang="ts">
  import { ref, watch, reactive } from "vue";
  let message = reactive({
    name: "helloworld",
  });
  watch(
    message,
    (newVal, oldVal) => {
      console.log(newVal);
      console.log(oldVal);
    },
    {
      deep: true,
      immediate: true,
    }
  );
</script>
```

<br>

当侦听对象内有多个属性时，参数一可以写成箭头函数的形式显示指定我们要监听的属性名称；  
如果不这样做，那就会全部监听

```html
<script>
  let message = reactive({
    name: "helloworld",
    name2: "none",
  });
  watch(
    () => message.name2,
    (newVal, oldVal) => {
      console.log(newVal);
    }
  );
</script>
```

<br>

### watchEffect 多属性监听

不同于 watch，它采用监听对象和监听方法相结合的形式；

1. watchEffect 接收一个函数，有一个参数 evt，把该参数写成箭头函数后即可在里面写入监听开始前要执行的代码
2. 剩余空间直接写当某参数变化时做出的监听
3. 把 watchEffect 设置为一个变量，而后用函数的形式调用它时就会停止监听
4. 还有附加参数，flush 表示副作用作用时期（即执行附加参数的时机），有三个：  
   pre 监听更新前执行， sync 同时进行， post 监听更新后执行  
   之后只需要在附加参数里写方法即可在对应时间段触发！

> onTrigger 方法为 vue 协助开发者测试的

```html
<script setup lang="ts">
  import { ref, watchEffect } from "vue";
  let msg1 = ref<string>("name");
  let msg2 = ref<string>("age");

  const wc = watchEffect(
    (evt) => {
      // 设置监听前要做的事
      evt(() => {
        console.log("正在执行多属性监听");
      });
      console.log("msg1 is changed:" + msg1.value);
    },
    {
      flush: "post",
      onTrigger() {
        debugger;
      },
    }
  );

  const stopWatch = () => wc(); // 停止监听
</script>
```

<br>

### Vue3 生命周期

主要有以下几个，直接通过部分引入实现

- onBeforeMount 实例创建之前
- onMounted 实例创建完成
- onBeforeUpdate 更新之前
- onUpdated 更新完毕
- onBeforeUnmount 卸载之前
- onUnmounted 卸载完毕

```html
<script setup lang="ts">
  import { onBeforeMount } from "vue";

  onBeforeMount(() => {
    console.log("创建之前");
  });
</script>
```

<br>

### less 编译器引入

在 vite 或者 vue-cli 根目录下 npm 同时安装 less 和 less-loader

```shell
npm i less less-loader
```

之后直接在任何一个 vue 模板内部设置 lang 即可使用

```html
<style lang="less" scoped>
  div {
    a {
      background-color: gray;
    }
  }
</style>
```

<br>

### defineProp 父子组件传参

vue3 中引用另一个组件非常简单，不再需要设置各个组件的 name，直接 import 导入即可！  
下方代码，父组件为 PropSuper.vue 子组件为 PropBase.vue

```html
<template>
  <prop-base-vue></prop-base-vue>
</template>

<script setup lang="ts">
  // 导入取名随意！
  import PropBaseVue from "./PropBase.vue";
</script>
```

<br>

**传参实例**
和 vue2 不同，使用 defineProps 替代原来的 props 传参；

1. 代码中上半部分为父组件，下半部分为子组件
2. :array="arr" 当传给子组件的参数为非字符串类型，就需要使用 v-bind 绑定，array 为子组件内数据名称，arr 为父组件内数据名称
3. title="base" 因为传入的是 string 类型，不需要 v-bind 绑定
4. 子组件先使用 type 定义存放接收数据的变量类型，然后作为类型传入 defineProps 后大功告成
5. 直接使用插值语法调用 type 内定义的变量即可取出数据！！！

```html
<!-- PropSuper.vue代码清单 -->
<template>
  <prop-base-vue :array="arr" title="base"></prop-base-vue>
</template>

<script setup lang="ts">
  import PropBaseVue from "./PropBase.vue";
  import { reactive } from "vue";

  const arr = reactive([1, 2, 3]);
</script>

<!-- PropBase.vue代码清单 -->
<template>
  <div>{{title}}-{{array}}</div>
</template>

<script setup lang="ts">
  import { type } from "os";
  import { reactive } from "vue";

  type props = {
    // ts版本需要强制设置类型
    title: string;
    array: number[];
  };
  defineProps<props>(); // 将类型直接传入即可
</script>
```

<br>

**传参默认值**
`withDefaults` 是一个编译器宏，他不仅可以设置 props 的默认值，还能给予类型检查！

```ts
interface Props {
  msg?: string;
  labels?: string[];
}

const props = withDefaults(defineProps<Props>(), {
  msg: "hello",
  labels: () => ["one", "two"],
});
```

<br>

### defineEmits 事件传递

此方法为子给父传递信息

1. @tap="getName" tap 为子组件内发送数据的方法，getName 为父组件内接收数据的方法
2. const getName 父组件内定义一个方法来接收子组件传入的数据，因为使用 ts，所以要显式指定形参类型
3. \<button @click="tap"\> 子组件设置一按钮点击事件连接到 tap 方法
4. defineEmits 接收一数组，数组内填发送数据的方法名称，且 defineEmits 需被赋予到一个变量上去
5. emit('tap', list) 触发数据发送事件，参一为方法名，参二及以后的参数为传入的实参！！！

```html
<!-- EmitSuper.vue -->
<template>
  <emit-base-vue @tap="getName"></emit-base-vue>
</template>

<script setup lang="ts">
  import EmitBaseVue from "./EmitBase.vue";
  const getName = (list: number[]) => {
    console.log(list);
  };
</script>

<!-- EmitBase.vue -->
<template>
  <div>
    <button @click="tap"></button>
  </div>
</template>

<script setup lang="ts">
  import { reactive } from "vue";
  const list = reactive([1, 2, 3]);

  const emit = defineEmits(["tap"]);
  const tap = () => {
    emit("tap", list);
  };
</script>
```

<br>

### defineExpose 暴露绑定

用法：子组件暴露方法或者 ref，父组件捕获并使用他们；

首先处理子组件： a.vue
定义一个 ref 属性 count，还有一个方法 getCount；  
使用 defineExpose 暴露对应内容（暴露方法时不需要尾部小括号！）

> defineExpose 无需引入，可以直接使用

```html
<template></template>

<script setup lang="ts">
  import { ref } from "vue";
  const count = ref(123);
  const getCount = () => {
    return count.value;
  };
  defineExpose({
    // 暴露
    count,
    getCount,
  });
</script>
```

<br>

然后再到父组件： b.vue

1. import 导入子组件，并且在 template 中写入子组件
2. 定义一个名叫 af 的获取子组件内容的空 ref，然后于 template 中子组件添加属性 ref，填写该 ref
3. 使用 af.value.xxx 的形式获取子组件传来的内容

> 获取子组件的 ref 属性时不需要再次使用 value，譬如下方代码的 af.value.count 就能直接获取值了

```html
<template>
  <div>
    <button @click="launch">click it!!!</button>
    <!-- 特别重要，这里必须设置子组件标签的ref属性指向我们定义的空ref才可以调用！ -->
    <aview ref="af" />
  </div>
</template>

<script setup lang="ts">
  // 导入子组件
  import aview from "./a.vue";
  import { ref } from "vue";
  const af = ref();
  const launch = () => {
    // af.value.xxx形式获取内容
    console.log(af.value.count);
    console.log(af.value.getCount());
  };
</script>
```

<br>

### 组件定义方式

全局导入只需要在 main.ts 文件内，导入该组件，然后 createApp 下使用 component 挂上去就好了；  
component 接收参数一为该全局组件的名称，参数二为导入的组件名称；  
以后在其他组件内调用该全局组件时，使用的标签名称就是\<hw\>

```ts
// 导入需要全局导入的组件位置
import HelloWorld from "../src/components/HelloWorld.vue";

createApp(App)
  .component("hw", HelloWorld) // 导入组件
  .mount("#app");
```

<br>

### 动态组件

非常重要！！！可以取代理由实现静态更新的一个方法！

1. 导入两个子组件
2. type Tabs 存储子组件别名和子组件实例名称，因为组件切换在 vue3 中需要使用组件实例名
3. type Com 里面使用 pick 从 tabs 里面挑选出 comname 属性
4. data 中存储着组件别名和实例名，markRaw 是为了不创建 proxy 造成冗余与错误！（需要导入 markRaw）
5. let current 设置目前需要展示的组件是哪一个？
6. const switchTabs 切换当前显示的组件
7. template 中使用列表渲染和 v-on 绑定了切换方法，自己去体会这地方太小写不下！！

```html
<template>
  <div class="content">
    <div class="tab">
      <div v-for="(item, index) in data" :key="index" @click="switchTabs(item)">
        {{ item.name }}
      </div>
    </div>
    <component :is="current.comName"></component>
  </div>
</template>

<script setup lang="ts">
  import { reactive, markRaw } from "vue";

  // 导入两个我们需要进行切换的子组件
  import PropBaseVue from "./PropBase.vue";
  import EmitBaseVue from "./EmitBase.vue";

  type Tabs = {
    name: string;
    comName: any;
  };

  // 使用pick从tabs里面挑选出comname属性
  type Com = Pick<Tabs, "comName">;

  const data = reactive<Tabs[]>([
    {
      name: "P子组件",
      comName: markRaw(PropBaseVue),
    },
    {
      name: "E子组件",
      comName: markRaw(EmitBaseVue),
    },
  ]);

  // 决定目前展示的组件是那个，默认展示第一个组件
  let current = reactive<Com>({
    comName: data[0].comName,
  });

  // 实现点击div标签切换不同的组件
  const switchTabs = (item: Tabs) => {
    current.comName = item.comName;
  };
</script>
```

<br>

### 插槽

和 vue2 差不多，子组件内 slot 定义插槽后，父组件 template 内调用插槽即可使用;  
被定义了名称的插槽可以使用 v-slot:name 或者简写形式#name 调用！！！

```html
<!-- 子组件模板 -->
<template>
  <slot name="s1"></slot>
</template>

<!-- 父组件调用插槽 -->
<template>
  <template #s1>
    <div></div>
  </template>
</template>
```

<br>

### teleport 传送

组件 template 标签内定义 teleport 标签，之后该标签将被插入到 vite 根目录下的 index.html 内；  
to=""中写一个选择器，譬如写 .container 则表示插入到 index.html 内具有该 class 的标签的内部！！！

```html
<template>
  <div class="main">
    <teleport to=".container">
      <div>123456</div>
    </teleport>
  </div>
</template>
```

<br>

### keep-alive 缓存

使用它可以在切换不同的组件，且组件之前定义的数据被缓存下来而不是清除，下次打开时自动填入！
(可以制作点击按钮切换 验证码登陆 或者 账号密码登陆 的效果)  
keep-alive 内的标签需要有 v-if 以保证他们不是同时出现的，且这里不可以用 v-show

```html
<template>
  <div class="container">
    <button @click="switchCom">切换另一登录方式</button>
    <keep-alive>
      <to-ref-vue v-if="flag"></to-ref-vue>
      <to-refs-vue v-else></to-refs-vue>
    </keep-alive>
  </div>
</template>

<script setup lang="ts">
  import { reactive, ref, onActivated, onDeactivated } from "vue";
  import ToRefVue from "./ToRef.vue";
  import ToRefsVue from "./ToRefs.vue";

  const flag = ref(false); // 设置初始组件显示与否
  const switchCom = () => {
    flag.value = !flag.value; // 切换组件
  };
</script>
```

<br>

### 依赖注入

人话：父组件将数据传递给子组件，且无论子组件嵌套多少层都有效！可以用 reactive 构造响应式数据

1. 父组件使用 provide 设置数据，provide 参数一位名称，参数二为值
2. 子组件使用 inject 接收父组件传来的数据，inject 接收一个参数为组件名
3. 子组件亦可以通过 flag.value 反向修改值，那么所有使用该值的任意一个组件的值都会发生改变（如果使用 ref 定义成响应式的话）

```html
<!-- 父组件使用provide传入数据 -->
<script setup lang="ts">
  import { provide } from "vue";
  import ToRefVue from "./ToRef.vue"; // 导入子组件
  provide("flag", false);
</script>

<!-- 子组件依赖注入接收数据 -->
<script setup lang="ts">
  import { inject } from "vue";
  let data = inject("flag");
</script>
```

<br>

### 消息发布与订阅模式库 Mitt

为 vite 安装 mitt

```shell
npm install mitt -S
```

mitt 可以用做 vue2 中全局事件总线的替代品

```ts
// 导入mitt后实例化！
import mitt from "mitt";
const mit = mitt();

// 声明mitt和全局事件总线bus模块，这是固定写法！
declare module "vue" {
  export interface ComponentCustomProperties {
    $Bus: typeof mit;
  }
}

const app = createApp(App);
app.mount("#app");
app.config.globalProperties.$Bus = mit; // 全局挂载mit
```

<br>

**使用 mitt 实现兄弟组件传参**
具体处理方式和原始的组件传参差不多，但是简化了步骤；

1. 兄弟 A 内：getCurrentInstance 获取当前组件实例，为后续注册 bus 做准备
2. 兄弟 A 内：ci?.proxy?.$Bus.emit 都是固定写法，emit 表示设置本组件为发出参数的一组  
   emit 接收俩参数，分别是参数名、参数值
3. 兄弟 B 内：也许新建实例然后 ci?.proxy?.$Bus.on 获取兄弟 A 传来的数据  
   on 接收俩参数，第一个为接收的参数名称，第二个即为传入参数值的回调函数

> ci?.proxy?.$Bus.on 的第一个参数还可以写 '\*' ，表示一次性接受全部传来的变量，那么同理后面的回调函数的形参就要和传入变量的值的数量相互匹配，注意这是有顺序的！！！

```html
<!-- 兄弟A -->
<template>
  <div>
    <button @click="emit">派发一个事件</button>
  </div>
</template>

<script setup lang="ts">
  import { getCurrentInstance } from "vue";

  const ci = getCurrentInstance();
  const emit = () => {
    ci?.proxy?.$Bus.emit("on-click", "mitt");
    ci?.proxy?.$Bus.emit("on-click2", "mitt");
  };
</script>

<!-- 兄弟B -->
<script setup lang="ts">
  import { getCurrentInstance } from "vue";
  const ci = getCurrentInstance();

  ci?.proxy?.$Bus.on("on-click", (str) => {
    console.log("这是兄弟A传来的数据：" + str);
  });

  ci?.proxy?.$Bus.on("*", (str, str2) => {});
</script>
```

<br>

### 全局函数和变量

代替 vue2 的全局时间总线操作；

- app.config.globalProperties.$env 注册一个全局变量，当然也可以是一个全局函数
- 然后要在 declare module 里面进行变量声明，不然后续使用会出现诸多问题；  
  这里的引用格式全部固定，直接照抄，里面给你需要的变量声明一个数据类型即可

```ts
// 变量声明，否则在外部引用的话会出现红线
declare module "@vue/runtime-core" {
  export interface ComponentCustomProperties {
    $env: string;
  }
}

const app = createApp(App);
app.mount("#app");

app.config.globalProperties.$env = "helloworld";
```

然后直接就可以在任何一个组件内部使用插值语法调用全局变量或者全局函数了

```html
<template>
  <div>{{$env}}</div>
</template>
```

<br>

###
