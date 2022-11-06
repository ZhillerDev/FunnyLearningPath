### 基础 module

#### module 特性

1. module 类似于一个核心，所有的 controller、service、entity 或者其他 bean 都需要在这里执行注入
2. 一个 module 中还可以注入其他的 module 以相互使用
3. module 中可注入任意类型数据（只要你愿意的话）

<br>

#### 从主 module 入手

我们先来看看默认生成的 `app.module.ts` 的结构

```ts
import { Module } from "@nestjs/common";
import { AppController } from "./app.controller";
import { AppService } from "./app.service";
import { UserModule } from "./user/user.module";

@Module({
  // 注入主controller
  controllers: [AppController],
  // 注入主service（在nestjs中service被叫做providers）
  providers: [AppService],
  // 注入外部的module
  imports: [UserModule],
})
export class AppModule {}
```

这里呈现了三个主要的参数（剩下的后续介绍）

1. controllers 顾名思义，专门注入 controller
2. providers 同理，专门注入 service
3. imports 导入外部的 module 或者其他数据

<br>

#### 拆分语法糖

现在来到我们刚刚创建的资源 user 文件夹内，找到 `user.module.ts`

可见，他和我们的 app.module 一样质朴，仅有简单的两个属性，分别注入 `controller` 和 `service`

代码中的 `[UserController]` 实际上是一种语法糖写法

```ts
import { Module } from "@nestjs/common";
import { UserService } from "./user.service";
import { UserController } from "./user.controller";

@Module({
  controllers: [UserController],
  providers: [UserService],
})
export class UserModule {}
```

<br>

我们可以使用对象的写法，为每个 bean 添加别名，别名使用 `provide` 属性定义，而 `useClass` 即我们需要注入的类

```ts
@Module({
  controllers: [UserController],
  providers: [
    {
      provide: 'us',
      useClass: UserService,
    },
  ],
})
```

此时我们来到 `user.controller.ts`

因为我们为 bean 设置了别名，故需要使用 `@Inject` 注解，明确支出需要使用哪一个 bean！！！

> ps:默认情况下自动生成的代码是不带 inject 注解的！这里要注意！

```ts
@Controller()
export class UserController {
  constructor(
    @Inject("us") private readonly userService: UserService
  ) {}
    ...
}
```

<br>

#### useValue

此属性可以注入一段数据

```ts
@Module({
  controllers: [UserController],
  providers: [
    ...
    {
      provide: 'list',
      // 注入了一个数值数组
      useValue: [1, 2, 3],
    },
  ],
})
```

<br>

然后依葫芦画瓢，在 controller 使用 inject 注解引入即可使用！

```ts
@Controller()
export class UserController {
  constructor(
    ...
    // 引入我们注入的数值！
    @Inject('test') private list: Number[],
  ) {}
}
```

<br>

#### useFactory

工厂方法，这个用途比较广泛，具体使用方法请查询文档

这里介绍一个注入 promise 的方法

```ts
@Module({
  controllers: [UserController],
  providers: [
    {
      provide: 'customPromise',
      async useFactory() {
        return await new Promise((val) => {
          setTimeout(() => {
            val('hellowprld');
          }, 1000);
        });
      },
    },
  ],
})
```

<br>

#### 模块暴露

在 module 文件内使用 exports 属性，可以将指定类暴露出去给其他模块使用

如下，我们把 UserService 给暴露到外界了！

```ts
@Module({
    ...
  exports: [UserService],
})
```

<br>

#### 全局暴露

直接给 module 添加 `@Global()` 注解即可！

<br>

### 进阶用法
