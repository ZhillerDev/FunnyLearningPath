### 属性拷贝

书接上文，我们使用 DTO 扩展了原实体类 Dish 的属性

那么我们在做分页处理的时候首先选择 Dish 作为分页类型，但由于部分属性需要 DTO，所以可以采用属性复制的方法，将 Dish 分页的数据原封不动的拷贝到 DTO 分页数据

```java
@Override
public DishDto getByIdWithFlavor(Long id) {
    // 查询菜品基本信息
    Dish dish = this.getById(id);
    DishDto dishDto = new DishDto();
    // 这里直接把原始dish的内容拷贝到了disdto里面去
    BeanUtils.copyProperties(dish, dishDto);

    LambdaQueryWrapper<DishFlavor> queryWrapper = new LambdaQueryWrapper<>();
    queryWrapper.eq(DishFlavor::getDishId, dish.getId());
    List<DishFlavor> flavors = flavorService.list(queryWrapper);
    dishDto.setFlavors(flavors);

    return dishDto;
}
```

<br>
