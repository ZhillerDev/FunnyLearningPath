### 最简启动页

新建一个 UI 节点

随便写一个 label 和 button，button 是用来进入主场景的

点击进入 `项目->项目设置->运行` 在里面找到“主场景”，选择我们的 UI 页面，此时启动游戏就默认进入 UI！

为 UI 根节点添加代码，将按钮点击事件连接到该代码内部，实现点击按钮改变场景以进入游戏主场景

```go
extends Control

func _on_Button_pressed():
	get_tree().change_scene("res://scenes/World.tscn")
```

<br>
