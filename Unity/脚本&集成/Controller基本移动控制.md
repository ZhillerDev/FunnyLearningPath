### 移动控制最优解

为被控物体添加刚体组件，并冻结 y 轴移动以及 xyz 轴旋转；  
将以下代码挂到该物体上；

代码主要思路：使用 getaxis 方法获取水平垂直方向的向量，并通过刚体的 MovePosition 方法实现物体的移动；

> 因为 getaxis 的值是逐渐增加的，故整个移动动作具有一定缓冲，不会显得僵硬；

```cs
private Rigidbody rb;
private void Start()
{
    rb = gameObject.GetComponent<Rigidbody>();
}

void Update()
{
    move();
}

void move()
{
    float h = Input.GetAxis("Horizontal");
    float v = Input.GetAxis("Vertical");
    rb.MovePosition(transform.position + new Vector3(h, 0, v) * speed * Time.deltaTime);
}
```

<br>
