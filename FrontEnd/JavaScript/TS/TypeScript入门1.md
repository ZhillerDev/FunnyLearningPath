---
title: TypeScript入门1
date: 2022-05-15 19:14:16
tags:

categories:
- 前端
---


### TS最简安装过程
1. 下载vscode和node.js，分别安装它们
2. 新建一个目录文件夹，在该文件夹下打开命令行，输入代码npm install -i typescript
3. 等待安装完成后即可使用ts

<br>

### TS有非异常提示功能
TS很依赖工具，我们使用npm下载typescript后，编写代码时会自动对以下几种状况做出错误提示
- 变量名或者函数名打错了，他会提示你正确的名字
- 函数后面忘记加上小括号
- if判断中出现永远为true或者永远为false的现象

```typescript
const user = {
    name:'john',
    age:12
}

user.location // 自动报错，因为location变量没有被定义
```

<br>

### TS文件转换成JS文件
1. 命令行输入 tsc name.ts 即可将名字为name的TS文件转换成JS文件
2. 输入node name.js 即可运行该JS文件
3. tsconfig.json内，通过修改rootdir和outdir设置TS和JS存放位置

<br>

### TS常见问题处理
1. 当我们编译JS文件后导致变量或者方法名重名，编译器会显示波浪线，此刻我们可以在该文件夹下输入  
tsc --init 创建初始化文件  
然后进入该文件，将strict:true这一行直接注释掉即可；
2. tsc --watch 可以实现我们TS代码实时转换成JS文件，并且会自动附带错误提示
3. tsc --noEmitOnError --watch 即在TS代码有错误的情况下不执行编译JS的动作

<br>

### TS向下兼容
TS编译成JS文件是遵照一定的规范的，我们进入tsconfig.json文件内  
我们会找到这么一项："target": "es2016" 他表示转换成的JS代码默认支持es2016标准  
如果我们改成 es5 则表示兼容es5及以上，达成向下兼容的目的

<br>

### TS严格模式
严格模式在tsconfig.json文件内定义，以下是它和其他两个配置项的关联
1. "strict": true 表示打开严格模式，进行类型检查，因为TS本就为了类型检查而生，如果不打开严格模式那么就失去了类型检查的优势，从而变回了JS  
（打开严格模式后，他的作用默认包含后面两个配置项的功能）
2. "noImplicitAny": true 不允许使用any类型，即开启类型检查
3. "strictNullChecks": true 开启空检查，即不能随便为变量定义Undefined或者null

<br>

### TS基元类型和数组
基元类型顾名思义，就是最常用的三种类型，注意Number类型不区分整数和浮点数  
数组有两种定义方式，以下代码的第二种叫做泛型定义（别跟我说你不懂什么叫泛型）
```ts
// 三年基元类型，number类型不区分浮点数和整数
let str: string = 'hello world'
let num: number = 100
let bool: boolean = false


// 两种创建数组的办法
let arr1: number[] = [1,2,3]
let arr2: Array<number> = [4,5,6]
```

<br>

### TS定义函数
1. 可以设置函数返回值类型，但没必要，TS会自动推断（而且是在严格模式下）
2. 箭头函数或者普通函数都可以成为匿名函数

```ts
// 最常见的完整的带有返回值类型的函数
function func1(str: string): void{
    console.log("hello my"+str)
}


// 事实上TS会为我们自动推断返回值类型，我们不用自己写
function getNum(){
    return 22
}


// 定义匿名函数，最后一个直接使用箭头函数
const names = ['a','b','c']
names.forEach(function(s){
    console.log(s.toUpperCase())
})
names.forEach((s)=>{
    console.log(s.toUpperCase())
})
```

<br>

### TS对象类型
所谓对象类型即形参为一个对象组成，而非分散的参数  
具体格式看下方代码上面一部分，形参定义为对象了那么传入的实参也必须要按照对象的格式

**TIPS:**形参变量名后加上问号在写冒号，表示该参数可以被省略并默认赋予undefine值  
但他永远是指定的类型的，即b?:string中的b仍然是string类型

```ts
// 基本对象类型
function getco1(user:{name:string, age:number}){

}
getco1({
    name:'hello',
    age:12
})


// 带默认初始化的形参
function getco2(obj:{a:string, b?:string}){
    if(obj.b !== undefined){
        console.log(obj.b.toUpperCase())
    }
}
getco2({
    a:'hello'
})
getco2({
    a:'name',
    b:'hey'
})
```

<br>

还有一个联合类型，即将类型之间使用竖线分割开来，表示该变量可能属于这些类型中的任意一个

```ts
// 联合类型
function getco3(id: number|string){

}
getco3(123)
getco3('helloworld')
```

<br>

### TS类型别名
格式 type name = xxx  
name表示别名，xxx表示类型或者有许多类型组成的结构（所谓的类型别名就类似于C++的typedef）

```ts
// 类型别名的使用
type point = {
    x:number,
    y:number
}
function getpoint(pt:point){
    console.log(pt.x+pt.y)
}
getpoint({
    x:100,
    y:200
})

// 其他常见的类型别名搭配类型设置
type ID = number|string
type UserName = string
```

<br>

### TS接口类型
接口类型和对象类型差不多，下面展示了接口的基本用法（注意书写格式）

```ts
// 新建接口并调用的基本方法介绍
interface inter1{
    x: number,
    y: number
}
function getInter(pt: inter1){

}
getInter({
    x:100,
    y:200
})
```

<br>

interface与type的继承实现方法
1. 接口使用关键词extends来继承父接口，在实现接口的时候务必记得所有参数都要实现！
2. 对象类型使用符号 & 来继承父对象

```ts
// 接口类型的继承实现
interface inter2_1{
    name: string
}
interface inter2_2 extends inter2_1{
    age: number
}
const it: inter2_2 = {
    name:'tom',
    age:100
}

// type继承
type t1 = {
    name: string
}
type t2 = t1 & {
    age: number
}
const tt: t2 = {
    name:'jerry',
    age:100
}
```

<br>

接口可以通过重名的方式来添加新成员，但是type不可以这么做！！！  
如下定义了两个同名接口inter3，他们的成员会叠加在一起

```ts
// 成员的叠加性质关于接口使用
interface inter3{
    i1: string
}
interface inter3{
    i2: boolean
}
const it3: inter3 = {
    i1:'123',
    i2: true
}
```

<br>

### TS类型断言
1. 类型断言有两种写法，一个是末尾as带断言；另一个是代码最前面使用尖括号带断言
2. 断言没有运行时检查，他在编译时就已经被自动剔除了
3. 断言仅支持类型的向下转型检查，即我们没法对一个已经确认时number类型的变量断言为string类型

```ts
// 类型断言的两种方式
const mCav = document.getElementById('mcav') as HTMLCanvasElement
const mCav2 = <HTMLCanvasElement>document.getElementById('mcav')

// 可以执行变量的向下转型检查
// 很明显把字符串转换成数字是错的，但这里先把字符串断言为未知类型，然后自然可以向下转型为string
const assertval = ('hello' as unknown) as number
```

<br>

### TS文字类型
- let定义的文字类型可以不断地改变值，而const就像常量一样没办法改变
- 可以将字符串作为类型给形参使用，此时就表示了我们调用该函数时传入的实参只能是这些字符串  
（如以下代码，我们形参规定两个字符串left和right，那么调用时就只能使用这两个字符串之一作为实参）

```ts
// let和const两种不同方式定义文本类型
let strt1: string = 'hello'
const strt2: string = 'world'


// 有趣的是，我们可以把字符串直接作为类型，但赋予的值必须等于该字符串的值
const strt3: 'name' = 'name'


// 根据上一法则，同理将字符串类型作为函数的形参时的效果
// 只能使用left right之中的任意一个
function getStrt(st: string, st2: 'left'| 'right'){

}
getStrt('helloworld','left')
getStrt('donkey','right')
```

<br>

文字类型转换细节处理  
如下代码所示，我们使用req存储值然后作为实参传入，我们在变量req的末尾加上了as const表示将该变量内部的成员全部变成文字类型，如果不这么做的话我们没有办法在下一行代码中使用req.method传入参数，这会导致类型不一致的错误

```ts
// 文字类型转换方法
function getUrl(url: string, method: 'GET'| 'POST'|'GUESS'){

}
const req = {
    url:'https://www.xxx.com',
    method: 'GET'
} as const
getUrl(req.url,req.method)
```

<br>

### TS中null和undefined类型
1. 当严格模式打开的情况下，没有办法为任何变量设置这两个初始值
2. 若函数形参为可选的且可选项中包含null或者Undefined，那么在函数体内使用该形参的时候可以在点号前面加一个感叹号，表示断言该变量不可能时null的时候才执行

```ts
// 带感叹号的变量简单断言
function getAssert(x: number|null){
    console.log(x!.toFixed())
}
```

<br>

### TS枚举（特有方法）
使用方式类似于JAVA中的枚举，看着套用格式就好了  
（因为这是TS自创的方法，所以转换成JS代码会非常复杂）

```ts
// 枚举初步
enum COLOR{
    RED,
    YELLOW,
    BLACK
}
console.log(COLOR.RED)
```

<br>

### TS不常用原语
ES2020新版本中新增了两个原语，TS把它们移植了，使用方法了JS一致

```ts
// 存储大整数的原语，ES2020+才可以用
const bigint1: bigint = BigInt(10)
const bigint2: bigint = 100n

// 另一个原语symbol的使用
const symbol1 = Symbol("name")
const symbol2 = Symbol("name")
```