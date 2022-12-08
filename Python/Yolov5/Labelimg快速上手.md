### 安装

使用清华镜像源，一句话搞定

```sh
pip install labelimg -i https://pypi.tuna.tsinghua.edu.cn/simple
```

<br>

### 入门

#### 标注类别

对图像进行打标时，需要针对不同的识别对象定义类别标签 label

在 `labelimg` 根目录下的 data 文件夹中找到 `predefined_classes.txt` 文件

以单行的形式填写你需要定义的类型，后续启动 labelimg 后，就会自动出现在类型预选下拉框里面了

譬如我们定义两个类型：  
并把它均保存到 predefined_classes.txt 内

```
sun
moon
```

<br>

#### 运行 labelimg

进入根目录并在此处打开命令提示符

运行： `python labelImg.py` 即可开启软件

<br>

#### 推荐设置

在打标前，请在菜单栏的 `“查看”` 中将最顶部的五个选项全部勾选，便于加快我们的工作效率

![](../img/yolo/labelimg/lb1.png)

<br>

#### 文件管理

一般的，建议新增一个文件夹来管理打标图片以及标注坐标文本文件

见以下参考目录结构：  
| demo 数据集名称  
|- images 图片存放  
|- annotations 标注坐标文本输出

<br>

#### 开始打标

右侧按钮选择 `“打开目录”` ，选择图片文件夹；  
`“改变存放目录”` ，选择打标结果 txt 文件存放文件夹；

更改存储格式为 `yolo`

之后就可以使用小键盘的上下箭头快速切换图片，并配合 W 键快速划区块进行打标；

> 打标前请确认当前类型是不是和你打标的对象是匹配的！

<br>

### 格式转换

> 一般的，我们从网站上下载的许多成型数据集各种格式的都有，故需要将这些不同格式的 labels 转换为 yolo 能识别的 txt 文件！

<br>

#### xml 转换 txt

我们仅需要修改的几个地方：

1. CLASSES 标注类别，请严格按照类别的顺序（0,1,2,3...）来填写！！！
2. xml_input 待转换的 xml 文件目录
3. out_file 输出文件夹（请保证输出路径内的所有文件夹都存在！这行代码不会自动为你创建新的文件夹的！）

> 配置完毕直接运行即可转换成对应的 label 文件了

```py
import os
import xml.etree.ElementTree as ET

# 修改处1： 对象类型
CLASSES = ["with_mask", "without_mask", "mask_worn_incorrectly"]

# 修改处2： xml文件目录
xml_input = r"F:\[2]others\Yolov\mask\annotations\\"


def convert(size, box):
    dw = 1. / size[0]
    dh = 1. / size[1]
    x = (box[0] + box[1]) / 2.0
    y = (box[2] + box[3]) / 2.0
    w = box[1] - box[0]
    h = box[3] - box[2]
    x = x * dw
    w = w * dw
    y = y * dh
    h = h * dh
    return (x, y, w, h)


def convert_annotation(image_id):
    in_file = open(xml_input + image_id)
    image_id = image_id.split(".")[0]

    # 修改处3： txt输出文件夹
    out_file = open(r"F:\[2]others\Yolov\mask\txts\%s.txt" % (image_id), "w")

    tree = ET.parse(in_file)
    root = tree.getroot()
    size = root.find("size")
    w = int(size.find("width").text)
    h = int(size.find("height").text)
    for obj in root.iter("object"):
        difficult = obj.find("difficult").text
        obj_cls = obj.find("name").text
        if obj_cls not in CLASSES or int(difficult) == 1:
            continue
        cls_id = CLASSES.index(obj_cls)
        xmlbox = obj.find("bndbox")
        points = (float(xmlbox.find("xmin").text),
                  float(xmlbox.find("xmax").text),
                  float(xmlbox.find("ymin").text),
                  float(xmlbox.find("ymax").text))
        bb = convert((w, h), points)
        out_file.write(str(cls_id) + " " + " ".join([str(a) for a in bb]) + "\n")


def make_label_txt():
    filenames = os.listdir(xml_input)
    for file in filenames:
        convert_annotation(file)


if __name__ == "__main__":
    make_label_txt()
```

<br>
