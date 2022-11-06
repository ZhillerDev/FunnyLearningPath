### Type

#### 如何判断空值

由于 js 特性，许多数据类型在不同情况下都可能变成空值，现举出几个常见的空值例子：

1. 任何假值都应为空值（比如空、未定义、0、 false、”和 NaN）
2. 空数组和对象也应该被认为是空的
3. 只有空格的字符串也应该被认为是空的
4. 无效的 Date 实例以及空的 Set 和 Map 实例都应视为空白

<br>

由此我们可以封装判断空值的方法

```js
const isFalsy = (value) => !value;
const isWhitespaceString = (value) =>
  typeof value === "string" && /^\s*$/.test(value);
const isEmptyCollection = (value) =>
  (Array.isArray(value) || value === Object(value)) &&
  !Object.keys(value).length;
const isInvalidDate = (value) =>
  value instanceof Date && Number.isNaN(value.getTime());
const isEmptySet = (value) => value instanceof Set && value.size === 0;
const isEmptyMap = (value) => value instanceof Map && value.size === 0;

// 判断空值的方法
const isBlank = (value) => {
  if (isFalsy(value)) return true;
  if (isWhitespaceString(value)) return true;
  if (isEmptyCollection(value)) return true;
  if (isInvalidDate(value)) return true;
  if (isEmptySet(value)) return true;
  if (isEmptyMap(value)) return true;
  return false;
};
```

<br>

#### Boolean 过滤器的使用

我们可以使用 boolean 类型过滤真值

```js
const values = [0, 0, 2, 0, 3];
const hasValidValue = values.some(Boolean);
const nonEmptyValues = values.filter(Boolean);
```

<br>

尽量避免使用 Boolean 构造函数，因为即便你传入的数值为 false，该 boolean 对象依然被视为 true  
`let b = Boolean(false); // 检测该对象时，实际结果为true`

<br>

#### 真值与假值

有六个公认的假值：`false,undefined,null,""'',NaN,0`

你可以使用双重否定 `!!` 或者 Boolean 构造函数 `Boolean(xxx)` 来检测真值与假值

<br>

#### 检查对象相等

下面的封装方法解决了所有可能遇见的相等判定行为检测

```js
const equals = (a, b) => {
  if (a === b) return true;

  if (a instanceof Date && b instanceof Date)
    return a.getTime() === b.getTime();

  if (!a || !b || (typeof a !== "object" && typeof b !== "object"))
    return a === b;

  if (a.prototype !== b.prototype) return false;

  const keys = Object.keys(a);
  if (keys.length !== Object.keys(b).length) return false;

  return keys.every((k) => equals(a[k], b[k]));
};
```

<br>

#### 将字符串布尔值转换为布尔值

因为 js 中所有非空字符串都被认为有意义，故我们需要将元素字符串消除大小写以及前后空格，然后再与标准布尔值“true”比较并得出结果

```js
const toBoolean = (value, truthyValues = ["true"]) => {
  const normalizedValue = String(value).toLowerCase().trim();
  return truthyValues.includes(normalizedValue);
};

toBoolean("true"); // true
toBoolean("TRUE"); // true
```

<br>

#### 检测集合是否为空的

即检测值是否为 null 或者集合长度为 0

```js
const isEmpty = (val) => val == null || !(Object.keys(val) || val).length;
```

<br>

#### 检查一个值是否是 promise 类型的

判断非空+是一个对象或者函数+原型内具有默认方法 then=promise

```js
const isPromiseLike = (obj) =>
  obj !== null &&
  (typeof obj === "object" || typeof obj === "function") &&
  typeof obj.then === "function";

isPromiseLike({
  then: function () {
    return "";
  },
}); // true
```

<br>

#### 值是异步函数

直接取出函数原型进行比对

```js
const isAsyncFunction = (val) =>
  Object.prototype.toString.call(val) === "[object AsyncFunction]";

isAsyncFunction(function () {}); // false
isAsyncFunction(async function () {}); // true
```

<br>
