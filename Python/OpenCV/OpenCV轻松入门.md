### 图像处理基础

#### 使用 numpy.array 访问像素

`item` 取出对应行列的像素值  
`itemset` 将对应索引的像素值进行替换

随机值生成 `256x256` 灰度图

```py
import numpy as np
import cv2

# 创建一个随机的256x256图像，值在0~255范围内（即uint8类型），存储在img变量中
img = np.random.randint(0, 256, size=[256, 256], dtype=np.uint8)

# 在一个名为“demo”的窗口中显示图像
cv2.imshow("demo", img)

# 等待用户按下任意键
cv2.waitKey()

# 关闭所有OpenCV窗口
cv2.destroyAllWindows()
```

<br>

生成对应彩色图像

```py
import cv2
import numpy as np

img = np.random.randint(0, 256, size=[256, 256, 3], dtype=np.uint8)
cv2.imshow("demo", img)
cv2.waitKey()
cv2.destroyAllWindows()
```

<br>

#### 感兴趣区域（ROI）

我们可以提取对应区域范围的图像，供细节处理，这块区域就被称为我们的感兴趣区域 ROI

```py
import cv2                              # 导入OpenCV库

a = cv2.imread("b1-ch.png", cv2.IMREAD_UNCHANGED)   # 从文件加载图像b1-ch.png，并存储在变量a中，以不变形式加载图像
face = a[220:400, 250:350]              # 从原始图像a中提取一个面部区域，该区域的坐标从第220行第250列开始，到第400行第350列结束
cv2.imshow("original", a)              # 显示原始图像a在名为“original”的窗口中
cv2.imshow("face", face)               # 显示提取的面部区域在名为“face”的窗口中
cv2.waitKey()                          # 等待用户按下任意键
cv2.destroyAllWindows()                 # 关闭所有OpenCV窗口
```

> 使用此方法可以借助上一节学到的灰度图，对指定区域进行打码操作

<br>

#### 通道操作

<br>

#### 获取图像属性

```py
import cv2
gray=cv2.imread("lena.bmp",0)
color=cv2.imread("lenacolor.png")
print("图像gray属性：")
print("gray.shape=", gray.shape)
print("gray.size=", gray.size)
print("gray.dtype=", gray.dtype)
print("图像color属性：")
print("color.shape=", color.shape)
print("color.size=", color.size)
print("color.dtype=", color.dtype)
```

<br>

### 图像运算
