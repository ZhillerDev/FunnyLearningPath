### 文件上传时 form 表单要求

1. method="post" 使用 post 方式提交
2. enctype="multipart/form-data" 上传文件采用 form-data
3. type="file" input 类型为 file

<br>

### 显示图片

我们上传图片后在前端显示需要经过以下两个步骤：

1. 用户上传图片，浏览器发起 POST 请求，服务端获取文件并保存
2. 浏览器再次发送 GET 请求下载服务端中存储的图片，并显示在网页上

<br>

下面使用一个 controller 管理图片的上传以及下载，具体含义请看注释

```java
@Slf4j
@RestController
@RequestMapping("/common")
public class CommonController {
    @Value("${reggie.path}")
    private String basepath;

    // 文件上传
    @PostMapping("/upload")
    public R<String> upload(MultipartFile file) {

        log.info("{}", file);

        // 获取文件完整名
        String filename = file.getOriginalFilename();
        // 取出文件后缀
        String suffix = filename.substring(filename.lastIndexOf("."));

        // 使用UUID作为临时文件的名称，可以避免文件名重复的尴尬
        String newfile = UUID.randomUUID().toString() + suffix;

        File dir = new File(basepath);
        if (!dir.exists()) {
            dir.mkdirs();
        }

        try {
            file.transferTo(new File(basepath + newfile));
        } catch (IOException e) {
            e.printStackTrace();
        }

        return R.success(newfile);
    }

    // 文件下载
    @GetMapping("/download")
    public void download(String name, HttpServletResponse response) {
        // 使用输入流分段读取文件，并output输出
        // 最后别忘了关闭输入输出流
        try {
            FileInputStream inputStream = new FileInputStream(new File(basepath + name));

            ServletOutputStream outputStream = response.getOutputStream();

            int len = 0;
            byte[] bytes = new byte[1024];
            while ((len = inputStream.read(bytes)) != -1) {
                outputStream.write(bytes, 0, len);
                outputStream.flush();
            }

            outputStream.close();
            inputStream.close();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```

<br>

### DTO（data transfer object）数据传输对象

可以看做是对原实体类的一个扩展；

譬如原实体类仅有属性 abc，而前端传过来的对象额外附带了一个新的属性 d，那么此时就需要 DTO 对原实体类进行扩展，并使用此 DTO 接收前端传来的对象！

下方是针对 Dish 实体类的 DTO 扩充

```java
package com.zhiyiyi.reggie.dto;

import com.zhiyiyi.reggie.entity.Dish;
import com.zhiyiyi.reggie.entity.DishFlavor;
import lombok.Data;

import java.util.ArrayList;
import java.util.List;

@Data
public class DishDto extends Dish {

    private List<DishFlavor> flavors = new ArrayList<>();

    private String categoryName;

    private Integer copies;
}

```

<br>
