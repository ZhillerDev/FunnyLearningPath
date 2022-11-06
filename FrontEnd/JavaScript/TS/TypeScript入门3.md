---
title: TypeScript入门3
date: 2022-05-16 19:32:10
tags:

categories:
- 前端
---


### TS可选参数
可选参数的存在可以让用户选择传参与否，直接在形参冒号前面加一个问号即可

还有带有默认值的参数

```ts
// 形参带有默认值的函数
function seletedParameter(n: number = 100) {
    console.log(n.toFixed(3))
}

// 形参时可选的函数
function seletedParameter2(n?: number){
    console.log(n.toPrecision(10))
}
```

<br>

**回调中的可选参数**
1. 法则：当为回调函数书写一个函数类型时，永远不要写一个可选参数！
2. 函数returnSeleted中第二个形参为一回调函数类型，在函数体内调用callback时传入的参数为arr[i]
3. 我们实例化callback函数时，使用的变量a实际上只是一种代表而已

```ts
// 回调函数中永远不要写可选参数
function returnSeleted(arr: any[], callback:(arg:any)=>void){
    for(let i=0;i<arr.length;i++){
        callback(arr[i])
    }
}

returnSeleted([1,2,3,4],(a)=>{
    console.log(a.toFixed(3))
})
```

<br>

### TS函数重载

**重载签名：**即对重载函数进行预先定义，分为两种状况
**重载实现：**即对重载签名的实现，他必须处在重载签名的后面！

**特别注意**
1. 重载签名的定义中，形参的数目必须是从上到下依次递增的！并且下面的重载签名的形参必须要包含上面所有的形参
2. 在最后实现时我们发现形参bc都加了可选符号，是因为只有第二个重载签名新增了这两个形参，他们必须是可选的，不然则无法调用第一个重载签名的函数！

```ts
// 重载签名
function reloadFunc(a: number): number
function reloadFunc(a: number, b: string, c: boolean): number

// 重载实现
function reloadFunc(a:number,b?:string,c?:boolean):number{
    return a
}
```

<br>

**形参的位置**
1. 在重载签名里面，形参重载是根据其位置做出决定的，无论重载签名有多少个，第一个形参名字怎么取，它们的含义都是一致的
2. 倘若两个重载签名都重载同一位置的形参（但是他们的类型不一致），那么在重载实现里面必须要对这个形参做出可选类型的处理  
3. 以下代码不仅对形参做出了此操作，还有返回值也做出了可选的重载

```ts
// 注意重载签名必须按照顺序，形参数量从上到下逐渐增多或者不变
function reloadFunc2(a: number): string
function reloadFunc2(a: string): number
function reloadFunc2(a: string | number): string | number {
    return 'helloworld'
}
```

<br>

### TS其他常见类型

常见类型列表列表解析：
1. void 在TS中，一个不返回任何值的函数将默认返回undefined这个值
2. object 表示任何一个不是基元类型的类型
3. unknown 代表任何值，和any类似但更安全，因为对Unknown做出的任何操作都是非法的
4. never 表示永远不会被观察到的值
5. Function 该类型的值总可以被调用，这些调用全部返回any

```ts
// return置空，默认表示返回值为void
function noop() {
    return;
}

// 不可能的值
function basicFail(msg: string): never {
    throw new Error(msg)
}

// Function
function basicFunction(f: Function) {
    return f(1, 2, 3, 4)
}
```

<br>

### TS形参展开

TS中的形参展开类似于JAVA的可变长的参数，即在形参前面加上三个英文句点，在调用时就可以传入无限个数的实参了
1. 形参展开的对象必须是一个数组
2. 形参展开必须位于函数形参中的最后一个！
3. 实参在传入的时候必须要按照顺序，即最后才传入的是形参展开

```ts
// 形参展开
// m.map(x => n * x)表示将m中的数据一一和n相乘然后生成一个map，最终返回该map
function restParameter(n: number, ...m: number[]) {
    return m.map(x => n * x)
}

const rp = restParameter(10,1,2,3,4)

console.log(rp) // 输出[10,20,30,40]
```

<br>

### TS实参展开

和形参展开类似，就是三个点+实参，传递一个数组进去

```ts
// 实参展开
const parr1 = [1,2,3]
const parr2 = [4,5,6]
parr1.push(...parr2)
```

<br>

### TS参数解构

请看规则：
1. 函数中以对象的形式写出形参，并且逐一对这些形参设置类型
2. 调用该函数时，实参要以对象的形式传入，而且必须名称一一对应！

```ts
// 参数解构的定义
function parameterSum(
    { a, b, c }: { a: number, b: number, c: number }
) {
    console.log(a + b + c)
}

// 调用参数解构必须遵照对象的格式来书写
parameterSum({
    a:10,
    b:20,
    c:30
})
```

<br>

### TS