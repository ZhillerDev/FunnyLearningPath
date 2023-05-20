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

#### 加权和

> 所谓图像加权和，就是在计算两幅图像的像素值之和时，将每幅图像的权重考虑进来

加权和函数：`dst=cv2.addWeighted(src1, alpha, src2, beta, gamma)`  
alpha 和 beta 是 src1 和 src2 所对应的系数，它们的和可以等于 1，也可以不等于 1  
参数 gamma 的值可以是 0，但是该参数是必选参数，不能省略

<br>

示例代码

```py
import cv2
import numpy as np

# 创建两个大小为 3 x 4 且所有像素值都为 100 和 10 的图像
img1 = np.ones((3, 4), dtype=np.uint8) * 100
img2 = np.ones((3, 4), dtype=np.uint8) * 10

# 设置 gamma 值
gamma = 3

# 对两个图像进行加权叠加，并应用 gamma 调整
img3 = cv2.addWeighted(img1, 0.6, img2, 5, gamma)

# 打印结果
print(img3)
```

<br>

#### 掩模

当使用掩模参数时，操作只会在掩模值为非空的像素点上执行，并将其他像素点的值置为 0

掩模的演示

```py
# 导入cv2和numpy库
import cv2
import numpy as np

# 创建两个 4x4 的矩阵，并用元素值初始化
img1 = np.ones((4,4), dtype=np.uint8) * 3
img2 = np.ones((4,4), dtype=np.uint8) * 5

# 创建一个 4x4 的零矩阵作为掩膜
mask = np.zeros((4,4), dtype=np.uint8)

# 在掩膜中指定一部分区域（从第三行第三列开始到最后一行最后一列）为1
mask[2:4, 2:4] = 1

# 创建一个 4x4 的矩阵，并用元素值初始化
img3 = np.ones((4,4), dtype=np.uint8) * 66

# 输出初始化的 img1、img2、mask 和 img3 的值
print("img1=\n", img1)
print("img2=\n", img2)
print("mask=\n", mask)
print("初始值img3=\n", img3)

# 将 img1 和 img2 相加，并根据掩膜 mask 进行加权，将结果存储在 img3 中
img3 = cv2.add(img1, img2, mask=mask)

# 输出加权后的 img3 的值
print("求和后img3=\n", img3)
```

<br>

#### 图像加解密

通过对原始图像与密钥图像进行按位异或，可以实现加密；  
将加密后的图像与密钥图像再次进行按位异或，可以实现解密

```py
# 导入cv2和numpy库
import cv2
import numpy as np

# 读取灰度图像 "b1-ch.png" 并将其存储在变量 lena 中
lena = cv2.imread("b1-ch.png", 0)

# 获取 lena 图像的行数和列数
r, c = lena.shape

# 生成一个与 lena 图像大小相同的随机密钥，并将其存储在变量 key 中
key = np.random.randint(0, 256, size=[r, c], dtype=np.uint8)

# 使用cv2.bitwise_xor函数对 lena 和 key 进行异或运算，并将结果存储在变量 encryption 中
encryption = cv2.bitwise_xor(lena, key)

# 对加密后的图像 encryption 和 key 进行异或运算，并将结果存储在变量 decryption 中
decryption = cv2.bitwise_xor(encryption, key)

# 在窗口中显示原始的 lena 图像、密钥 key、加密图像 encryption 和解密图像 decryption
cv2.imshow("lena", lena)
cv2.imshow("key", key)
cv2.imshow("encryption", encryption)
cv2.imshow("decryption", decryption)

# 等待用户按下任意按键
cv2.waitKey()

# 关闭所有窗口
cv2.destroyAllWindows()
```

<br>

#### 数字水印

最低有效位（Least Significant Bit, LSB）指的是一个二进制数中的第 0 位（即最低位）。  
最低有效位信息隐藏：将一个需要隐藏的二值图像信息嵌入载体图像的最低有效位，从而隐藏图像

载体图像最低有效位嵌入图像后整体改变很微弱，基本无法察觉，接收者即可从该有效位提取秘密信息

这种隐藏图像的方式就是 `数字水印`

<br>
