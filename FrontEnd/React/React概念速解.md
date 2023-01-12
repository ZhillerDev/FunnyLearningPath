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
    <a href="#" onClick={handleClick}>
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
        <ListItem key={number.toString()} value={number} />
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
        <input type="submit" value="提交" />
      </form>
    );
  }
}
```

<br>

对于文本域 `textarea` ，我们不再向其内部写标签，直接使用 `value` 作为该文本域的内容

```js
<textarea value={this.state.value} onChange={this.handleChange} />
```

<br>

select 组件里面的 value 直接指代当前激活的是哪一个 option，他的值等于当前几乎跌 option 的 value 值

```js
<select value={this.state.value} onChange={this.handleChange}>
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

### 高级部分

####
