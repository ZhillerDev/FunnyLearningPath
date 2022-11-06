---
title: TypeScript入门2
date: 2022-05-16 09:42:49
tags:

categories:
- 前端
---


### TS类型缩小
当形参可以接受多种类型时，就需要对传入数据的类型进行检测  

**typeof xxx === 'xxx'叫做类型守护**，表示判断某个变量是否属于某种类型，而且不用担心类型不符合等错误，TS已经智能的为我们处理好了这些问题

```ts
function miniType(str: number|string){
    if(typeof str === 'number'){
        console.log(new Array(str))
    }else if(typeof str === 'string'){
        console.log(str)
    }
}
```

<br>

### TS真值缩小
真值：即除了0 null undefined 0n 这些假值以外的所有值

真值缩小是为了精确定位传入值的数据类型，如下代码所示，对于传入的strs变量，我们仅仅判断它是一个object是不够的，因为null也是一个object对象，这很明显扰乱我们代码逻辑  

此时我们在判定条件前面加上 **strs &&** 即可执行真值缩小，代码逻辑变成：该变量类型为object且同时为真值（非null）才成立

```ts
function trueNarrow1(strs: string | string[] | null) {
    if(strs && typeof strs === 'object'){
        for(const s of strs){
            console.log(s)
        }
    }
}
```

<br>

### TS等值缩小

使用 === 或者 !== 则表示严格的等于或者不等于机制
1. 上部分代码因形参都具有string类型，那么使用===表示当且仅当俩变量类型都是string的才执行代码
2. 下部分代码表示当他不等于null时才执行（特别注意他只针对null这个空值进行判断，因为它是严格的）

```ts
// ===符号
function equalNarrow1(s1: string | undefined, s2: string | number) {
    if(s1===s2){
        console.log(s1+s2)
    }
}

// !==符号
function equalNarrow2(s3: number|null){
    if(s3!==null){
        // ....
    }
}
```

<br>

使用!=来表示宽松的不等于机制
所谓宽松的，即指它是否定一类而不是单个类型，譬如null和undefined都属于同一类（无意义字符），所以我们只需要令变量 !=null 就可以包含 !=undefined 等多种状况 

```ts
// !=宽松机制
function equalNarrow3(s4: string|null|undefined){
    if(s4!=null){
        // ...
    }
}
```

<br>

### TS in操作符缩小
顾名思义，以下定义了两个物种，鸟和鱼，它们分别具有各自的移动特性，那么我们在判断条件里面使用in缩小操作符将移动条件限制于仅可以游泳，那么自然就会归类到鱼身上，此时的函数调用就不会出错了

```ts
type Fish = { swin: () => void }
type Bird = { fly: () => void }

function inNarrow(animal: Fish | Bird) {
    if('swin' in animal){
        animal.swin()
    }else{
        animal.fly()
    }
}
```

<br>

### TS instanceof操作符缩小
和大多数编程语言一致，instanceof检测前者是不是后者的一个实例，如果是的话返回true

```ts
function instanceofNarrow(x: Date|string){
    if(x instanceof Date){
        console.log(x.toUTCString())
    }else{
        console.log(x.toUpperCase())
    }
}

instanceofNarrow(new Date())
instanceofNarrow("helloworld")
```

<br>

### TS分配缩小
分配缩小即变量的类型会取决于变量定义式中的可能出现的类型趋向；  
说人话就是（以下方代码中的三元运算符示例）
1. 如果random取值小于0.5，那么x被赋值12，此刻x为number类型的
2. 如果random取值大于0.5，那么x被赋值too samll，此刻x为string类型的
3. 所以根据所有趋向，x类型已经固定在了number和string之间，故不能再次把x赋予其他类型的值！

```ts
let x = Math.random() < 0.5 ? 12 : 'too small!!!'

x=true  // 出错，类型不匹配！！！
```

<br>

### TS类型谓词
开始变得复杂起来了= w =，请看下文步骤分解
1. 类型谓词定义于函数的返回值里面，格式为 parameter is type ，parameter为形参，而type是我们之前定义好的类型
2. 在return语句一行，他表示先缩小para范围，然后判断里面有没有方法swin，如果有的话return true，即返回para is ParameterTest1为真，此时函数返回类型ParameterTest1
3. 如果return判断的结果为false，那么返回para is ParameterTest1为假，此时函数返回类型ParameterTest2

```ts
type ParameterTest1 = {
    name: string
    swin: () => void
}

type ParameterTest2 = {
    name: string
    fly: () => void
}

function typeParameter(para: ParameterTest1|ParameterTest2): para is ParameterTest1{
    return (para as ParameterTest1).swin !== undefined
}
```

<br>

### TS联合类型unions
所谓的联合类型就是将两个及以上的类型结合成为一个变量，然后根据这些类型的共同参数来划分该联合类型到底属于那种子类型  
常常用到switch分支结构，如下方代码所示，根据kind的不同来定位出是与那个子类型

```ts
interface u1{
    kind:'u1'
    para1:number
}

interface u2{
    kind:'u2'
    para2: number
}

// 创建联合类型
type unionsType = u1|u2

// 根据联合类型中共同属性Kind，使用switch分支结构判断所属于那个接口
function unionsDistinct(u:unionsType){
    switch(u.kind){
        case 'u1': return u.para1**2
        case 'u2': return u.para2**4
    }
}
```

<br>

### TS函数类型表达式
下面是一个函数类型代码的解析  
如文中的 fn: (a:string) => void 他是一个函数类型，他作为形参而存在
- fn 表示该函数的名称
- a:string 表示形参以及形参的类型
- => void 表示该函数不返回任何值

```ts
// 首先定义一个函数，该函数的形参是一个函数类型
function basicFunc1(fn: (a:string) => void){
    fn('helloworld')
}

// 创建一个符合函数类型的函数
function basicFunc2(s: string){
    console.log(s)
}

// 将函数作为实参传递进去
basicFunc1(basicFunc2)
```

<br>

当然，我们依然可以使用type来定义函数类型，此刻，我们可以将上文的basicFunc1改为以下basicFunc3来书写

```ts
type BasicFunc = (a:string) => void

function basicFunc3(fn:BasicFunc){
    fn('nameless')
}
```

<br>

### TS调用签名
函数调用签名的格式如下方代码第一部分所示
- description: string 这表示签名，签名可以有多个但最好只保留一个
- (someThing:number) : boolean 表示函数接收number类型形参并且返回boolean值，特别注意这里返回值前面写的是冒号而不是箭头！

在用作形参的函数 descriptionFunc2 内，除了要定义和形参中规定一致格式的函数，还需要对签名做出定义！比如在该函数后面就紧跟着了description的定义

```ts
// 创建含调用签名的函数类型表达式
type DescriptionFunc = {
    description: string
    (someThing:number) : boolean
}

// 中继函数，这里面调用了形参函数中的签名description
function descriptionFunc1(fn:DescriptionFunc){
    console.log(fn.description+fn(100))
}

// 作为形参的函数，函数返回值以及形参一一对应之外，还需要对签名做出额外设置
function descriptionFunc2(num: number){
    console.log(num)
    return false
}
descriptionFunc2.description='helloworld'

descriptionFunc1(descriptionFunc2)
```

<br>

### TS构造签名
定义函数类型的方法和调用签名差不多，只是在函数类型的前面加了个new而已  
- 特别注意，返回值类型的String是包装器类型，而形参中的string只是一个基元类型！
- 然后在中继函数中return是需要使用new关键词来构造函数


```ts
// 构造签名
type StructFunc = {
    new (s: string): String
}

function structFunc(cstr:StructFunc){
    return new cstr('helloworld')
}

const sFunc = structFunc(String)
```

<br>

### TS泛型函数
和许多编程语言一样，TS也有泛型函数，下面代码实现了获取数组第一个元素的泛型函数
- getFirstElement<T>(arr: T[]): T | undefined 一个标准的声明泛型函数的格式，T表示泛型类型，这个字母可以任意取
- 在我们使用泛型函数时，可以不用尖括号声明类型，而是让编译器自己推断
- 如果我们使用尖括号指定了类型后，就只能使用该类型的数据了！

```ts
// 声明泛型函数
function getFirstElement<T>(arr: T[]): T | undefined {
    return arr[0]
}

getFirstElement<string>(['hello','hey'])
getFirstElement([1,2,3])
```

<br>

手动实现map数据类型：

```ts
// 自己实现map数据类型
function map<Input, Output>(arr: Input[], func: (arg: Input) => Output): Output[] {
    return arr.map(func)
}

const parsed = map(['1', '2', '3'], (n) => parseInt(n))
```

<br>

### TS泛型函数限制条件
对泛型类型添加约束条件
1. <T extends { length: number }> 使用extends关键字对泛型类型T进行限制，花括号中写的是类型T必须具有的属性，所以这里面的名字不是乱取的！！！
2. 类型T现在约束为，必须具有属性length，所以我们不能传入一个number作为实参，因为number类型没有属性length
3. string和array类型的数据才具有length属性，所以可以放心使用！

```ts
// 带有限制条件的泛型函数
function longestValue<T extends { length: number }>(a: T, b: T) {
    if (a.length > b.length) {
        // ...
    } else if (a.length < b.length) {
        // ...
    }
}

const lstr = longestValue('hellow', 'world')
const nstr = longestValue(100, 10)  // 错误！Number类型没有属性length
```

<br>

**指定类型参数**
当形参有两个及以上时，我们希望传入不同类型的实参，此时就需要显式指定类型参数，不能让编译器自动推断！！！

显示声明类型的顺序要和传入的实参类型一一配对，且尖括号内声明的类型使用竖线隔开

```ts
// 指定泛型函数类型
function setParametersType<T>(arr1: T[], arr2: T[]): T[] {
    return arr1.concat(arr2)
}

// 显示指定形参的泛型类型，注意顺序配对不可以出错
const spt = setParametersType<string | number>(['a', 'b'], [1, 2])
```

<br>
