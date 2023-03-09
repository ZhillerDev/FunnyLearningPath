### 第一人称控制器

#### 角色配置

所有节点展示

`DirectionalLight3D` 标准太阳光  
`WorldEnvironment` 标准世界环境

`MeshInstance3D` 带碰撞的地面  
`CharacterBody3D` 玩家  
`Neck` 一个空物体，挂载在玩家的颈部位置

![](../images/script/fps/f1.png)

neck 是一个空物体，他处于玩家的颈部位置，其下有一子节点为摄像机，我们之后即控制 neck 物体的旋转来带动摄像机以实现自由视角效果

这是 neck 以及对应摄像机的部位（直接右键点击 neck 并创建摄像机节点，摄像机会默认在该节点位置处创建，不需要二次调整位置！）

![](../images/script/fps/f2.png)

<br>

#### 脚本

为对象 CharacterBody3D 创建脚本

注意，默认 godot 会为该脚本指定一个模板：CharacterDefault，我们可以直接选用该第一人称控制器模板做二次开发，或者完全从头开始手写，这取决于你

这是对应的完整代码：

```go
extends CharacterBody3D

# 定义移动速度以及跳跃高度（强度）
const SPEED = 5.0
const JUMP_VELOCITY = 6

var gravity = ProjectSettings.get_setting("physics/3d/default_gravity")

# onready初始化，获取节点
# := 看起来像是go的写法，在这里表示自动类型推断并赋值
@onready var neck := $Neck
@onready var camera := $Neck/Camera3D

# 鼠标移动相机跟随与锁定光标方法
func _unhandled_input(event):
	# 当点击游戏窗口自动锁定鼠标且隐藏光标
	# 按下exit退出焦点锁定
	if event is InputEventMouseButton:
		Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	elif event.is_action_pressed("ui_cancel"):
		Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)

	# 获取鼠标移动方向，设置摄像机跟随该方向，实现上下左右自由视角
	if Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
		if event is InputEventMouseMotion:
			neck.rotate_y(-event.relative.x *0.01)
			camera.rotate_x(-event.relative.y *0.01)
			camera.rotation.x = clamp(camera.rotation.x,deg_to_rad(-30),deg_to_rad(60))


func _physics_process(delta):
	# 判断是否在地面，以此决定能否再次按下跳跃按键
	if not is_on_floor():
		velocity.y -= gravity * delta
	# 控制跳跃
	if Input.is_action_just_pressed("ui_accept") and is_on_floor():
		velocity.y = JUMP_VELOCITY

	# 使用我们映射的wsad按键进行角色移动
	var input_dir = Input.get_vector("left", "right", "forward", "back")

	# neck.transform.basis获取当前脖子朝向，使角色向该角度前进！
	var direction = (neck.transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	if direction:
		velocity.x = direction.x * SPEED
		velocity.z = direction.z * SPEED
	else:
		velocity.x = move_toward(velocity.x, 0, SPEED)
		velocity.z = move_toward(velocity.z, 0, SPEED)

	move_and_slide()

```
