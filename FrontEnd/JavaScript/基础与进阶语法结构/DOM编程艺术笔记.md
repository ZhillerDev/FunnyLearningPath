### DOM 解释

`DOM` 文档对象模型
`BOM` 浏览器对象模型

JS 中的对象可以分为三种类型：

1. 用户定义对象：即码农们自己创建的对象
2. 内建对象：内建在 JS 语言里面的对象
3. 宿主对象：由浏览器提供的对象

<br>

**三大 DOM 节点**  
对于这一行代码： `<p title="test">123</p>`
`<p></p>` 称为元素节点  
`title="test` 称为属性节点  
`123` 称为文本节点

<br>

> 总而言之，一份文档就是一棵节点树，每个节点都是一个对象

<br>

**属性配置**
以下代码通过 `getElementsByTagName` 获取 p 标签的对象数组；  
因为 `getAttribute` 只能作用于 `object`，所以遍历对象数组来获取对应属性！

```js
// 获取对象数组后再依次取出属性
var paras = document.getElementsByTagName("p");
for (let i = 0; i < paras.length; i++) {
  alert(paras[i].getAttribute("title"));
}
```

同理，也存在 `setAttribute`，用来对指定的对象添加属性

```js
paras[0].setAttribute("title", "helloworld");
```

<br>

### JS 图片库

使用 childNodes 可以获取到 DOM 节点下的所有子节点，并返回一个对象数组

```js
var nodes = document.getElementsByTagName("body")[0].childNodes;
```

<br>

nodeType 可以获取节点的属性，他返回一个数值：  
元素节点为 1，属性节点为 2，文本节点为 3

<br>

### 最佳实践

**伪协议**  
即在一个 html 标签内嵌一个 js 代码块，类似于 eval()的作用！  
下面就在一个链接内嵌入伪协议，点击链接执行代码；
`<a href="javascript:alert('hello');>url</a>`

> 若浏览器不支持 js，那么伪协议亦失效
> 极其不推荐使用伪协议！！！

<br>

**内嵌点击事件**  
onclick 属性内写代码就可以了，这就是一个简单的点击事件
`<a href="#" onclick="alert('123');">123</a>`

<br>

**兼容性措施**
使用 if 判断加上取反符号，来检测该浏览器是否支持对应的方法，如果不支持直接 return false 退出代码

```js
if (!document.getElementById || !document.getElementsByTagName) return false;
```

<br>

**DOM 操作特点**  
每次进行一次 DOM 操作时，都会搜索全部的节点，造成很大的资源浪费；  
我们一般将 `<script>` 标签放在 `<body>` 标签里面，而不是放在 `<head>` 里

<br>

### 更优雅的脚本设计

<br>

**结构化程序设计理论**
定义：即函数都应该只有一个入口和一个出口；  
这代表着我们不应该同时使用多个 return 设置出口，而应该把它们聚合起来！

<br>

**addLoadEvent 函数**
该函数包含一个形参，该参数表示打算在页面加载完毕时执行的函数的名字，下面主义解释本函数将要实现的动作：

1. 把现有的 window.onload 事件处理函数的值存入变量 oldonload
2. 如果在这个处理函数上还没有绑定任何函数，就像平时那样把新函数添加给它
3. 如果在这个处理函数上已经绑定了一些函数，就把新函数追加到现有指令的末尾

```js
function addLoadEvent(func) {
  var oldonload = window.onload;
  if (typeof window.onload != "function") {
    window.onload = func;
  } else {
    window.onload = function () {
      oldonload();
      func();
    };
  }
}
```

<br>

**DOM Core 和 HTML-DOM**

DOM Core：诸如 getElementById 这些获取 DOM 节点的方法都不是 HTML 独有的，事实上任何一个支持 DOM 操作的程序设计语言都可以使用这些方法；

HTML-DOM：专属于 HTML 的特有 DOM 操作方法，譬如 HTML-DOM 提供了一个 forms 对象，可以进行以下两行代码的相互替代！
`document.getElementById('form');`  
`document.forms`

<br>

### 动态创建标记

<br>

**传统的动态创建标记的方法**  
document.write() 可以将一段字符串或者 HTML 标签插入到指定位置  
但他违背了“行为应该与表现分离”的原则
`document.write('<p>this is a sentence</p>');`

> XHTML 文档不会执行 document.write()方法！

innerHTML 作用和上面一个差不多，都可以直接对一个标签块进行修改然后呈递给浏览器并渲染！

> 同样的，XHTML 亦不支持本方法！

<br>

**createElement**  
可以使用 createElement 来创建一个新元素，并把它赋予给一个变量存储；  
createElement 得到的元素已经进行默认初始化，但他还没添加到任何一个节点中去，目前叫做 `孤儿节点`  
使用 appendChild 将孤儿节点添加到任意一个元素的下方作为子元素存在

```js
var para = document.createElement("p");
var testdiv = document.getElementById("testdiv");
testdiv.appendChild(para);
```

> createTextNode 创建文本节点，它的使用方法和创建元素是一致的，只不过他得到的结果就是一段字符串，需要把它 appendChild 到任何一个元素中去！

<br>

**ajax 重点问题**  
具体的 ajax 原理以及使用方法请参考本博客下的专门文章，这里不做过多赘述；  
下面来陈述一个重要的易错点：  
直接双击打开本地 html 文件时，若使用 ajax，就会被浏览器 `同源策略` 阻挡，因此我们必须要构建本地服务器来运行该 html 文件，才不会触发跨域；

<br>

### 文档优化手法

<br>

**渐进增强（progressive enhancement）原则**  
即从最核心的部分使用标记来实现良好的结构，然后再逐步增强这些内容；  
可以通过 CSS 改进，或者为 DOM 添加行为来进行增强；

<br>

**平稳退化原则**  
即缺少了必要的 CSS 或者 DOM 节点，用户进行访问时依然不会造成太大的阻碍

<br>

**XHTML5 和 HTML5 区别**  
HTML5 声明很简单，并且标签不区分大小写；  
XHTML5 是严格依照 XML 规则的 HTML，他的标签只能是小写的！

**lastChild 易错点**  
事实上，我们在提取某个元素的最后一个子元素时，不一定取得的是元素节点；  
可能会提取到文本节点，譬如以下代码：  
提取元素节点 blockquote 下的最后一个子元素，得到的是一个换行符，即一个文本节点！！！

> 故最好检测以下节点的 nodeType，来判断是否就是我们需要获取的元素节点而不是文本节点！

```html
<blockquote>
  <p>123</p>
</blockquote>
```

<br>

### CSS-DOM

<br>

**网页的三层结构**

1. 结构层：由 HTML 或 XHTML 之类的标记语言负责创建，即我们编写的标签
2. 表示层：有 CSS 负责来描述页面内容应该如何呈现
3. 行为层：负责内容应该如何响应事件这一问题，主要有 JS 和 DOM

<br>

**style 属性**  
在 JS 中，style 也是作为一个对象存在的，譬如如下根据某个 DOM 取出的 style 就是一个对象！  
`var styles = document.getElementById('123').style`

对于一般 style 属性，可以直接用小写英文单词获取，如获取元素的文本颜色： `element.style.color`  
而对于单横线链接的 style 属性，使用小驼峰形式获取，如获取 font-family 属性： `element.style.fontFamily`

在 JS 里面通过 style 对象直接改变元素的样式，需要传入的值必须是字符串！ `element.style.color = 'black'`

<br>

**DOM 操作 CSS 的方法**  
一种就像上面一样，直接使用 style 对象进行设置；  
另一种方式即使用 className，通过修改元素的 class，从而适配对应的 CSS

```js
function addClass(element, value) {
  if (!element.className) {
    // 如果不存在class，就赋予默认值value
    element.className = value;
  } else {
    // 如果存在class，那么就将该class替换为value
    element.className = value;
  }
}
```

<br>

### JS 实现动画

<br>

undefined
