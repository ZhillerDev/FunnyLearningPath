### 基本射线检测模板

```cs
public Camera camera;
public GameObject box;

// 检测鼠标左键单击
if (Input.GetMouseButtonDown(0))
{
    // 从指定摄像机射出一条射线，射线从玩家屏幕点击的位置射出
    Ray ray = camera.ScreenPointToRay(Input.mousePosition);
    // 碰撞对象
    RaycastHit hit;
    // 发出射线并获取被碰撞到的对象，返回一个布尔值表示是否碰撞到了东西
    bool res = Physics.Raycast(ray, out hit);
    // 如果碰到了
    if (res)
    {
        // 输出射线碰撞的位置
        Debug.Log(hit.point);
        // 在射线碰撞的地方生成一个新的物体
        Object.Instantiate(box,hit.point,Quaternion.identity);
    }
}
```

#### 相关 API 解释

`camera.ScreenPointToRay` 从屏幕点指向世界坐标点；

`RaycastHit hit` 碰撞类型，hit 中存储碰撞到的对象实例

`Physics.Raycast` 投射射线；  
`ray` 为射线；  
`out hit` 表示获取射线射中的对象；

<br>

### API 进阶解释

#### RaycastHit

假设有如下代码：

```cs
Ray ray = camera.ScreenPointToRay(Input.mousePosition);
RaycastHit hit;
if (Physics.Raycast(ray, out hit))
{
}
```

hit.point 获取世界空间中射线命中碰撞体的撞击点；  
hit.collider.gameObject 获取碰撞体对象；  
hit.transform 命中的刚体或碰撞体的 Transform；  
hit.distance 从射线原点到撞击点的距离；

<br>

#### Physics.Raycast 三个重要构造函数

发射一条射线 ray，将碰撞对象存储在变量 hit 里面；  
`Physics.Raycast(ray, out hit)`

此时射线的最大距离为无限！（第三个参数表示射线距离）  
`Physics.Raycast(ray, out hit, Mathf.Infinity)`

参数分别为：射线起点、射线方向、碰撞体信息、射线距离  
`Physics.Raycast(transform.position, -Vector3.up, out hit, Mathf.Infinity)`

> 注意：一般来说 Physics.Raycast 还包括最后两个参数，他们分别是层遮罩和碰撞器，作用不大，这里不做过多解释  
> 如有需要请前往官网查看：[Physics.Raycast 官方 API](https://docs.unity3d.com/cn/current/ScriptReference/Physics.Raycast.html)

<br>

### Debug 画线指南
