---
title: Vue3-基础2
date: 2022-05-25 18:44:44
tags:

categories:
- 前端
---


### 数据监测原理
- 事实上，我们每在data中写入一个属性、对象亦或是数组，vue都会为我们自动的创建对应的getter、setter方法，而这些方法全部存储在_data里面
- vue自动为_data执行了代理，故假设我们现在有vue实例v，那么就会得到 data.name == v._data.name
- 无论你在data中定义的对象有多少层，vue都会不断地向下递归查找直到找到不是一个对象，然后逐层向上设置对应的getter、setter，不漏下一个
- 整个渲染数据的流程：在data中新建属性->vue在_data中注册getter setter->vue将data作为代理->显示数据到页面

<br>

### 内置指令
v-text  
使用该标签，即将元素内容直接替换为v-text所指定的data中属性了！  
下面展示了插值语法和v-text的替换写法
```html
<!-- 二者的等价写法 -->
<div>{{name}}</div>
<div v-text="name"></div>
```

v-html  
将一个元素变成链接形式，链接指向data中的属性
```html
<div v-html="url"></div>

<script>
    new Vue({
        data:{
            url:"www.baidu.com"
        }
    })
</script>
```

v-cloak属性
- 在HTML加载时会有所谓JS阻塞，也即我可以延迟一段时间后才引入JS代码，那么受到该代码牵连的元素就会无法正常展示功能
- 某元素被添加v-cloak属性后，在阻塞时该属性存在，而阻塞结束后该属性自动消失！
- v-cloak配合CSS我们可以做出类似于很多APP里面在加载数据时显示的灰色条纹，加载完毕后才显示真正的内容（业内认为可以缓解用户等待焦虑）

```html
<div class="root">
	<h2 v-cloak>{{name}}</h2>
</div>

<!-- 阻塞5s后才调用vue.js文件 -->
<script src="http://localhost:8080/vsourse/5s/vue.js"></script>
```

v-once
为任意一个元素添加该属性后，只会对里面的插值语法初始化一次，后续变量怎么变都不影响
```html
<div v-once>{{name}}</div>
```

v-pre
若元素具有该属性，则跳过他的vue编译阶段（即把该元素当做默认的html元素看待）
```html
<!-- 插值语法不解析，故显示出来的就是 {{name}} -->
<div v-pre>{{name}}</div>  
```

<br>

### 自定义指令-函数式
假设我们需要自定义一个属性v-big
1. 自定义组件的注册是在directives里面的，他和data同级别
2. 注册组件时只要写组件名而不须加v-前缀，譬如下方设置函数big，那么使用时带上前缀即可：v-big
3. 函数默认有两个形参，第一个参数存储该自定义属性所在的DOM节点；  
第二个参数存储该自定义属性的属性
4. 第二个参数存储以下属性：  
expression v-big后面跟着的内容  
name 自定义指令的名字  
value 如果该自定义指令接收的内容是一个data中属性，那么value则表示该属性的值！！！这里很重要！！！
5. 函数只有在这两个状况下调用：指令与元素成功绑定时；指令所在的模板被重新解析时

```html
<div class="root">
	<h2 v-big="n"></h2>
    <button @click="n++">点我n+1</button>
</div>

<script>
    directives:{
        big(element,binding){
            // bingind.value获取到的表达式的值然后把他乘以100
            element.innerText = binding.value*100
            console.log(element,binding)
        }
    }
</script>
```

<br>

### 自定义指令-对象式
1. 函数式没办法对DOM节点的整个生命周期进行详细的把控，但是对象式可以针对性的对DOM节点不同状态使用不同代码
2. 主要有三个状态，注意这三个状态的名称是固定的不能写错（这三个状态都可以接收elemnet和binding参数）：  
bind 指令和元素成功绑定时；  
inserted 指令所在元素插入页面时
update 指令所在模板被重新解析时
3. 这三个状态都是函数，专业一点他们被叫做钩子

```html
<script>
    new Vue({
        directives:{
            fbind:{
                bind(element,binding){
                    element.value = binding.value*100
                },
                inserted(element,binding){
                    element.foucs()
                },
                update(element,binding){
                    console.log(element)
                }
            }
        }
    })
</script>
```

<br>

### 生命周期
- 一共有八个攻击四对声明周期函数，他们还被称为钩子函数  
- 最重要的是mounted钩子，他表示当页面的所有DOM初始化完毕后才启用该钩子
- 所有声明周期函数在一个vue实例里面只能有一个！他和data平级！

```html
<script>
    new Vue({
        mounted(){
            console.log('123')
        }
    })
</script>
```

<br>

### 非单文件组件
1. 使用Vue.extend新建一个非单文件组件
2. 组件内部template内使用模板内容（两个反引号）填入该组件的模板，特别注意最外层必须是一个div  
模板里面调用data参数或者方法也一样，看着就会了
3. 组件内部除了data需要写成函数形式并使用return返回参数，其他的内容都正常写
4. 在调用组件的vue实例内，使用参数components来注册局部组件，注册方式就是 别名:组件名  
注意，如果直接写组件名而不写别名也可以注册，只不过调用时使用的是原名称而已
5. 回到顶部的div里面，直接使用\<xuexiao\>\</xuexiao\>调用我们注册好的组件即可

```html
<body>

    <div class="root">
        <h2>这里即将展示新内容</h2>
        <xuexiao></xuexiao>
    </div>

    <script>
        Vue.config.productionTip = false;

        const school = Vue.extend({
            template:`
                <div>
                    <div>你的名字{{name}}</div>
                    <button @click="changeName">弹出对话框</button>    
                </div>
            `,
            data(){
                return {
                    name:'hello',
                    age:12
                }
            },
            methods:{
                changeName(){
                    alert(this.name)
                }
            }
        })

        new Vue({
            el: '.root',
            components:{
                xuexiao:school
            }
        })
    </script>
</body>
```

<br>

### 一个重要的内置关系和其他细节
1. 在组件中，this指向该组件内的data methods computed watch 
2. 在vue实例中，this指向该实例内部的data methods computed watch
3. 我们只需要在网页中写入\<xxx\>\</xxx\>后，vue就为我们创建了VueComponent
4. 每次调用Vue.extend({})，返回的都是一个全新的VueComponent
5. VueComponent对象简称vc，Vue实例对象简称vm

- 重要的内置关系：  
VueComponent.prototype.__proto__ == Vue.prototype  
组件实例对象vc就是通过该方法来访问vue实例中的属性、方法等内容的

> 目前对于文件的命名有以下两种方式：  
my-school.vue 使用短横线链接两个单词  
MySchool 大驼峰命名（推荐）  
myschool 无大写命名（不推荐）

<br>

### 单文件组件
1. 以下展示了.vue文件，它被分为三个板块，分别是模板template，代码script和样式style
2. template内部必须要写根div，即最外层必须是一个div
3. script内部的代码需要执行暴露操作，即export default{}的意义，只有在花括号内写代码才可以暴露给外界使用  
代码中的name表示对该组件命名，最好和.vue文件名一致，如果不命名，那么vue实例中的components中就可以为这个组件随意的命名了！！！
4. style样式，直接洗css样式就好了，可以直接用在template模板里面

```html
// 模板
<template>
    <div class="demo">
        <div>{{name}}</div>
    </div>
</template>

// 代码
<script>
export default {
    name:'NewComponent',
    data() {
        return {
            name:"hello"
        }
    },
    methods: {
        changeName(){
            this.name = 'name is changed'
        }
    }
}
</script>

// 样式
<style>
    .demo{
        background-color: gray;
    }
</style>
```

<br>

### 动态参数
在v-xxx系列指令后使用尖括号来启用动态参数；  
vue会先对方括号内容进行求值得到结果后才执行本行代码；  

> btnName为data域中的属性，假设其为focus，那么最终解析为v-on:focus="changeName"

```html
<div v-on:[btnName]="changeName"></div>
```

