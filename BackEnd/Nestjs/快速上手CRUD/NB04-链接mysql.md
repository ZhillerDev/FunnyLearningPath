### 配置链接

> 首先需要安装 mysql2 以及 typeorm 框架  
> `npm i mysql2 @nestjs/typeorm`

<br>

#### 内联链接法

直接在 app.module.ts 文件内配置

首先引入 TypeOrmModule，并在 imports 内部使用 TypeOrmModule.forRoot 注册 mysql 数据库即可

```ts
...

import { TypeOrmModule } from "@nestjs/typeorm";

@Module({
  imports: [
    TypeOrmModule.forRoot({
      type: "mysql",
      host: "localhost",
      port: 3306,
      username: "root",
      password: "123456",
      database: "demonest",
      entities: [],
      synchronize: true,
      retryDelay: 10000,
    })
  ],
  ...
})
export class AppModule {}
```

<br>

#### 配置文件法

> 目前我没有成功过，估计是路径的问题？

新建文件 ormconfig.json ，写上所有配置

```json
{
  "type": "mysql",
  "host": "localhost",
  "port": 3306,
  "username": "root",
  "password": "123456",
  "database": "demonest",
  "entities": [],
  "synchronize": true,
  "retryDelay": 10000
}
```

之后在 app.module.ts 里面直接使函数 forRoot 置空即可！会自动调用配置文件

```ts
@Module({
  imports: [
    TypeOrmModule.forRoot()
  ],
  ...
})
```

<br>

### 简单 Get

#### 数据库准备

这是我的数据库信息：

1. 数据库名：demonest
2. 数据库表（只有这一个表）：user
3. 表内属性：id username password
4. 有且仅有一行数据：1 admin 123456

<br>

#### 编写实体类

快速创建一个模块（nestjs 项目根目录下开命令行执行）：`nest g resource user`

创建完毕，删去没用的实验方法，删去两个没用的 dto

按照数据库对应编写实体类：`user.entity.ts`

> BaseEntity 基类，需要继承

```ts
import { BaseEntity, Entity, Column, PrimaryGeneratedColumn } from "typeorm";

// 装饰器表明这是一个实体
@Entity()
// 注意继承BaseEntity
export class User extends BaseEntity {
  // PrimaryGeneratedColumn设置主键
  // type 属性类型
  // name 属性在数据库内的名字
  // comment 一些注解
  @PrimaryGeneratedColumn({
    type: "int",
    name: "id",
    comment: "主键id索引",
  })
  id: number;

  // 剩下的属性以此类推即可
  @Column({ type: "varchar", name: "username" })
  username: String;

  @Column({ type: "varchar", name: "password" })
  password: String;
}
```

<br>

#### 编写 DTO

dto 文件夹内新建一个文件 `user.dto.ts`

dto 是为了方便处理数据的，这里为了便于演示，仅使用一条数据，即实现需求：通过用户名查询用户所有信息

```ts
export class GetSingleUser {
  username: String;
}
```

<br>

#### 存储库注册

每个模块都有自己的一个存储库，需要到对应的 module 文件内进行注册，才可以使用 typeorm 对应功能  
`user.module.ts`

```ts
...

import { User } from "./entities/user.entity";
import { TypeOrmModule } from "@nestjs/typeorm";

@Module({
  // 根据实体类注册内部存储库
  imports: [TypeOrmModule.forFeature([User])],
  ...
})
export class UserModule {}
```

<br>

#### 编写 controller

我们编写两个查询方法，返回值均使用 Promise 类型

具体逻辑放置在 provider 层（即 service）

`@Controller("/user")` 和 `@Get("users")` 都是定义请求 url 的，具体请求格式请往下看

```ts
import {
  Controller,
  Get,
  Post,
  Body,
  Patch,
  Param,
  Delete,
  Query,
} from "@nestjs/common";
import { UserService } from "./user.service";
import { User } from "./entities/user.entity";

@Controller("/user")
export class UserController {
  constructor(private readonly userService: UserService) {}

  @Get("users")
  findAll(): Promise<User[]> {
    return this.userService.findAll();
  }

  @Get("getuser")
  findOne(@Query() query): Promise<User> {
    return this.userService.findOne(query);
  }
}
```

请求这两个方法的格式：  
`/user/users`  
`/user/getuser?username=xxx`

<br>

#### 编写 service

首先在 service 的构造方法 constructor 里面注册 repository！！！

最好使用 `async` 异步请求

```ts
import { Injectable } from "@nestjs/common";
import { Repository } from "typeorm";
import { InjectRepository } from "@nestjs/typeorm";
import { User } from "./entities/user.entity";
import { GetSingleUser } from "./dto/user.dto";

@Injectable()
export class UserService {
  // 注册对应repo，即我们之前提到的存储库
  // 格式就这样，照抄即可
  constructor(
    @InjectRepository(User)
    private userRepo: Repository<User>
  ) {}

  // query可以执行一条sql查询语句
  // 查询结果以promise格式返回，因为这里取出多用户，泛型使用数组
  async findAll(): Promise<User[]> {
    return await this.userRepo.query("select * from user");
  }

  // 查找单个用户
  // 这里用到了user.dto.ts里面的GetSingleUser，获取信息格式
  async findOne(info: GetSingleUser): Promise<User> {
    return await this.userRepo.query(`select *
                    from user
                    where username = '${info.username}'`);
  }
}
```

<br>

#### 编译测试

回到 nestjs 项目根目录下，执行  
`npm run start:dev`

<br>

### 简易 POST

<br>
