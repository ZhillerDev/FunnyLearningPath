---
title: Vue3-基础1
date: 2022-05-21 18:56:18
tags:

categories:
- 前端
---


### | 写在前面 |
本文及下一篇文章 **Vue3基础2** ，都是基于Vue3官方文档编写而来，因作者精力有限，少部分文章内容还停留于Vue2阶段，将会在近期内持续更新为Vue3语法；  
Vue3已经成为官方标准，可以使用Vue2作为入门，后续补全Vue3的语法差距（虽然版本高了点，但是依然换汤不换药）；  
下面是一些文章中很重要的知识点：
- v-bind b-model v-on v-if 等
- Vue的生命周期
- Vite与Vue-cli脚手架搭建与开发


<br>

### Vite安装
vue开发可以使用官方脚手架vue-cli，但是他还不够简洁且运行速度慢；  
而使用vite则可以快速构建类似脚手架样式的项目；  
以下简介安装过程（具体过程看文档：[点我查看vite官方文档](https://vitejs.cn/guide/#scaffolding-your-first-vite-project)）  

```shell
# 新建一个文件夹，打开命令行，按步骤执行以下几个命令即可！

# npm初始化最新版vite，之后按照步骤选择对应你需要的vite模板即可
npm init vite@latest

# 假设你新建的项目名称为myproj，则cd进入该目录下
cd myproj

# npm安装
npm install 

# 运行vite本地服务器，大功告成
npm run dev
```

<br>

### main.js文件介绍
打开你新建的vite项目，选中main.js，下面将为代码逐行做出介绍
```js
// Vue3已经使用createApp方法创建Vue实例，所以这里导包！
import { createApp } from 'vue'

// 每一个vite项目的全局组件管理App.vue是必不可少的，我们这里导入，以便下文注册
import App from './App.vue'

// 使用createApp方法注册App.vue，然后挂在对象为#app
createApp(App).mount('#app')
```

<br>

### Vue插值语法与实例化
1. 以对象形式声明vue实例v
2. 在data(){}方法的return内写入我们需要注册的属性，以key value键值对的形式！
3. {{name}} 插值语法格式，双大括号里面写data中定义的变量名称
4. createApp(v) 注册Vue实例v
5. mount('#v') 我们看到顶部div的id为v，那么这条代码就是将vue实例的作用域控制于拥有该id的标签上

```html
    <!-- 插值语法 -->
    <div id="v">
		<div>这是我的名字:{{name}}</div>
	</div>

    <script>		
		const v = ({
			data(){
				return{
					name:'lily'
				}
			}
		});
		Vue.createApp(v).mount('#v')
    </script>
```

<br>

### v-bind模板绑定
1. 专门绑定元素属性
2. v-bind后接元素属性，即可将属性内容变成data里面的变量（此时无需双大括号）
3. v-bind:可以简写为一个冒号 :

```html
<a v-bind:href="url">123456</a>	// 完整形式
<a :href="url">123456</a>		// 简写格式
```

<br>

### 双向数据绑定
1. 单向数据绑定：使用v-bind，只有后台修改值能影响网页内容，反过来不行
2. 双向数据绑定：使用v-model，后台和网页内容完全同步，任何一方修改都会影响对方
3. 注意，v-model仅可以对表单内部（即输入类）属性使用！
4. v-model:value=""可以简写为v-model=""，因为v-model的默认附加属性就是value
5. v-model.lazy 表示触发change事件后才执行数据同步
6. v-model.number 自动将数据转换成数值形式后才同步
7. v-model.trim 自动清除用户输入的空白字符

```html
单向数据绑定：<input type="text" v-bind:value="name">
双向数据绑定：<input type="text" v-model:value="name">
```

<br>

### el和data的另一种写法->使用原型格式
已下方代码为例，若我们直接使用console.log(v)将会输出v实例的内容，在开发者模式下，我们点开该vue实例，里面加了
美元符号$的是我们可以操作的内容，而加了短下划线的是vue底层实现内容  
最重要的是属性 __proto__ 它是vue的原型  
1. $mount可以直接指定该vue实例挂载的元素，代替el
2. data的另一种写法就是函数式写法（这种写法的好处是可以在函数内部通过this取得当前vue实例）

```html
<script>
	const v = new Vue({
		<!-- data函数式写法 -->
		data:funtion(){
			console.log(this)
			return{
				name:'helloworld'
			}
		}
	})
	v.$mount(".root")	// 外部挂载写法
</script>
```

<br>

### MVVM模型
1. M模型(model):对应data内部数据
2. V视图(view):对应模板
3. VM视图模型(viewmodel):对应vue实例对象

> 设目前有vue实例名称为vm，则有以下性质  
> 1. data中的所有属性都出现在vm身上
> 2. vm身上所有的属性，以及vue实例中原型（__proto__）中的所有属性都可以直接作为模板调用

<br>

### 数据代理

**以JS代码作为示例**
1. Object.defineProperty设置单个数据代理（所谓代理就是让某个对象代替某个对象执行操作）
2. 第一个参数为欲执行代理的对象，第二个参数为被执行对象的属性
3. 第三个参数则写getter和setter方法，他表示当我调用对象obj2时应当执行的动作
4. 因为obj2为obj1的代理，且重写了obj2的getter、setter方法，故执行obj2.x时返回100

```html
<script>
	let obj1 = {x:100}
	let obj2 = {y:200}
	
	Object.defineProperty(obj2,'x',{
		get() {
			return obj1.x
		},
		set(val) {
			obj1.x=val
		}
	})
</script>
```

<br>

**以vue作为示例**
假设目前有vue实例vm，则
1. 实例对象vm实际上是作为数据域data的代理而出现的
2. vm.name === vm._data.name 从实例对象vm中取出data中的属性可以按照左边两种形式的写法
3. 使用插值语法时，也可以按照上方格式来写，因为插值语法实际上就是对实例vm的调用！！！

```html
<body>
	<div class="root">
		<div>{{_data.name}}</div>
		<div>{{name}}</div>
	</div>
</body>

<script>
	const v = new Vue({
		el:'.root',
		data:{
			name:'tomes'
		}
	})
</script>
```

<br>

### 事件处理
所谓事件处理，就是为vue实例内增加一些方法，方法全部写在methods里面
1. 所有methods内的方法都写普通函数而最好避免写箭头函数
2. 箭头函数的this指向window； 而普通函数的this指向当前所在的vue实例对象
3. v-on:click=""或者简写@click="" 用来指定元素点击后使用的方法，双引号内直接写方法名称即可
4. v-on内部的方法如果要传参，务必加上小括号，不传参的直接省略小括号

```html
<body>
	<div class="root">
		<button @click="swin($event,123)"></button>
		<button v-on:click="svm"></button>
	</div>
</body>

<script>
	methods:{
		svm:(e)=>{
			console.log(this)	// 不要用箭头函数
		},
		swin(e,num){
			console.log(this)	// 一般用普通函数
		}
	}
</script>
```

<br>

### 事件修饰符

**vue中最常用的事件修饰符**
1. prevent:阻止默认事件（常用）
2. stop:阻止事件冒泡（常用）
3. once:事件只触发一次（常用）4.capture:使用事件的捕获模式
5. self:只有event.target是当前操作的元素是才触发事件
6. passive:事件的默认行为立即执行，无需等待事件回调执行完毕

> 特殊：  
@click.prevent.self 会阻止元素本身及其子元素的点击的默认行为  
@click.self.prevent 只会阻止对元素自身的点击的默认行为  

```html
<body>
	<div class="root">
		
		<a href="#" @click.prevent="show">阻止默认事件</a>
		
		<div class="demo1" @click="show">
			<div @click.stop="show">阻止事件冒泡</div>
		</div>
		
		<button @click.once="show">事件只会被触发一次</button>
		
	</div>
</body>
```

<br>

**passive和滚轮事件解析**
1. @wheel="" 事件：每检测到一次鼠标滚轮滚动就触发一次方法；
2. @scroll="" 事件：鼠标滚轮滚动和滑动条滚动都可以触发该方法
3. 给wheel加了passive修饰后，无需等待show回调方法执行完毕，就可马上执行wheel的初始动作（即滚动内容下移）

<br>

### 键盘事件
按下某个案件即触发方法，最常用的是@keyup
1. 常见键代码：enter回车	delete退格	esc退出	space空格	tab换行
2. 在常见键代码中，除了tab必须使用@keydown触发，其他都使用@keyup触发
3. 对于控制性按键如ctrl alt shift等  
4. 对于控制性按键，@keyup表示按下修饰键后再按下其他键，待其他键松开后才触发事件  
5. 对于控制性按键，@keydown表示正常触发事件


```html
<body>
	<div class="root">
		
		<input type="text" @keyup.enter="show"/>
		
		<input type="text" @keydown.tab="show"/>
		
		<input type="text" @keyup.caps-lock="show"/>
		
		<input type="text" @keyup.ctrl="show"/>
		<input type="text" @keydown.ctrl="show"/>
		
	</div>
</body>
```

<br>

### 计算属性
基本类似于方法，但是他使用get获取到对象，以下为代码解析步骤
1. 设置计算属性fullName，并为其设置一个get方法
2. 在get方法里面的this依旧指向当前的vue实例
3. 调用计算属性和调用data属性是一样的，都可以直接使用插值语法

```html
<body>
	<div class="root">
		<div>{{fullName}}</div>
	</div>
</body>

<script>
	computed:{
		fullName:{
			get(){
				console.log(this.name)
				return this.name
			},
			set(val){
				console.log(val)
			}
			
		}
	}
</script>
```

<br>

**getter setter方法解析**
1. 当有人读取fullName时，get方法被调用，get中return的值就作为fullName的值
2. get方法在初次调用后如果fullName的值没有变动，则后续调用都无效，直到fullName的值发生变化
3. 当fullName的值被修改时调用set方法

<br>

### 计算属性的简写
不把计算属性作为对象写，直接把他当做一个方法来写
1. 这样做将只有getter属性而无法写setter属性
2. 同样的，别用箭头函数，不然导致this指向错误
3. 调用时同理，也是有参数加括号没参数只写名字


```html
<body>
	<div class="root">
		<div>{{only}}</div>
	</div>
</body>

<script>
	computed:{		
		only(){
			console.log(this)
			return this.name
		}
	}
</script>
```

<br>

### 监视属性
即对data中的某项属性变化进行实时监控，已下方代码为例
1. val 表示欲监听的属性
2. handler 表示监听句柄，我们指定的两个形参默认含义为改变后的值和原来的值
3. handler 里面写变化后需要执行的代码
4. immediate 该参数默认为false，如果设置为true表示一启动页面就开始监听

```html
<script>
data:{
	val:false
},

watch:{
	val:{
		handler(newVal,oldVal){
			console.log('内容已经修改',newVal,oldVal)
		}
	}
}
</script>
```

<br>

另一种直接使用vue实例进行监视的方式

```html
<script>
v.$watch('val',{
	immediate:false,
	handler(n,o){
		console.log(n,o)
	}
})
</script>
```

<br>

### 深层次监视
监视data中的多层结构时，使用单引号括起来就可以针对性的监听结构中的某一个属性了  

假设我们需要监听一个对象下面的所有属性，那么就需要在监视属性中写入deep:true来执行深层次监视

```html
<script>
data: {
	numbers:{
		a:1,
		b:2
	}
},

watch:{
	'numbers.a':{
		handler(){
			console.log('a正在发生改变')
		}
	},
	
	numbers:{
		deep:true,	// 执行深层次监视
		handler(){
			console.log('123')
		}
	}
}
</script>
```

<br>

**监视的简写形式**
使用这种办法将无法设置deep和immediate这两个属性，但是handler的属性以及形参的含义没有变化

```html
<script>
watch:{
	num1(newVal,oldVal){
		console.log(newVal,oldVal)
	}
}


<!-- 特别注意，下面代码是写在vue实例外面的！！！ -->
v.$watch('num2',function(newVal,oldVal){
	console.log('123')
})
</script>
```

<br>

### watch和computed区别
1. watch可以实现异步回调，到那时computed不可以
2. 所有被vue管理的函数最好使用普通函数，此时this指向的才是vue实例对象
3. 不被vue管理的函数（如定时器和ajax异步）最好写箭头函数，这样才可以保证this指向vue实例

<br>

### 绑定class和style样式
1. 绑定class就是在data域中新建一个属性，并将该属性命名为欲绑定的类的名称，然后于元素中使用v-bind绑定  
2. 注意，第一个class绑定后面跟着的:class是v-bind的形式，表示附加的class，千万不可以少了冒号！
3. 有三种绑定方式，字符串型、数组型、对象型，其中数组和对象型可以通过method方法动态更改样式的启用与否

```html
<style>
    .standard{
        height: 100px;
    }
    
    .normal{
        margin: 100px;
    }
</style>

<div class="root">
	<div class="height" :class="bind_class">我将具有两个css样式</div>
</div>

<script>
	const v = new Vue({
		data:{
			// 字符串绑定
			bind_class:'normal',

			// 数组型绑定
			bind_arr:[
				'standard',
				'normal'
			],

			// 对象型绑定
			bind_obj:{
				standard:true,
				normal:false	
			}
		}
	})
</script>
```

<br>

**直接于标签内使用数组绑定class**
如题，数组内可以使用三元运算符来判断启用那个样式（isActive为一个开关）
```html
<div :class="[isActive ? activeClass : '', errorClass]"></div>
```

<br>

**绑定style样式**
类似内联样式表，data中使用对象存储样式信息然后于style内调用该对象启用样式

```html
<div :style="styleObject"></div>

<script>
	data() {
		return {
			styleObject: {
			color: 'red',
			fontSize: '13px'
			}
		}
	}
</script>
```

<br>

### 条件渲染
1. v-show接收一个布尔值，它类似于display属性，可以显示或者隐藏元素，但是该元素仍然存在  
v-if也接收一个布尔值，但是只要取值为false，那么它对应的元素将不被创建
2. v-if v-else-if v-else是一组使用的，也就是他们必须要相邻！他们一连串判断下去
```html
<div v-show="false"></div>

<div v-if="true"></div>

<div v-if="1"></div>
<div v-else-if="1"></div>
<div v-else="1"></div>
```

v-if最好喝template结合在一起使用，这样子就可以实现符合条件则显示或者隐藏一堆元素的办法了！
```html
<template v-if="1==1">
	<div></div>
</template>
```

<br>

### 列表渲染
1. 普通列表渲染使用v-for，且v-key表示制定遍历索引，他必须要写
2. (p, index) in persons表示以index为索引，p为值的遍历，遍历的对象是persons数组
3. 注意我们这里的index是默认值，我们可以把它改成p.id好让索引以我们设置好的参数来遍历！
4. 遍历代码中的in可以改为of

```html
<ul>
    <li v-for="(p, index) in persons" :key="index">
        {{p.name}}
    </li>
</ul>

<script>
	persons:[
        {id:'001',name:'张三'},
        {id:'002',name:'李四'},
        {id:'003',name:'王五'}
    ]
</script>
```

还有一个次数渲染，of后面接着需要遍历的次数即可，前面的括号就写data域中定义的属性
```html
<ul>
	<li v-for="(a,b) of 10">{{a}}+{{b}}</li>
</ul>
```

<br>

### key有啥用

**虚拟DOM中key的作用：**
key是虚拟DoM对象的标识，当数据发生变化时，Vue会根据【新数据】生成【新的虚拟DoM】  
随后Vue进行【新虚拟DoM】与【I旧虚拟DoM】的差异比较

**用index作为key可能会引发的问题：**
1. 若对数据进行：逆序添加、逆序删除等破坏顺序操作：会产生没有必要的真实D0M更新=>界面效果没问题，但效率低。
2. 如果结构中还包含输入类的D0M:会产生错误D0M更新==>界面有问题

**开发中如何选择key?**
1. 最好使用每条数据的唯一标识作为key,比如id、手机号、身份证号、学号等唯一值
2. 如果不存在对数据的逆序添加、逆序删除等破坏顺序操作，仅用于渲染列表用于展示，使用index作为key是没有问题的。

<br>

### 列表过滤
使用计算属性来对对象数组执行过滤操作
1. 首先创建对象数组persons，以及属性keywords并将其v-model到外部的input元素上
2. 使用列表渲染，并将渲染对象指定为计算属性filpersons而不是对象数组persons
3. filpersons函数中的第一个return返回的是一个已经被过滤后的对象数组，this.persons.filter表示此次过滤是针对persons数组的！
4. filpersons函数中的第二个return返回过滤条件，这里设置为检测keywords里面的值，若该值属于persons数组中的任意对象下的name属性的一部分，那么返回真
5. filter接收一个箭头函数，该函数返回一个布尔值，他对对象函数内所有对象进行判断，如果return的是真，那么就返回符合该条件的对象！！！（有抽象，具体理解）

```html
<body>
	<div id="root">
        <h2>人员列表</h2>
        <input type="text" placeholder="请输入搜索值" v-model="keywords">
        <ul>
            <li v-for="(p, index) in filpersons" :key="index">
                {{p.name}}-{{p.age}}
            </li>
        </ul>
    </div>
</body>

<script>
	new Vue({
            el:'#root',
            data:{
                keywords:'',
                persons:[
                    {id:'001',name:'王小曼',age:22},
                    {id:'002',name:'王五',age:21},
                    {id:'003',name:'无名氏',age:42},
                    {id:'004',name:'张三',age:12},
                    {id:'005',name:'李四',age:35}
                ]
            },
            computed:{
                filpersons(){
                    return this.persons.filter((p)=>{
                        return p.name.indexOf(this.keywords) !== -1
                    })
                }
            }
        });
</script>
```

<br>


