### compose 获取资源文件的几种方式

文本： stringResource(R.string.hello_world)  
颜色： colorResource(R.color.black)  
尺寸： dimensionResource(R.dimen.padding)  
图片： painterResource(R.drawable.head_icon)

<br>

### Icon 组件

icon 组件可以接受三种类型的图片，分别为：bitmap、imagevector、painter

imagevector 一般直接使用 compose 内置的图标

内置图标有两种：

1. 镂空的 Icons.Default.xxx
2. 实心的 Icons.Filled.xxx

`Icon(imageVector = Icons.Default.Add, contentDescription = "demo")`

<br>

而对于 painter，直接可以使用 res 的 drawable 文件夹下我们存储的 xml 后缀图标文件

注意这里需要使用 painterResource 通过 ID 获取到指定图标文件

`Icon(painter = painterResource(id = R.drawable.ic_pwd), contentDescription = "good")`

<br>
