### 核心概念

#### JSX

`JSX` 内大括号可以写任意 js 代码，同理可以调用变量

`ReactDOM.render` 将指定 JSX 渲染到指定 DOM 上面去

```js
const name = "Josh Perez";
const element = <h1>Hello, {name}</h1>;

ReactDOM.render(element, document.getElementById("root"));
```

<br>

JSX 可以当做参数传入，或者返回

```js
// 变量直接赋值JSX完全没问题
const element = <div tabIndex="0"></div>;

// user这个形参可以直接传入JSX
function getGreeting(user) {
  return <h1>Hello, Stranger.</h1>;
}
```

<br>

两种创建 JSX 的方法

```js
// 方法一
const element = <h1 className="greeting">Hello, world!</h1>;

// 方法二
const element = React.createElement(
  "h1",
  { className: "greeting" },
  "Hello, world!"
);
```

<br>

#### 组件与传参

react 的组件定义最方便的方法是：直接用函数，接受一个形参即 props

自定义组件的属性直接传给形参 props，如下代码传参后的结果是： `{name:'Sara'}`

> 组件名必须大写开头

```js
// 顶一个组件，props为参数
function Welcome(props) {
  return <h1>Hello, {props.name}</h1>;
}

// 针对自定义组件，所有的属性都统一作为参数传递给props
const element = <Welcome name="Sara" />;
ReactDOM.render(element, document.getElementById("root"));
```

<br>

把组件单独抽离出来便于复用，这是一个好习惯！

<br>

#### 生命周期

若要使用 state 控制组件生命周期，请改用 class 来声明组件而非函数式

在同一 DOM 内多次调用 render 时，class 式声明的组件在其生命周期内只会存在一个实例！

```js
class Clock extends React.Component {
  render() {
    return (
      <div>
        <h1>Hello, world!</h1>
        <h2>It is {this.props.date.toLocaleTimeString()}.</h2>
      </div>
    );
  }
}
```

<br>

`this.props` 与 `this.state` 是 react 内部字段

这是一个完整实现每隔 1s 更新一次时间的组件 Clock

1. componentDidMount 挂载完毕钩子
2. componentWillUnmount 卸载前钩子

```js
class Clock extends React.Component {
  // 组件的构造函数
  constructor(props) {
    super(props);
    // state是内部值，直接向里面丢东西
    this.state = { date: new Date() };
  }

  // 挂载完毕，开启间隔计时器，每1s执行一次方法tick()
  componentDidMount() {
    this.timerID = setInterval(() => this.tick(), 1000);
  }

  // 卸载前清除计时器
  componentWillUnmount() {
    clearInterval(this.timerID);
  }

  // 间隔更新函数，setState（差不多和微信小程序一个意思）修改值
  tick() {
    this.setState({
      date: new Date(),
    });
  }

  // render里面渲染DOM
  render() {
    return (
      <div>
        <h1>Hello, world!</h1>
        <h2>It is {this.state.date.toLocaleTimeString()}.</h2>
      </div>
    );
  }
}

// 渲染，没啥好说的
ReactDOM.render(<Clock />, document.getElementById("root"));
```

<br>

state 注意事项

1. 更新 state 中的值务必使用 `setState` 方法，否则改数据后不会重新渲染！
2. state 更新可能是异步的，可以在 setState 方法内使用函数来替换对象写法
3. state 是单向数据流，即父组件的 state 可以传递给其子孙组件并影响他们的渲染，但父组件的 state 无法影响其上级（如果父组件还存在一个父组件的话！）

<br>

#### 事件处理

在 react 中，阻止组件的默认行为只能通过 js 代码实现  
下面将一个函数抽离，并使用 preventDefault 阻止组件默认行为

```js
function ActionLink() {
  function handleClick(e) {
    // 阻止组件的默认行为
    e.preventDefault();
    console.log("The link was clicked.");
  }

  return (
    <a
      href="#"
      onClick={handleClick}
    >
      Click me
    </a>
  );
}
```

<br>

class 式创建自定义组件时，其中的方法不会自动绑定 this，所以需要我们手动改变其 this 指向到对应的 class 里面去

原理：当你调用一个方法时若不在结尾添加一个小括号，就必须明确其 this 指向，如下代码，我们调用 `handleClick` 方法时没有加小括号，故需要在 `constructor` 函数内改变其 this 指向

```js
class Toggle extends React.Component {
  constructor(props) {
    ...

    // 改变class内的方法handleClick的this指向为本class
    this.handleClick = this.handleClick.bind(this);
  }

  handleClick() {
    ...
  }

  render() {
    return (
      // 此情况下必须要为方法handleClick指定this指向
      <button onClick={this.handleClick}>
        {this.state.isToggleOn ? "ON" : "OFF"}
      </button>
    );
  }
}
```

<br>

这是两种等价的，为方法确定正确的 this 指向的写法

```js
<button onClick={(e) => this.deleteRow(id, e)}>Delete Row</button>
<button onClick={this.deleteRow.bind(this, id)}>Delete Row</button>
```

<br>

#### 条件渲染

与运算符&&

react 基本条件运算，根据之前我们所学知识，花括号内可以填写任意表达式；故以下代码判断，当 `unreadMessages.length > 0` 条件成立时就会执行 `&&` 运算符后面的 JSX

```js
function Mailbox(props) {
  const unreadMessages = props.unreadMessages;
  return (
    <div>
      <h1>Hello!</h1>
      {unreadMessages.length > 0 && (
        <h2>You have {unreadMessages.length} unread messages.</h2>
      )}
    </div>
  );
}
```

<br>

和 vue 一致，你也可以使用三元运算符

```js
render() {
  const isLoggedIn = this.state.isLoggedIn;
  return (
    <div>
      The user is <b>{isLoggedIn ? 'currently' : 'not'}</b> logged in.
    </div>
  );
}
```

<br>

如果你想让组件不渲染，直接返回 null  
`return null`

<br>

#### 列表渲染

react 的列表渲染比较朴实一点，直接调用元素 map 迭代渲染

但和 vue 一样，最好使用 key，否则会出现警告

```js
function NumberList(props) {
  const numbers = props.numbers;

  // 使用map取出numbers内部的所有值，渲染到指定JSX
  const listItems = numbers.map((number) => (
    // 万不得已的情况下可以直接拿值作为key
    <li key={number.toString()}>{number}</li>
  ));
  return <ul>{listItems}</ul>;
}

const numbers = [1, 2, 3, 4, 5];
ReactDOM.render(
  <NumberList numbers={numbers} />,
  document.getElementById("root")
);
```

不过官方推荐数据若没有给出 id 值的话，可以拿 map 的索引值作为 key

```js
// 这里的todo为值，而index是map函数为我们生成的对应索引
const todoItems = todos.map((todo, index) => <li key={index}>{todo.text}</li>);
```

<br>

关于 key 使用的注意事项

1. key 必须在 map 函数内定义！
2. key 在其兄弟节点之间必须唯一，但是全局范围内可以重复

<br>

直接内联 return 一个列表渲染的 JSX，省去了冗余的中间变量

```js
function NumberList(props) {
  const numbers = props.numbers;
  return (
    <ul>
      {numbers.map((number) => (
        <ListItem
          key={number.toString()}
          value={number}
        />
      ))}
    </ul>
  );
}
```

<br>

#### 受控组件

react 中的表单采用将所有数值都统一由 state 进行管理

代码解释：
表单 form 中的 input，其 value 属性直接由顶层 state `管理；onChange` 也是由函数 `handleChange` 动态更新 state 中指定属性的值；

> 由于构造函数内 state 已经初始化为空字符串，故不用担心渲染 input 时会出错！

```js
class NameForm extends React.Component {
  constructor(props) {
    super(props);
    this.state = { value: "" };

    // 没有小括号的函数调用必须要明确指定this指向
    // 这里没有使用箭头函数的方法，而是使用bind绑定
    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleChange(event) {
    this.setState({ value: event.target.value });
  }

  // 表单提交操作
  handleSubmit(event) {
    alert("提交的名字: " + this.state.value);
    // 阻止默认的表单post或者get操作，由该方法进行自定义指定
    event.preventDefault();
  }

  render() {
    return (
      <form onSubmit={this.handleSubmit}>
        <label>
          名字:
          <input
            type="text"
            value={this.state.value}
            onChange={this.handleChange}
          />
        </label>
        <input
          type="submit"
          value="提交"
        />
      </form>
    );
  }
}
```

<br>

对于文本域 `textarea` ，我们不再向其内部写标签，直接使用 `value` 作为该文本域的内容

```js
<textarea
  value={this.state.value}
  onChange={this.handleChange}
/>
```

<br>

select 组件里面的 value 直接指代当前激活的是哪一个 option，他的值等于当前几乎跌 option 的 value 值

```js
<select
  value={this.state.value}
  onChange={this.handleChange}
>
  <option value="grapefruit">葡萄柚</option>
  <option value="lime">酸橙</option>
  <option value="coconut">椰子</option>
  <option value="mango">芒果</option>
</select>
```

<br>

当存在多个 input 时，为他们添加 name 属性，设置不同名字，并在 onchange 函数内部进行判断即可动态更新对应的 state

```js
handleInputChange(event) {
    ...
    // 获取当前input的name属性值
    const name = target.name;

    // 根据键值对的方法，赋予指定name的值为value
    this.setState({
      [name]: value
    });
  }
 render() {
    return (
      <form>
        <label>
          参与:
          <input
            name="isGoing"
            type="checkbox"
            checked={this.state.isGoing}
            onChange={this.handleInputChange} />
        </label>
        ...
      </form>
    );
  }
```

<br>

#### React 哲学？

我不需要哲学，代码能跑就行

<br>

### 高级部分

#### 组合与继承

> 这部分概念和 Vue 的插槽很像，大家可以类比学习一下

我们首先准备一个函数组件 `PropTest.jsx`

`props.children` 可以看做是存放所有父组件传递过来的 DOM 的插槽，在我们没有提供具名插槽（vue 的叫法，不是 react 官方术语）时，父组件传递过来的所有 DOM 都会在这里渲染

```jsx
import React from "react";

export default function PropsTest(props) {
  // style样式设置为父组件传递过来的参数color
  return <div style={{ color: props.color }}>{props.children}</div>;
}
```

很好，现在我们于父组件调用子组件，同时传递了两个标签  
这两个标签都会在 `props.children` 的位置渲染

```jsx
import PropsTest from "../components/PropsTest";

export default function App() {
  return (
    // 我们为子组件的插槽传递了两个DOM，他们均会被渲染
    <PropsTest color="blue">
      <p>this is a place</p>
      <div>another</div>
    </PropsTest>
  );
}
```

<br>

> 就像我们前面说过的，可以使用具名插槽

稍微改动一下 `PropsTest.jsx`  
我们提供了两个具名插槽，slot1、slot2

```jsx
import React from "react";

export default function PropsTest(props) {
  return (
    <div>
      {props.slot1}
      {props.slot2}
    </div>
  );
}
```

注意，此时在父组件内，插槽是作为属性写在子组件标签内部的！

```jsx
import PropsTest from "../components/PropsTest";

export default function App() {
  // 简单的为两个插槽添加了各自的div标签
  return (
    <PropsTest
      slot1={<div>123</div>}
      slot2={<div>456</div>}
    />
  );
}
```

<br>

#### 代码分割

React.lazy 实现动态懒加载组件，他接受一个函数，该函数 import 对应的组件

欲懒加载的组件在调用时必须使用 `Suspense` 标签包裹  
且 `Suspense` 标签内部可以嵌套任意多层非懒加载组件标签，但必须要保证懒加载组件标签在 Suspense 标签的内部

`Suspense` 的 `fallback` 属性提供元素加载过程时需要显示的内容

```jsx
import React, { Suspense } from "react";

const LazyTest = React.lazy(() => import("../components/LazyTest"));

export default function App() {
  return (
    <Suspense fallback={<div>loading...</div>}>
      <LazyTest />
    </Suspense>
  );
}
```

> React.lazy 懒加载的模块必须使用 `export default`，它不支持具名导出模式

<br>

#### Context

> `context` 作用是解决组件透传时需要频繁书写 props 的冗余，仅需一次声明 context，即可透传任意深度的 DOM 树

定义一个 context 的步骤如下：

1. `React.createContext` 在组件顶层创建一个新的 context，他接受一个参数表示上下文的默认值
2. `<MyContext.Provider value="what">` 固定格式，表示透传参数，value 属性指定透传给后续 DOM 的上下文的值
3. `useContext(MyContext)` 最新的 hook，获取 context，接收的一个参数为我们顶层定义的 `context`
4. 直接使用 context

```jsx
import React, { useContext } from "react";

// 定义一个context
const MyContext = React.createContext("damn");

// 起始组件
export default function ContextTest() {
  return (
    // Provider透传context
    <MyContext.Provider value="what">
      <Demo1 />
    </MyContext.Provider>
  );
}

// 中间组件
function Demo1() {
  return <Demo2 />;
}
// 末端组件
function Demo2() {
  // useContext获取指定上下文
  const ctx = useContext(MyContext);
  return <div>{ctx}</div>;
}
```

该代码最终实现的结果是：定义一个默认值为 damn 的 context，使用 provider 定义 context 的新值为 what 并透传给下属所有的 DOM；后续任意层次的组件内部仅需使用 useContext 钩子即可获取对应 context 存储的内容

<br>

`Consumer` 用于快速获取 context，下方代码的 value 代表获取到的值，以箭头函数的形式使用该值

此处 `context` 值为最近一个 provider 所定义的值，如果整个 DOM 树都没有 provider，那么直接使用顶层定义的默认 context 值

```jsx
function Demo2() {
  return (
    <MyContext.Consumer>{(value) => <div>{value}</div>}</MyContext.Consumer>
  );
}
```

<br>

**动态 context**

动态 context 是为了协助我们在深层次嵌套的组件内部更新 context 数据

首先定义顶层 context，该 context 包含一个属性以及修改属性的方法  
代码清单：`default-context.js`

```js
import React from "react";

export const DemoContext = React.createContext({
  count: 0,
  toggleCount: () => {},
});
```

之后就是主代码，主要实现功能为点击按钮给数值+1

由于我们的 `DemoContext` 存储了一个包含两个属性的对象，故在使用该 context 时必须要对其进行解构，确保两个属性都用上！！！

```jsx
import React, { useState } from "react";
import { DemoContext } from "../constant/default-context";

// 被调用的组件
function ToggleTest() {
  return (
    // 解构DemoContext，分为count和toggleCount
    <DemoContext.Consumer>
      {({ count, toggleCount }) => (
        <>
          <div>{count}</div>
          <button onClick={toggleCount}>点我加一</button>
        </>
      )}
    </DemoContext.Consumer>
  );
}

// 主组件
export default function DeepContext() {
  // 简单定义一个state
  const [count, setCount] = useState(0);
  return (
    // 因为定义的context是对象，故依葫芦画瓢这里也必须以对象的形式传入value
    // setCount必须以函数的形式执行，否则react编译报错
    <DemoContext.Provider
      value={{
        count: count,
        toggleCount: () => setCount(count + 1),
      }}
    >
      <ToggleTest />
    </DemoContext.Provider>
  );
}
```

<br>

#### Fragments

`Fragments` 可以视为 vue 中的 template 标签，它提供一个空的，无实际意义的标签作为根标签包裹其余标签

`Fragments` 可以使用短语法实现：`<></>`

下面是一个渲染 ul 的方法

```jsx
import React from "react";

export default function FragmentsTest() {
  return (
    <ul>
      <FragmentsFragment name="tom" />
      <FragmentsFragment name="jack" />
      <FragmentsFragment name="har" />
      <FragmentsFragment name="asd" />
    </ul>
  );
}

function FragmentsFragment(props) {
  return (
    <>
      <li>{props.name}</li>
    </>
  );
}
```

<br>

#### 高阶组件 HOC

所谓高阶组件，即接收一个参数为组件，返回值为新组件

下面的案例展示了使用高阶组件配合 props 传参

```jsx
import React from "react";

// 高阶组件HOC
function withDescription(Comp) {
  return class extends React.Component {
    constructor(props) {
      super(props);
      this.state = { flag: "asd" };
    }
    render() {
      return <Comp data={this.state.flag} />;
    }
  };
}

// 欲被实例化的组件
function InnerComp(props) {
  return <div>{props.data}</div>;
}

// 调用高阶组件
export default function HOCTest() {
  const Desp = withDescription(InnerComp);
  return <Desp />;
}
```

<br>

#### 第三方库协调

> 目前用不到，不更新

<br>

#### 深入理解 JSX

react 组件必须以大写字母作为开头，否则将会被识别为普通 HTML 标签

花括号内的语句会被解析，所以以下两个写法是等价的

```jsx
<MyComponent message="&lt;3" />
<MyComponent message={'<3'} />
```

<br>

如果你想渲染 `false、true、null、undefined` 等值，你需要先将它们转换为字符串

```jsx
<div>My JavaScript variable is {String(myVariable)}.</div>
```

<br>

#### Portals

> 技术不足，无法解析

<br>

#### Refs to DOM

这是官方给出的，通过获取 refs 指定的 DOM，然后触发 input 获取焦点

```jsx
import React from "react";

export class CustomTextInput extends React.Component {
  constructor(props) {
    super(props);
    // 创建一个 ref 来存储 textInput 的 DOM 元素
    this.textInput = React.createRef();
    this.focusTextInput = this.focusTextInput.bind(this);
  }

  focusTextInput() {
    // 直接使用原生 API 使 text 输入框获得焦点
    // 注意：我们通过 "current" 来访问 DOM 节点
    this.textInput.current.focus();
  }

  render() {
    // 告诉 React 我们想把 <input> ref 关联到
    // 构造器里创建的 `textInput` 上
    return (
      <div>
        <input
          type="text"
          ref={this.textInput}
        />
        <input
          type="button"
          value="Focus the text input"
          onClick={this.focusTextInput}
        />
      </div>
    );
  }
}
```

<br>

请注意，在函数式组件内无法直接使用 ref，因为没有 DOM；  
但是可以使用 `useRef` 或者 `forwardRef` 替代之

下面即通过 `useRef` 钩子获取到 DOM，然后执行内容修改的一个简单函数式组件

```jsx
export default function RefsDom() {
  // 获取DOM时需要预先指定好Ref对象
  const textRef = useRef(null);
  function changeText() {
    // 获取DOM并改变内容
    textRef.current.innerHTML = "asd";
  }
  return (
    <React.Fragment>
      <div ref={textRef}>normal</div>
      <button onClick={changeText}>改变数据</button>
    </React.Fragment>
  );
}
```

<br>
