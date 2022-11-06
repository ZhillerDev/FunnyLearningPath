---
title: Vue3-脚手架
date: 2022-05-28 18:41:06
tags:

categories:
- 前端
---


### vue-cli脚手架安装
三步走，第一部全局安装后，以后只需要进行后两步就可以创建脚手架了
```shell
# 使用npm全局安装vue-cli脚手架
npm install -g @vue/cli

# 创建脚手架，name为该脚手架的名字
vue create [name]

# 启动服务（需要进入刚刚创建的脚手架文件目录下面）
npm run serve
```

<br>

### 脚手架文件结构解读
| 文件名 | 作用 |
| --- | --- |
| babel.config.js | 设置ES高低版本转换规则 |
| .gitignore | git必备 |
| assets文件夹 | 存储图片音频或者文字样式等杂项 |
| App.vue | 所有子组件的父组件，通过他来管理所有下属组件 |
| main.js | 总脚本控制，在vue实例中设置挂载的元素id或者class，可以在public文件夹下的html直接设置元素id或者class来实现组件调用 |

<br>

附：App.vue代码基本解释
```js
// 首先导入vue，然后导入总组件管理器App.vue（注意后缀的书写！）
import Vue from 'vue'
import App from './App.vue'

// 设置开发模式不弹出
Vue.config.productionTip = false

// vue实例，生命周期函数mount设置vue实例作用的元素id，render函数下一节讲
new Vue({
  render: h => h(App),
}).$mount('#app')
```

<br>

### render函数
首先打开main.js文件，在第一行我们默认引入的是一个精简版的vue编译器，它不具备完整的模板template解析器  
所以我们可以修改后面的路径从而导入一个完整版的vue
```js
import Vue from 'vue' // 精简版的vue
import Vue from 'vue/dist/vue' // 完整版的vue
```

<br>

**render函数解析**
1. 专门针对精简版vue使用（即脚手架默认引入的vue版本），因为他无法使用template解析
2. render接受一个形参，该形参是一个函数，使用它我们可以创建新元素
3. render返回形参指定的函数后即可在页面中创建指定元素！

```js
// 默认render函数，它创建一个新元素，此元素就是我们的组件App.vue
new Vue({
  render: h => h(App),
}).$mount('#app')

// 使用render我们创建了元素h1
new Vue({
  render(createElement) {
    return createElement('h1','helloworld')
  },
}).$mount('#app')
```

<br>

**不同版本vue解析器分析**
- vue.js 是完整版的vue解析器，包括： 核心功能 + 模板解析器
- vue.runtime.xxx.js 是运行版vue解析器，只包括核心功能而没有模板解析器
- 两大解析器都存放在文件夹vue/dist下面

<br>

### 修改默认配置
以下命令可以查看vue脚手架的默认配置，他只负责输出内容，修改它不会对实际结构造成影响
```shell
vue inspect > output.js
```

而使用vue.congif.js文件可以直接对脚手架进行配置  
他必须和.gitignore文件同级别，具体配置项可以在vuecli官网查看

<br>

### ref属性
使用ref属性可以轻松地获取该组件template内的元素实例对象
- 首先对元素添加ref属性
- 然后通过this.$refs.name获取指定元素的实例对象（如果不明确指定，那么返回全部加了ref的对象）
- 所有被添加了ref属性的元素都会被vue捕获到，方便我们使用$refs来定向获取某个DOM节点

```html
<template>
  <div>
    <button @click="getInstance" ref="myself" >获取自己的DOM</button>
  </div>
</template>

<script>
export default {
  methods: {
    getInstance(){
      console.log(this.$refs.myself)    // 获取加了ref属性的元素的DOM节点
    }
  }
}
</script>
```

<br>

### props配置
使用该配置项，可以对调用多次的组件设置不同的data值，实现复用
1. 使用props设置参数，使得该参数可以由外界传值
2. 已经在props中设置的参数不可以在data里面重新设置
3. 可见在调用Name.vue的App.vue中，不难发现调用组件Name时传入了参数stdname，这就是props的作用！！！
4. 可以在data中使用this获取到props中设置的值，因为vue是先执行props然后才执行data的

```html
// 以下代码对应组件Name.vue
<template>
  <div>
     <h2>学生的名字是:{{stdname}}</h2> 
  </div>
</template>

<script>
export default {
    name:'Name',

    data(){
      return {
        myname:this.stdname // 可以使用this来获取props中传入的内容，因为初始化的顺序是不一致的
      }
    }

    props:['stdname']   // 设置完props后就无需再在data中设置对应的属性
}
</script>


// 以下代码对应组件App.vue
<template>
  <div>
    <Name stdname="张三"/>  // 调用时传参！
  </div>
</template>

<script>
  import Name from './components/Name'

  export default {
    name: 'App',
    components: {
      Name
    }
  }
</script>
```

<br>

**带类型的props接收**
可以使用以下两种形式，分别可以进行数据类型的限定或者是否必传、是否有默认值等

```html
<script>
export default {
    name:'Name',
    
    // 对象形式限制了接收参数的数据类型 
    props:{
      stdname:String
    }

    // type表示限制类型，required表示必须要传入该参数，default表示默认传入的值
    stdname:{
      type:String,
      required:true,
      default:'helloworld'
    }
}
</script>
```

> 特别注意，不要尝试使用任何方法动态修改props里面定义的属性，会报错的！  
还有：别用props设置ref这个属性，有冲突！

<br>

### mixin混合
混合是为了提高代码的复用率而设置的  
创建mixin.js在src文件下，下面针对该文件主要组成解析：
1. 文件内所有内容都必须使用export暴露后才可以被外界使用
2. getName表示混合的名称，后续导入混合时需要使用该名称
3. 变量内部写的是对应的方法，如methods或者data watch computed
4. 特别注意，内容必须要和我们导入的组件中属性相匹配，譬如下方的scname表示使用该混合的组件内拥有这个属性

```js
export const getName = {
    methods: {
        showName(){
            alert(this.scname)
        }
    }
}
```

<br>

在组件中使用混合：
1. import导入，花括号里面写需要使用的混合名称，上一步我们已经设置好了名字getName
2. 后续直接使用mixins:[xxx]导入混合的内容即可
3. 导入内容后即可直接使用，特别是对于多个组件如果具有相同属性或者方法时，使用混合模式将极大地提高代码易读性

```html
<script>
  import {getName} from '../mixin' 

  export default {
      name:'MixinTest',
      data() {
          return {
              scname:'未名湖大学',
              scaddress:'北京'
          }
      },
      mixins:[getName]
  }
</script>
```

> 注意：如果组件中配置的参数（如配置mounted）混合中也有（他也配置mounted），执行时会先执行mixin内定义的参数，之后才执行组件中定义的参数，不会造成冲突错误的发生（类似情况还会发生诸如写了两个data或者多个computed这些玩意）

<br>

**全局混合**
以下代码写在main.js文件内，一旦设置，则为所有的组件都挂载指定的混合选项  
（如下方代码表示为所有组件挂载混合中自定义的mixinTest选项）

```js
import {mixinTest} from './mixin'
Vue.mixin(mixinTest)
```

<br>

### 插件
插件的存在就是为了增强Vue的实用性
1. 插件使用一个对象定义，其中该对象内必须有一个install()方法
2. 在main.js里面直接导入插件后使用
3. 如下图不按常理掺入对象，而是使用了Vue实例中的原型来添加一个名称为hello的方法，一旦执行，该方法会同时在vc和vm中生成对象

```js
// 写在plugins.gs
export default {
  install(vue){
    Vue.prototype.hello = ()=>{
            alert('nihao')
        }
  }
}
```

<br>

### scoped限制CSS作用域
- 一般的，如果我们在多个组件的style里面定义了同名称的CSS（并未设置优先级），就必然会导致样式覆盖问题  
- 即，根据App.vue内组件注册的先后顺序，后注册的同名称样式会覆盖先注册的样式！！！
- 给style标签加上属性scoped后，表示该样式表仅作用于该组件内部，限定了作用域
- App.vue内的style设置的是全局CSS，他对他管理的所有组件都有效

```html
<!-- 具有局部作用域的样式表 -->
<style scoped>
  .title{
    background-color: gray;
  }
</style>
```

<br>

给style标签新增一个属性lang，然后在里面填入你将要数学样式表使用的语言格式  
下面我们选择了Less作为样式表输入语言，此刻就可以轻松的写出嵌套样式了
```html
<style lang="less">
  .title{
    color: yellow;
    .subtitle{
      font-size: 100px;
    }
  }
</style>
```

<br>

### JS浏览器本地存储

**LocalStorage**
以下案例展示了localstorage的增删改查（使用原生JS而非vue）
- window.localStorage.setItem 存储数据，该数据是一个键值对，且如果本地存储中已经有同名的键，那么则执行键值的覆盖，即LocalStorage里面不允许有重复的键出现
- 设置键值对是，key和value都是字符串，如果value传入了一个非字符串，会强制类型转换为字符串
- JSON.stringify() 可以将对象格式化，从而使得对象可以作为value而被存储。  
对象的读取则需要JSON.parse()进行解析即可！
- 剩下的删改比较简单，随便看看就会了

> LocalStorage最大的特点是，浏览器关闭后依然能存储数据（类似于cookie）

```html
<body>
    <h2 class="ht2">localStorage</h2>
    <button onclick="saveData()">点我保存数据</button>
    <button onclick="readData()">点我读取数据</button>
    <button onclick="deleteData()">点我删除数据</button>
    <button onclick="cleanData()">点我清空数据</button>

    <script>
      let p = {
          name:'李华',
          age:18
      };

      function saveData(){
          window.localStorage.setItem('msg','helloworld');    // 标准数据存储格式
          window.localStorage.setItem('msg2',123);            // 123会被强制toString()为'123'
          window.localStorage.setItem('person',JSON.stringify(p));    // 存储对象的方式
      }

      function readData(){
          console.log(localStorage.getItem('msg'));
          
          const result = localStorage.getItem('person');
          console.log(JSON.parse(result));
      }

      function deleteData(){
          localStorage.removeItem('msg');
      }

      function cleanData(){
          localStorage.clear();
      }
    </script>
</body>
```

<br>

**SessionStorage**
使用方法和localstorage一模一样，但是它存储的数据会在关闭浏览器后全部清除，而localstorage则会一直存储

<br>

### 自定义事件-绑定
形如v-on:xxx=""就称为一个事件绑定，自定义绑定可以用来实现子传父，父接收的效果（类似于props）  
首先处理App.vue，我们需要在里面设置一个按钮，用来获取子组件传入的数据
1. 使用v-on:sf绑定一个自定义组件，注意这里的SonFather表示子组件的名字！
2. getStudentName为父接收到的子传入的数据！！！
3. getStudentName里面有一个参数，它是子传递过来的实参，后面我们在设置子组件过程中会有详细介绍  
接收完该实参后，使用弹出对话框的形式来验证成功与否

```html
<!-- 这一行写在App.vue的template下 -->
<SonFather v-on:sf="getStudentName"/>

<!-- 父处理介绍到的子传入的参数 -->
<script>
  methods: {
    getStudentName(name){
      alert(name)
    }
  }
</script>
```

<br>

之后设置子组件SonFather
1. 创建一个按钮，同来向父组件发送数据，为该按钮绑定点击事件
2. this.$emit('sf',this.stdname) 参数一表示获取到的自定义绑定事件的名字，我们之前已经在父组件内部定义过，为sf  
参数二则是我们传递给父组件的参数  
3. 父组件getStudentName中的形参就是接收这里$emit传入的实参的！！！所以结果就是父组件获取到了子组件内部的stdname属性！

```html
<template>
  <div>
      <button @click="sendStudentName">发送数据给父组件</button>
  </div>
</template>

<script>
export default {
  name:'SonFather',
  data() {
    return {
        stdname:'张三'
    }
  },
  methods: {
    sendStudentName(){
        this.$emit('sf',this.stdname)
    }
  }
}
</script>
```

<br>

**解除绑定**
使用$off即可解绑事件，以下代码写在子组件内部！！！
- this.$off() 解绑所有自定义事件
- this.$off('name1') 解绑一个事件
- this.$off(['name1','name2']) 解绑多个事件

> 或者可以直接使用$destroy来销毁一个组件实例！

```html
// 写在template
<button @click="sendStudentName">发送数据给父组件</button>

unbind(){
  this.$off('sf');
  this.$off(['name1','name2']);
  this.$off();
}
```

<br>

### 动画初步
实现效果：点击button后，让一个h1标题执行进场/退场动画后显示隐藏；  
1. template中：  
button设置点击事件，使得isShow取反，代表标签的显示隐藏状态  
transition为vue特有的标记动画区域的标签，他可以name取名，以便后续指向  
v-show为控制h1标签显示与隐藏  
2. script中：设置开关isShow
3. style中：  
.move-enter-active 前面的move即为我们自定义的动画区域名称，这个选择器里面写第一次激活动画区域所做出的样式变动  
.move-leave-active 第二次激活的样式变动，此变动执行完毕后下一次就继续执行.move-enter-active，循环往复  
@keyframes moveBox 不多说，动画模块  

> 动画触发流程总结：  
首先transition专门构建了一个动画空间，当内部标签属性发生变动时，触发动画；  
第一次触发的动画即为 .v-enter-active{}样式，当我们没有指定transition的name属性时，开头就直接用v代表，否则替换成对应的name名称；  
第二次触发动画即为 .v-leave-active{}；  
这两个样式循环触发，所以很方便做出进场退场的效果！！！；

> 特别注意：被transition包裹的标签如果发生属性变化触发动画时，是先执行完动画，然后才让属性变化产生效果！

> TIPS：transition标签内只能服务一个元素，如果要实现多元素过渡，请使用transition-group标签（下下节有介绍）

<br>

以下完整代码清单 Animations.vue  
```html
<template>
  <div>
    <!-- 按钮点击事件，让isShow发生变化，使得h1隐藏掉 -->
    <button @click="isShow = !isShow">显示/隐藏</button>
    <!-- transition为动画区域，可以自定义名称 -->
    <transition name="move">
      <h1 v-show="isShow">这是一段文字</h1>
    </transition>
  </div>
</template>

<script>
export default {
  name: "Animations",
  data() {
    return {
      // isShow为布尔值，表示h1现在隐藏的状况
      isShow: true
    };
  }
};
</script>

<style>
h1 {
  background-color: orange;
}

/* 进厂激活动画 */
.move-enter-active {
  animation: moveBox 1s;
}
/* 退场激活动画（reverse表示反着播放，刚好符合退场） */
.move-leave-active {
  animation: moveBox 1s reverse;
}

/* 动画模块 */
@keyframes moveBox {
  from {
    transform: translateX(-100%);
  }
  to {
    transform: translateX(0px);
  }
}
</style>
```

<br>

### 动画过渡效果
以下代码实现的效果和上面一节完全一致，仅对style模块进行了修改  
- 首先在需要执行动画的标签样式上写入transition属性来实现平滑过渡！
- .move-enter .move-enter-to 分别表示设置动画入场的起始点和结束点（这里的move即为transition标签设置的name属性值，上一节有介绍）
- .move-leave .move-leave.to 即为出场的开始点和结束点
- 因为本动画的出入场起始结束点有镜像关系，所以可以逗号分割使用同一个选择器

```css
<style>
/* 别漏掉了在需要执行动画的标签上添加transition属性 */
h1 {
  background-color: orange;
  transition: all 0.5s ease;
}

/* 设置入场起始点和出场结束点 */
.move-enter, .move-leave-to{
    transform: translateX(-100%);
}
/* 设置入场结束点和出场起始点 */
.move-enter-to, .move-leave{
    transform: translateX(0px);
}

</style>
```

<br>

### 多元素动画过渡
transition-group标签可以服务多个元素  
为每个元素都添加不同的key值即可（key值大小不影响展示先后，只做标记使用）  
或者使用v-for进行列表渲染也可  

> 若需要对大量元素进行同时且一致的动画操作，可以吧他们都放在一个div里面，并对div挂载属性变化，然后使用transition可是可以的！！！

```js
// 使用transition-group控制多元素不同动画
<transition-group name="move" appear>
  <h1 v-show="!isShow" key="1">这是一段文字</h1>
  <h1 v-show="isShow" key="2">这是另段文字</h1>
</transition-group>

// 取巧，一个div控制多个元素同时进行同样的动画
<transition name="move">
  <div v-show="isShow">
    <h1>这是一段文字</h1>
    <h1>这是一段文字</h1>
    <h1>这是一段文字</h1>
  </div>
</transition>
```

<br>

### 第三方动画库animate.css

当前vue脚手架目录下npm安装animate.css
```
npm install animate.css --save
```

任意一个vue文件中，在script标签下导入animate.css
```js
import 'animate.css';
```

在transition中设置三个属性：
- name中填入固定内容，参照下方代码
- enter-active-class 设置入场动画，自己去animate.css官网上看，选一个入场动画的名称copy下来，粘贴进去就好了
- leave-active-class 设置出场动画，做法和设置入场动画一致

> 使用第三方动画库后，我们只需要对下方的h1写样式即可，至于前面两节所写的动画代码全部可以不写！！！

```js
<transition 
    appear 
    name="animate__animated animate__bounce"
    enter-active-class="animate__backInDown"
    leave-active-class="animate__backOutRight"
>
  <h1 v-show="isShow">这是一段文字</h1>
</transition>
```

<br>

### 配置代理方式一

当服务器之间传递数据时可能会发生跨域访问问题，即当俩服务器的HTTP协议，URL或者端口有任何一个不一致，那么一方就会拒绝掉另一方发回来的响应；  
（由浏览器的 同源安全策略 导致）
1. 此情况下，服务器A和服务器B都有发送接收文件的过程，只是最后一步传递给发送者的响应文件被同源安全策略拦截
2. CORS 跨域资源访问，就是为了解决同源安全策略的出现；
3. jsonp 使用script的src巧妙的实现了跨域资源访问，但只针对get方法，且在实战中用的非常少
4. 使用最多的是代理服务器，即在发送者和接收者之间创建一个平级的代理服务器，作为类似中间人的事务；  
（代理服务器可以用nginx，但这是后端的；最好直接用vue-cli提供的方法光速创建代理服务器）

> 多翻vue-cli的生态文档，并找到vue.config配置项的有用内容，这里配置代理服务器就是使用这个配置项

> 每修改一次vue.config.js后，必须要重启服务器才可以生效

```js
// 在vue.config.js文件中设置代理服务器
module.exports = {
    devServer: {
        proxy: 'http://localhost/5000'
    }
}
```

以上配置了代理服务器，注意我们的代理服务器端口默认和发送者端口一致，无需自行设置；  
上图中的proxy端口实际上是指接收者的端口号；  

<br>

### 配置代理方式二
为了能同时指定多个代理，故不使用第一种方式；第二种方式以逗号分隔开各个请求头，以达到设置对应代理服务器的需求（以下方代码逐一介绍配置属性含义）；  
（TIPS：以下配置参数中target不可或缺，其他的可有可无）

> **请求过程**  
发送方以设置好的请求头（/api）发送到代理服务器，代理服务器又转发到接收方（http://localhost:5000）  

> **'/api'** 设置请求头，当我们使用axios的get方法请求http://localhost:8080/home 时，实际上是请求http://localhost:8080/api/home

> **target** 设置目的主机的地址和端口号  

> **pathRewrite** 使用对象格式书写，里面包含一个键值对，表示当代理服务器请求目标主机时，需要对请求URL做出对修改；键值对中key为查找的对象（使用正则匹配），value为欲替换成的内容；  
代码中的意思是将URL中的 /api 字段全部删除（即替换为空）；  
因为我们请求目标主机URL应当是http://localhost:5000/home 而不是http://localhost:5000/api/home ，这样做才可以做到正确的请求文件  

> **ws** 是否开启websocket  

> **changeOrigin** 欺骗目标主机请求源的URL，若设置为true，则请求是以目标主机的端口（5000）为准的；否则以真正请求主机的端口（8080）为准！！！

```js
devServer: {
  proxy: {
    '/api': {
      target: 'http://localhost:5000',
      pathRewrite:{'^/api':''},
      ws: true,
      changeOrigin: true
    }
  }
}
```

<br>

### Axios请求与全局事件总线
安装axios非常简单，直接在vuecli目录下用npm装
```
npm install axios
```

<br>

**使用Axios请求案例**
以下案例展示通过请求github开放的一个接口，获取用户基础信息；  
- npm安装完axios下一步就是import导包
- axios.get() 填入你想要请求的URL，代码中使用ES6的模板字符串，this.KeyWord是为了方便传入params  
- then() 包含请求后成功或者失败的回调函数
- respone.data.items 取出respone内的数据并返回一个对象数组（因为get到的内容包含30个用户的数据）
- this.$bus.$emit 触发全局事件总线，传入get到的用户的对象数组！

```js
// 写在script下，导入axios
import axios from "axios";

// 写在methods下
searchUsers() {
  axios.get(`https://api.github.com/search/users?q=${this.KeyWord}`).then(
    respone => {
      console.log("请求成功了！",respone.data.items);
      this.$bus.$emit("getUsers", respone.data.items);
    },
    error => {
      console.log("请求失败了！", error.message);
    }
  );
}
```

<br>

**全局事件总线解析**
在main.js中写入声明周期方法beforeCreate，在Vue实例化之前即注册全局事件总线；  
Vue.prototype.$myline = this 表示在vue实例的原型对象内注册当前实例为总线，且$myline名字是自定义的；一旦注册完毕，那么各个组件就完全可以通过这个myline属性来相互进行变量、方法的访问了！！！

> 这就是全局事件总线的精髓

```js
new Vue({
  render: h => h(App),

  // 创建前即注册全局事件总线
  beforeCreate(){
    Vue.prototype.$myline = this
  }
}).$mount('#app')
```

<br>

**总线侦听**
以上面注册的myline事件为例，总线监听主要有如下两种方法：
1. $emit(event,data) 表示触发事件event，并且给该事件传入数据data
2. $on(event, function(evt){}) 表示接收事件event，并且调用函数，函数形参即为接收到的数据

```js
// 触发全局事件总线
this.$myline.$emit('getName',data.name);

// 接收全局事件总线
this.$myline.$on('getName',(name)=>{
  console.log('小王的名字是',name);
})
```

<br>

### 插槽
插槽（slot）定义在组件内部，当他被调用时可以用其他标签代替这个插槽；
下方代码清单表示，在组件Sub.vue中设置两个有名字的插槽，然后于App.vue中调用该组件并指定两个插槽用什么标签替代  
1. \<slot\>定义一个插槽，放在\<template\>内部
2. slot标签可以设置属性name，即插槽的名称，在调用的时候也必须要指明对应插槽的名称
3. 指定插槽时只需要在对应元素添加属性slot="xxx"即可（可以不按顺序）
3. 如果在调用组件时缺省某个插槽，那么就会显示我们在slot里面写的默认文本

```js
// 在Sub.vue内
<template>
  <div>
    <slot name="s1">如果没有指定插槽，那将显示此默认文本</slot>
    <slot name="s2"></slot>
  </div>
</template>

// 在App.vue内
<Sub>
  <h1 slot="s1">123</h1>
  <h2 slot="s2">456</h2>
</Sub>
```

<br>

特别的，在使用一个template标签作为插槽时，slot="xxx"可以换成另一种写法  
即 v-slot:xxx （特别注意这里是没有等号和双引号的，直接写插槽的名称即可）  

> 为什么这么做呢？想想看，当我们套用一大堆div时往往导致结构紊乱，而template在实际构建中会消失掉，此时结构就会更加明晰了！

```js
// 使用template包裹代码块，直接将其作为插槽
<Sub>
  <template v-slot:s1>
    <h1>123</h1>
    <h2>456</h2>
  </template>
</Sub>
```

<br>

**作用域插槽**
作用域插槽定义：将组件内的数据打包好放入插槽中，然后传递给调用组件插槽的标签！  
以下代码实现：将组件Sub.vue内的items数组传递给调用该组件插槽；  

> :item="items" 中item表示我们传递过去的属性名称，而items则是我们需要在本组件内打包的属性名称  
前面的冒号别丢了！！！

代码清单：Sub.vue
```html
<template>
  <div>
    <slot name="s2" :item="items"></slot>
  </div>
</template>

<script>
export default {
    name:'Sub',
    data() {
        return {
            items:[1,2,3,4]
        }
    }
}
</script>
```

<br>

使用作用于插槽注意事项：
- 该插槽必须是template标签
- 必须设置两个属性slot-scope和slot，  
slot-scope里面的名字随便取，用来作为获取到的元素的别名  
slot依然写插槽的名称（这里不可以用v-slot:xxx）
- 在插槽内部使用插值语法调用传过来的数据

代码清单App.vue
```js
<Sub>
  <template slot-scope="info" slot="s2">
    <span>{{info}}</span>
  </template>
</Sub>
```

> 执行完毕后页面上显示 { "item": [ 1, 2, 3, 4 ] }  
接收一个对象，key为组件定义的:item，而value为组件属性items  
注意到slot-scope="info"中的info仅作为代名称，实际上传来的数据名称是不会变的！！

<br>

### 