### Invoke 唤醒

延迟指定秒数后唤醒指定方法（仅执行一次）  
`Invoke("demo",4.0f);`

重复唤醒指定方法；  
参数二指首次唤醒延迟秒数，参数三表示后续每次唤醒延迟秒数；  
`InvokeRepeating("demo",1.0f,0.5f);`

取消唤醒指定方法  
`CancelInvoke("demo");`  
如果不指定方法名称，那就取消唤醒所有方法  
`CancelInvoke();`

<br>

> 😴😴😴 注意！ 所有的 invoke 方法接收的延迟时长单位均为秒，且类型为 float，别忘了在末尾加上 f

<br>

### InvokeRepeating 细节

因为这是一个重复唤醒的方法，所以写在 start 方法里面 `而非 update`！！！

被重复唤醒的方法中使用了 return 并不是表示直接退出唤醒循环，而是表示开始下一唤醒循环！

```cs
public int a = 10;

void Start()
{
    InvokeRepeating("demo",1.0f,2.0f);
}

void demo()
{
    // 不代表直接退出InvokeRepeating
    if(a!=5) return;
}
```

<br>
