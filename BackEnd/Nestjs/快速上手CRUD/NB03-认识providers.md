### provider 基础

所谓的 provider，其实可以等同于 springboot 里面的 service

service 协助处理 controller 层的事务，下面是一个简单的 service

```ts
import { Injectable } from "@nestjs/common";
import { CreateUserDto } from "./dto/create-user.dto";

@Injectable()
export class UserService {
  create(createUserDto: CreateUserDto) {
    return "This action adds a new user";
  }

  findAll() {
    return `This action returns all user`;
  }

  findOne(id: number) {
    return `This action returns a #${id} user`;
  }
}
```
