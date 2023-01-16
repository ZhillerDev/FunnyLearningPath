### model

> 其实就是 service 层，但对于大多数小项目直接写在 controller 就行了，没必要增加复杂度

<br>

#### 简单工具类

我们可以直接把 model 层内容作为一个工具类使用

譬如我们写一个把 unix 时间转换为正常时间的简单工具方法

```go
package model

import "time"

func Unix2Time(timetamp int) string {
	t := time.Unix(int64(timetamp), 0)
	return t.Format("2006-01-02 15:04:05")
}
```

<br>

### 实用方法

#### 上传图片完美方法

上传图片的完整流程介绍

1. 首先通过 FormFile 获取文件
2. 取出文件后缀名，并且与欲过滤的后缀名进行比较
3. 如果比较成功，表示通过类型检测
4. 获取当前时间，并以此时间在 uploads 文件夹下创建同名目录
5. 获取当前时间戳，将图片命名为时间戳，保存到新建的目录下，大功告成

```go
func UserUpload(c *gin.Context) {
	pic, err := c.FormFile("pic")
	if err == nil {
		// 获取文件后缀名
		extName := path.Ext(pic.Filename)
		allow := map[string]bool{
			".jpg":  true,
			".jpeg": true,
			".gif":  true,
			".png":  true,
		}

		// 对文件后缀进行比较，如果符合map中的任意一项，则验证成功，否则验证失败
		if _, ok := allow[extName]; !ok {
			c.String(http.StatusBadRequest, "请求失败")
			return
		}

		// 获取当前日期
		day := time.Now().Format("20060102")
		// 获取欲创建的目录
		savedir := "./src/uploads/" + day

		// 创建目录，0666表示所有组合
		err := os.MkdirAll(savedir, 0666)
		// 创建目录失败后要做的事
		if err != nil {
			c.String(http.StatusBadRequest, "创建目录失败")
			return
		}

		// 获取当前时间戳，作为图片文件名
		// 时间戳加上后缀名构成一张独特的图片文件
		filename := strconv.FormatInt(time.Now().Unix(), 10) + extName

		// 把保存路径和文件名称直接联合起来
		dst := path.Join(savedir, filename)
		// 保存图片到指定文件夹下
		c.SaveUploadedFile(pic, dst)
	}
}
```

<br>

#### 
