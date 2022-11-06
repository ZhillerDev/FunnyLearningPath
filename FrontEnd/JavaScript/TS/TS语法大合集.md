### TS 最简安装过程

1. 下载 vscode 和 node.js，分别安装它们
2. 新建一个目录文件夹，在该文件夹下打开命令行，输入代码 npm install -i typescript
3. 等待安装完成后即可使用 ts

<br>

### TS 有非异常提示功能

TS 很依赖工具，我们使用 npm 下载 typescript 后，编写代码时会自动对以下几种状况做出错误提示

- 变量名或者函数名打错了，他会提示你正确的名字
- 函数后面忘记加上小括号
- if 判断中出现永远为 true 或者永远为 false 的现象

```typescript
const user = {
  name: "john",
  age: 12,
};

user.location; // 自动报错，因为location变量没有被定义
```

<br>

### TS 文件转换成 JS 文件

1. 命令行输入 tsc name.ts 即可将名字为 name 的 TS 文件转换成 JS 文件
2. 输入 node name.js 即可运行该 JS 文件
3. tsconfig.json 内，通过修改 rootdir 和 outdir 设置 TS 和 JS 存放位置

<br>

### TS 常见问题处理

1. 当我们编译 JS 文件后导致变量或者方法名重名，编译器会显示波浪线，此刻我们可以在该文件夹下输入  
   tsc --init 创建初始化文件  
   然后进入该文件，将 strict:true 这一行直接注释掉即可；
2. tsc --watch 可以实现我们 TS 代码实时转换成 JS 文件，并且会自动附带错误提示
3. tsc --noEmitOnError --watch 即在 TS 代码有错误的情况下不执行编译 JS 的动作

<br>

### TS 向下兼容

TS 编译成 JS 文件是遵照一定的规范的，我们进入 tsconfig.json 文件内  
我们会找到这么一项："target": "es2016" 他表示转换成的 JS 代码默认支持 es2016 标准  
如果我们改成 es5 则表示兼容 es5 及以上，达成向下兼容的目的

<br>

### TS 严格模式

严格模式在 tsconfig.json 文件内定义，以下是它和其他两个配置项的关联

1. "strict": true 表示打开严格模式，进行类型检查，因为 TS 本就为了类型检查而生，如果不打开严格模式那么就失去了类型检查的优势，从而变回了 JS  
   （打开严格模式后，他的作用默认包含后面两个配置项的功能）
2. "noImplicitAny": true 不允许使用 any 类型，即开启类型检查
3. "strictNullChecks": true 开启空检查，即不能随便为变量定义 Undefined 或者 null

<br>

### TS 基元类型和数组

基元类型顾名思义，就是最常用的三种类型，注意 Number 类型不区分整数和浮点数  
数组有两种定义方式，以下代码的第二种叫做泛型定义（别跟我说你不懂什么叫泛型）

```ts
// 三年基元类型，number类型不区分浮点数和整数
let str: string = "hello world";
let num: number = 100;
let bool: boolean = false;

// 两种创建数组的办法
let arr1: number[] = [1, 2, 3];
let arr2: Array<number> = [4, 5, 6];
```

<br>

### TS 定义函数

1. 可以设置函数返回值类型，但没必要，TS 会自动推断（而且是在严格模式下）
2. 箭头函数或者普通函数都可以成为匿名函数

```ts
// 最常见的完整的带有返回值类型的函数
function func1(str: string): void {
  console.log("hello my" + str);
}

// 事实上TS会为我们自动推断返回值类型，我们不用自己写
function getNum() {
  return 22;
}

// 定义匿名函数，最后一个直接使用箭头函数
const names = ["a", "b", "c"];
names.forEach(function (s) {
  console.log(s.toUpperCase());
});
names.forEach((s) => {
  console.log(s.toUpperCase());
});
```

<br>

### TS 对象类型

所谓对象类型即形参为一个对象组成，而非分散的参数  
具体格式看下方代码上面一部分，形参定义为对象了那么传入的实参也必须要按照对象的格式

**TIPS:**形参变量名后加上问号在写冒号，表示该参数可以被省略并默认赋予 undefine 值  
但他永远是指定的类型的，即 b?:string 中的 b 仍然是 string 类型

```ts
// 基本对象类型
function getco1(user: { name: string; age: number }) {}
getco1({
  name: "hello",
  age: 12,
});

// 带默认初始化的形参
function getco2(obj: { a: string; b?: string }) {
  if (obj.b !== undefined) {
    console.log(obj.b.toUpperCase());
  }
}
getco2({
  a: "hello",
});
getco2({
  a: "name",
  b: "hey",
});
```

<br>

还有一个联合类型，即将类型之间使用竖线分割开来，表示该变量可能属于这些类型中的任意一个

```ts
// 联合类型
function getco3(id: number | string) {}
getco3(123);
getco3("helloworld");
```

<br>

### TS 类型别名

格式 type name = xxx  
name 表示别名，xxx 表示类型或者有许多类型组成的结构（所谓的类型别名就类似于 C++的 typedef）

```ts
// 类型别名的使用
type point = {
  x: number;
  y: number;
};
function getpoint(pt: point) {
  console.log(pt.x + pt.y);
}
getpoint({
  x: 100,
  y: 200,
});

// 其他常见的类型别名搭配类型设置
type ID = number | string;
type UserName = string;
```

<br>

### TS 接口类型

接口类型和对象类型差不多，下面展示了接口的基本用法（注意书写格式）

```ts
// 新建接口并调用的基本方法介绍
interface inter1 {
  x: number;
  y: number;
}
function getInter(pt: inter1) {}
getInter({
  x: 100,
  y: 200,
});
```

<br>

interface 与 type 的继承实现方法

1. 接口使用关键词 extends 来继承父接口，在实现接口的时候务必记得所有参数都要实现！
2. 对象类型使用符号 & 来继承父对象

```ts
// 接口类型的继承实现
interface inter2_1 {
  name: string;
}
interface inter2_2 extends inter2_1 {
  age: number;
}
const it: inter2_2 = {
  name: "tom",
  age: 100,
};

// type继承
type t1 = {
  name: string;
};
type t2 = t1 & {
  age: number;
};
const tt: t2 = {
  name: "jerry",
  age: 100,
};
```

<br>

接口可以通过重名的方式来添加新成员，但是 type 不可以这么做！！！  
如下定义了两个同名接口 inter3，他们的成员会叠加在一起

```ts
// 成员的叠加性质关于接口使用
interface inter3 {
  i1: string;
}
interface inter3 {
  i2: boolean;
}
const it3: inter3 = {
  i1: "123",
  i2: true,
};
```

<br>

### TS 类型断言

1. 类型断言有两种写法，一个是末尾 as 带断言；另一个是代码最前面使用尖括号带断言
2. 断言没有运行时检查，他在编译时就已经被自动剔除了
3. 断言仅支持类型的向下转型检查，即我们没法对一个已经确认时 number 类型的变量断言为 string 类型

```ts
// 类型断言的两种方式
const mCav = document.getElementById("mcav") as HTMLCanvasElement;
const mCav2 = <HTMLCanvasElement>document.getElementById("mcav");

// 可以执行变量的向下转型检查
// 很明显把字符串转换成数字是错的，但这里先把字符串断言为未知类型，然后自然可以向下转型为string
const assertval = "hello" as unknown as number;
```

<br>

### TS 文字类型

- let 定义的文字类型可以不断地改变值，而 const 就像常量一样没办法改变
- 可以将字符串作为类型给形参使用，此时就表示了我们调用该函数时传入的实参只能是这些字符串  
  （如以下代码，我们形参规定两个字符串 left 和 right，那么调用时就只能使用这两个字符串之一作为实参）

```ts
// let和const两种不同方式定义文本类型
let strt1: string = "hello";
const strt2: string = "world";

// 有趣的是，我们可以把字符串直接作为类型，但赋予的值必须等于该字符串的值
const strt3: "name" = "name";

// 根据上一法则，同理将字符串类型作为函数的形参时的效果
// 只能使用left right之中的任意一个
function getStrt(st: string, st2: "left" | "right") {}
getStrt("helloworld", "left");
getStrt("donkey", "right");
```

<br>

文字类型转换细节处理  
如下代码所示，我们使用 req 存储值然后作为实参传入，我们在变量 req 的末尾加上了 as const 表示将该变量内部的成员全部变成文字类型，如果不这么做的话我们没有办法在下一行代码中使用 req.method 传入参数，这会导致类型不一致的错误

```ts
// 文字类型转换方法
function getUrl(url: string, method: "GET" | "POST" | "GUESS") {}
const req = {
  url: "https://www.xxx.com",
  method: "GET",
} as const;
getUrl(req.url, req.method);
```

<br>

### TS 中 null 和 undefined 类型

1. 当严格模式打开的情况下，没有办法为任何变量设置这两个初始值
2. 若函数形参为可选的且可选项中包含 null 或者 Undefined，那么在函数体内使用该形参的时候可以在点号前面加一个感叹号，表示断言该变量不可能时 null 的时候才执行

```ts
// 带感叹号的变量简单断言
function getAssert(x: number | null) {
  console.log(x!.toFixed());
}
```

<br>

### TS 枚举（特有方法）

使用方式类似于 JAVA 中的枚举，看着套用格式就好了  
（因为这是 TS 自创的方法，所以转换成 JS 代码会非常复杂）

```ts
// 枚举初步
enum COLOR {
  RED,
  YELLOW,
  BLACK,
}
console.log(COLOR.RED);
```

<br>

### TS 不常用原语

ES2020 新版本中新增了两个原语，TS 把它们移植了，使用方法了 JS 一致

```ts
// 存储大整数的原语，ES2020+才可以用
const bigint1: bigint = BigInt(10);
const bigint2: bigint = 100n;

// 另一个原语symbol的使用
const symbol1 = Symbol("name");
const symbol2 = Symbol("name");
```

<br>

### TS 类型缩小

当形参可以接受多种类型时，就需要对传入数据的类型进行检测

**typeof xxx === 'xxx'叫做类型守护**，表示判断某个变量是否属于某种类型，而且不用担心类型不符合等错误，TS 已经智能的为我们处理好了这些问题

```ts
function miniType(str: number | string) {
  if (typeof str === "number") {
    console.log(new Array(str));
  } else if (typeof str === "string") {
    console.log(str);
  }
}
```

<br>

### TS 真值缩小

真值：即除了 0 null undefined 0n 这些假值以外的所有值

真值缩小是为了精确定位传入值的数据类型，如下代码所示，对于传入的 strs 变量，我们仅仅判断它是一个 object 是不够的，因为 null 也是一个 object 对象，这很明显扰乱我们代码逻辑

此时我们在判定条件前面加上 **strs &&** 即可执行真值缩小，代码逻辑变成：该变量类型为 object 且同时为真值（非 null）才成立

```ts
function trueNarrow1(strs: string | string[] | null) {
  if (strs && typeof strs === "object") {
    for (const s of strs) {
      console.log(s);
    }
  }
}
```

<br>

### TS 等值缩小

使用 === 或者 !== 则表示严格的等于或者不等于机制

1. 上部分代码因形参都具有 string 类型，那么使用===表示当且仅当俩变量类型都是 string 的才执行代码
2. 下部分代码表示当他不等于 null 时才执行（特别注意他只针对 null 这个空值进行判断，因为它是严格的）

```ts
// ===符号
function equalNarrow1(s1: string | undefined, s2: string | number) {
  if (s1 === s2) {
    console.log(s1 + s2);
  }
}

// !==符号
function equalNarrow2(s3: number | null) {
  if (s3 !== null) {
    // ....
  }
}
```

<br>

使用!=来表示宽松的不等于机制
所谓宽松的，即指它是否定一类而不是单个类型，譬如 null 和 undefined 都属于同一类（无意义字符），所以我们只需要令变量 !=null 就可以包含 !=undefined 等多种状况

```ts
// !=宽松机制
function equalNarrow3(s4: string | null | undefined) {
  if (s4 != null) {
    // ...
  }
}
```

<br>

### TS in 操作符缩小

顾名思义，以下定义了两个物种，鸟和鱼，它们分别具有各自的移动特性，那么我们在判断条件里面使用 in 缩小操作符将移动条件限制于仅可以游泳，那么自然就会归类到鱼身上，此时的函数调用就不会出错了

```ts
type Fish = { swin: () => void };
type Bird = { fly: () => void };

function inNarrow(animal: Fish | Bird) {
  if ("swin" in animal) {
    animal.swin();
  } else {
    animal.fly();
  }
}
```

<br>

### TS instanceof 操作符缩小

和大多数编程语言一致，instanceof 检测前者是不是后者的一个实例，如果是的话返回 true

```ts
function instanceofNarrow(x: Date | string) {
  if (x instanceof Date) {
    console.log(x.toUTCString());
  } else {
    console.log(x.toUpperCase());
  }
}

instanceofNarrow(new Date());
instanceofNarrow("helloworld");
```

<br>

### TS 分配缩小

分配缩小即变量的类型会取决于变量定义式中的可能出现的类型趋向；  
说人话就是（以下方代码中的三元运算符示例）

1. 如果 random 取值小于 0.5，那么 x 被赋值 12，此刻 x 为 number 类型的
2. 如果 random 取值大于 0.5，那么 x 被赋值 too samll，此刻 x 为 string 类型的
3. 所以根据所有趋向，x 类型已经固定在了 number 和 string 之间，故不能再次把 x 赋予其他类型的值！

```ts
let x = Math.random() < 0.5 ? 12 : "too small!!!";

x = true; // 出错，类型不匹配！！！
```

<br>

### TS 类型谓词

开始变得复杂起来了= w =，请看下文步骤分解

1. 类型谓词定义于函数的返回值里面，格式为 parameter is type ，parameter 为形参，而 type 是我们之前定义好的类型
2. 在 return 语句一行，他表示先缩小 para 范围，然后判断里面有没有方法 swin，如果有的话 return true，即返回 para is ParameterTest1 为真，此时函数返回类型 ParameterTest1
3. 如果 return 判断的结果为 false，那么返回 para is ParameterTest1 为假，此时函数返回类型 ParameterTest2

```ts
type ParameterTest1 = {
  name: string;
  swin: () => void;
};

type ParameterTest2 = {
  name: string;
  fly: () => void;
};

function typeParameter(
  para: ParameterTest1 | ParameterTest2
): para is ParameterTest1 {
  return (para as ParameterTest1).swin !== undefined;
}
```

<br>

### TS 联合类型 unions

所谓的联合类型就是将两个及以上的类型结合成为一个变量，然后根据这些类型的共同参数来划分该联合类型到底属于那种子类型  
常常用到 switch 分支结构，如下方代码所示，根据 kind 的不同来定位出是与那个子类型

```ts
interface u1 {
  kind: "u1";
  para1: number;
}

interface u2 {
  kind: "u2";
  para2: number;
}

// 创建联合类型
type unionsType = u1 | u2;

// 根据联合类型中共同属性Kind，使用switch分支结构判断所属于那个接口
function unionsDistinct(u: unionsType) {
  switch (u.kind) {
    case "u1":
      return u.para1 ** 2;
    case "u2":
      return u.para2 ** 4;
  }
}
```

<br>

### TS 函数类型表达式

下面是一个函数类型代码的解析  
如文中的 fn: (a:string) => void 他是一个函数类型，他作为形参而存在

- fn 表示该函数的名称
- a:string 表示形参以及形参的类型
- => void 表示该函数不返回任何值

```ts
// 首先定义一个函数，该函数的形参是一个函数类型
function basicFunc1(fn: (a: string) => void) {
  fn("helloworld");
}

// 创建一个符合函数类型的函数
function basicFunc2(s: string) {
  console.log(s);
}

// 将函数作为实参传递进去
basicFunc1(basicFunc2);
```

<br>

当然，我们依然可以使用 type 来定义函数类型，此刻，我们可以将上文的 basicFunc1 改为以下 basicFunc3 来书写

```ts
type BasicFunc = (a: string) => void;

function basicFunc3(fn: BasicFunc) {
  fn("nameless");
}
```

<br>

### TS 调用签名

函数调用签名的格式如下方代码第一部分所示

- description: string 这表示签名，签名可以有多个但最好只保留一个
- (someThing:number) : boolean 表示函数接收 number 类型形参并且返回 boolean 值，特别注意这里返回值前面写的是冒号而不是箭头！

在用作形参的函数 descriptionFunc2 内，除了要定义和形参中规定一致格式的函数，还需要对签名做出定义！比如在该函数后面就紧跟着了 description 的定义

```ts
// 创建含调用签名的函数类型表达式
type DescriptionFunc = {
  description: string;
  (someThing: number): boolean;
};

// 中继函数，这里面调用了形参函数中的签名description
function descriptionFunc1(fn: DescriptionFunc) {
  console.log(fn.description + fn(100));
}

// 作为形参的函数，函数返回值以及形参一一对应之外，还需要对签名做出额外设置
function descriptionFunc2(num: number) {
  console.log(num);
  return false;
}
descriptionFunc2.description = "helloworld";

descriptionFunc1(descriptionFunc2);
```

<br>

### TS 构造签名

定义函数类型的方法和调用签名差不多，只是在函数类型的前面加了个 new 而已

- 特别注意，返回值类型的 String 是包装器类型，而形参中的 string 只是一个基元类型！
- 然后在中继函数中 return 是需要使用 new 关键词来构造函数

```ts
// 构造签名
type StructFunc = {
  new (s: string): String;
};

function structFunc(cstr: StructFunc) {
  return new cstr("helloworld");
}

const sFunc = structFunc(String);
```

<br>

### TS 泛型函数

和许多编程语言一样，TS 也有泛型函数，下面代码实现了获取数组第一个元素的泛型函数

- getFirstElement<T>(arr: T[]): T | undefined 一个标准的声明泛型函数的格式，T 表示泛型类型，这个字母可以任意取
- 在我们使用泛型函数时，可以不用尖括号声明类型，而是让编译器自己推断
- 如果我们使用尖括号指定了类型后，就只能使用该类型的数据了！

```ts
// 声明泛型函数
function getFirstElement<T>(arr: T[]): T | undefined {
  return arr[0];
}

getFirstElement<string>(["hello", "hey"]);
getFirstElement([1, 2, 3]);
```

<br>

手动实现 map 数据类型：

```ts
// 自己实现map数据类型
function map<Input, Output>(
  arr: Input[],
  func: (arg: Input) => Output
): Output[] {
  return arr.map(func);
}

const parsed = map(["1", "2", "3"], (n) => parseInt(n));
```

<br>

### TS 泛型函数限制条件

对泛型类型添加约束条件

1. <T extends { length: number }> 使用 extends 关键字对泛型类型 T 进行限制，花括号中写的是类型 T 必须具有的属性，所以这里面的名字不是乱取的！！！
2. 类型 T 现在约束为，必须具有属性 length，所以我们不能传入一个 number 作为实参，因为 number 类型没有属性 length
3. string 和 array 类型的数据才具有 length 属性，所以可以放心使用！

```ts
// 带有限制条件的泛型函数
function longestValue<T extends { length: number }>(a: T, b: T) {
  if (a.length > b.length) {
    // ...
  } else if (a.length < b.length) {
    // ...
  }
}

const lstr = longestValue("hellow", "world");
const nstr = longestValue(100, 10); // 错误！Number类型没有属性length
```

<br>

**指定类型参数**
当形参有两个及以上时，我们希望传入不同类型的实参，此时就需要显式指定类型参数，不能让编译器自动推断！！！

显示声明类型的顺序要和传入的实参类型一一配对，且尖括号内声明的类型使用竖线隔开

```ts
// 指定泛型函数类型
function setParametersType<T>(arr1: T[], arr2: T[]): T[] {
  return arr1.concat(arr2);
}

// 显示指定形参的泛型类型，注意顺序配对不可以出错
const spt = setParametersType<string | number>(["a", "b"], [1, 2]);
```

<br>

### TS 可选参数

可选参数的存在可以让用户选择传参与否，直接在形参冒号前面加一个问号即可

还有带有默认值的参数

```ts
// 形参带有默认值的函数
function seletedParameter(n: number = 100) {
  console.log(n.toFixed(3));
}

// 形参时可选的函数
function seletedParameter2(n?: number) {
  console.log(n.toPrecision(10));
}
```

<br>

**回调中的可选参数**

1. 法则：当为回调函数书写一个函数类型时，永远不要写一个可选参数！
2. 函数 returnSeleted 中第二个形参为一回调函数类型，在函数体内调用 callback 时传入的参数为 arr[i]
3. 我们实例化 callback 函数时，使用的变量 a 实际上只是一种代表而已

```ts
// 回调函数中永远不要写可选参数
function returnSeleted(arr: any[], callback: (arg: any) => void) {
  for (let i = 0; i < arr.length; i++) {
    callback(arr[i]);
  }
}

returnSeleted([1, 2, 3, 4], (a) => {
  console.log(a.toFixed(3));
});
```

<br>

### TS 函数重载

**重载签名：**即对重载函数进行预先定义，分为两种状况
**重载实现：**即对重载签名的实现，他必须处在重载签名的后面！

**特别注意**

1. 重载签名的定义中，形参的数目必须是从上到下依次递增的！并且下面的重载签名的形参必须要包含上面所有的形参
2. 在最后实现时我们发现形参 bc 都加了可选符号，是因为只有第二个重载签名新增了这两个形参，他们必须是可选的，不然则无法调用第一个重载签名的函数！

```ts
// 重载签名
function reloadFunc(a: number): number;
function reloadFunc(a: number, b: string, c: boolean): number;

// 重载实现
function reloadFunc(a: number, b?: string, c?: boolean): number {
  return a;
}
```

<br>

**形参的位置**

1. 在重载签名里面，形参重载是根据其位置做出决定的，无论重载签名有多少个，第一个形参名字怎么取，它们的含义都是一致的
2. 倘若两个重载签名都重载同一位置的形参（但是他们的类型不一致），那么在重载实现里面必须要对这个形参做出可选类型的处理
3. 以下代码不仅对形参做出了此操作，还有返回值也做出了可选的重载

```ts
// 注意重载签名必须按照顺序，形参数量从上到下逐渐增多或者不变
function reloadFunc2(a: number): string;
function reloadFunc2(a: string): number;
function reloadFunc2(a: string | number): string | number {
  return "helloworld";
}
```

<br>

### TS 其他常见类型

常见类型列表列表解析：

1. void 在 TS 中，一个不返回任何值的函数将默认返回 undefined 这个值
2. object 表示任何一个不是基元类型的类型
3. unknown 代表任何值，和 any 类似但更安全，因为对 Unknown 做出的任何操作都是非法的
4. never 表示永远不会被观察到的值
5. Function 该类型的值总可以被调用，这些调用全部返回 any

```ts
// return置空，默认表示返回值为void
function noop() {
  return;
}

// 不可能的值
function basicFail(msg: string): never {
  throw new Error(msg);
}

// Function
function basicFunction(f: Function) {
  return f(1, 2, 3, 4);
}
```

<br>

### TS 形参展开

TS 中的形参展开类似于 JAVA 的可变长的参数，即在形参前面加上三个英文句点，在调用时就可以传入无限个数的实参了

1. 形参展开的对象必须是一个数组
2. 形参展开必须位于函数形参中的最后一个！
3. 实参在传入的时候必须要按照顺序，即最后才传入的是形参展开

```ts
// 形参展开
// m.map(x => n * x)表示将m中的数据一一和n相乘然后生成一个map，最终返回该map
function restParameter(n: number, ...m: number[]) {
  return m.map((x) => n * x);
}

const rp = restParameter(10, 1, 2, 3, 4);

console.log(rp); // 输出[10,20,30,40]
```

<br>

### TS 实参展开

和形参展开类似，就是三个点+实参，传递一个数组进去

```ts
// 实参展开
const parr1 = [1, 2, 3];
const parr2 = [4, 5, 6];
parr1.push(...parr2);
```

<br>

### TS 参数解构

请看规则：

1. 函数中以对象的形式写出形参，并且逐一对这些形参设置类型
2. 调用该函数时，实参要以对象的形式传入，而且必须名称一一对应！

```ts
// 参数解构的定义
function parameterSum({ a, b, c }: { a: number; b: number; c: number }) {
  console.log(a + b + c);
}

// 调用参数解构必须遵照对象的格式来书写
parameterSum({
  a: 10,
  b: 20,
  c: 30,
});
```

<br>
