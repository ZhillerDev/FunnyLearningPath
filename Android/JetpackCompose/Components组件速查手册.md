### TextField

> 文本域

#### 简单用户输入框

value 编辑框内文本  
onValueChange 当文本改动时需要做出的操作  
label 文本域标题  
placeholder 懂得都懂

leadingIcon 文本域开头图标  
trailingIcon 文本域结尾图标

<br>

```kotlin
@Composable
fun TextFieldDemo() {
    var content by remember {
        mutableStateOf("")
    }
    TextField(
        value = content,
        onValueChange = { content = it },
        label = @Composable { Text(text = "用户名") },
        placeholder = @Composable { Text(text = "请输入您的用户名") },
        leadingIcon = @Composable { Icon(painterResource(id = R.drawable.ic_user), null) },
        trailingIcon = @Composable {
            // 这里使用官方自带图标模拟了叉号
            // pointerInput是为了禁止水波纹效果，更加符合真实条件
            Icon(
                Icons.Default.Close,
                null,
                modifier = Modifier.pointerInput(Unit) {
                    detectTapGestures(onTap = { content = "" })
                }
            )
        }
    )
}
```

<br>

#### 搜索框

思路：关闭错误提示，添加些许圆角，当编辑框有焦点和无焦点时下方视觉条都是透明的

isError 是否有错误提示（即编辑框变红）

```kotlin
@Composable
fun SearchTextField() {
    var content by remember {
        mutableStateOf("")
    }
    TextField(
        value = content, onValueChange = { content = it },
        placeholder = { Text(text = "search for") },
        leadingIcon = { Icon(Icons.Default.Search, null) },
        trailingIcon = {
            Icon(
                Icons.Default.Close,
                null,
                modifier = Modifier.pointerInput(Unit) {
                    detectTapGestures(onTap = { content = "" })
                }
            )
        },

        // 错误信息不显示
        isError = false,

        // 设置有焦点和无焦点时编辑框底部视觉引导条都是透明的
        colors = TextFieldDefaults.textFieldColors(
            focusedIndicatorColor = Color.Transparent,  // 有焦点时
            unfocusedIndicatorColor = Color.Transparent // 无焦点时
        ),

        // 加一个圆角
        shape = RoundedCornerShape(8.dp)
    )
}
```

<br>

#### 密码输入

我们只需要关注密文输入的方式以及限制行数这两个主要问题就差不多了，剩下前面的内容可以直接照搬代码

```kotlin
@Composable
fun PasswordField() {
    var content by remember {
        mutableStateOf("")
    }
    OutlinedTextField(
        ...

        // 密文输入方式
        visualTransformation = PasswordVisualTransformation(),

        // 输入时键盘的回车键做出的动作类型
        keyboardOptions = KeyboardOptions(imeAction = ImeAction.Done),

        // 限制为单行
        singleLine = true
    )
}
```

<br>
