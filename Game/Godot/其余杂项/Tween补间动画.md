### 基础补间

> 本补间动画在 2d 层面执行，使用 godot4 版本

新建一个 sprite2d 作为我们的基础角色，创建过程不再赘述

为刚创建的 sprite 挂载如下代码，实现鼠标点击一个位置，sprite 自动移动到该位置

首先需要使用 `create_tween()` 获取 `tween` 实例

`tween_property` 接收四个参数：

- 参数一：欲添加补间动画的对象
- 参数二：初始状态（这是一个内部属性，可以使 position 也可以是 rotation_degrees）
- 参数三：截止状态，这里设置为鼠标点击位置
- 参数四：补间动画执行时长，单位为秒

```go
extends Node2D

func _input(event):
	if event is InputEventMouseButton:
		var tween = create_tween()
		tween.tween_property(self,"position",event.position,1)
```

<br>

### 并行补间

在基础补间的基础上，我们在获取 tween 实例之时，使用 `set_parallel` 设置该实例下的所有补间动画均为并行执行的

什么意思？即所有动画都会在同一时刻进行

> 注意：在并行模式下，为避免动画冲突，使用 `tween_property` 设置动画时，被执行的属性最好是不一样的！即不可以同时设置两个 `position` 属性的动画！！！

```go
extends Node2D

func _input(event):
	if event is InputEventMouseButton:
        # set_parallel设置为true，表示该实例下的所有动画均为并行执行的
		var tween = create_tween().set_parallel(true)
		tween.tween_property(self,"position",event.position,1)
		tween.tween_property(self,"rotation_degrees",self.rotation_degrees+180,1)
```

<br>

当然，你也可以单一的针对某一个动画使用并行模式

为 `tween_property` 之前添加 `parallel` 方法即可使其变成并行运行模式

```go
extends Node2D

func _input(event):
	if event is InputEventMouseButton:
		var tween = create_tween()
		tween.tween_property(self,"position",event.position,1)
		tween.parallel().tween_property(self,"rotation_degrees",self.rotation_degrees+180,1)
```

<br>

### 后缀属性

`set_delay()` 让动画延迟 x 秒后执行  
`tween.tween_property(self,"rotation_degrees",180,1).set_delay(1)`

`from_current()` 从当前状态执行到目标状态  
`tween.tween_property(self,"rotation_degrees",180,1).from_current()`

`from()` 从指定状态执行到目标状态  
`tween.tween_property(self,"rotation_degrees",180,1).from(90)`

<br>

`set_ease` 设置线性动画过渡曲线

`set_trans` 同样是设置过渡曲线，但是附带一定物理特效

<br>
