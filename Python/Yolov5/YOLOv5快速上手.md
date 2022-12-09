> 非原创  
> 参考文献：  
> Yolov5 训练自己的数据集（详细完整版） https://blog.csdn.net/qq_45945548/article/details/121701492

<br>

### 安装

主要就需要安装以下三个东西：

1. anaconda 数据分析集成环境
2. yolov5 框架
3. labelimg 打标软件（请查看同目录下对应文章学习）

<br>

以上三者安装过程在网络上已经有诸多同类型文章，这里不做过多赘述，仅就运行过程可能出现的部分问题做出解决分析

<br>

### 数据准备

接下来将简易实现口罩检测的完整训练与测试流程

数据集请从 `kaggle` 上下载，我是用的是其排名第一的，拥有 820+口罩图片的数据集

该数据集有三种类型：

1. with_mask 戴口罩了
2. without_mask 没戴口罩
3. mask_worn_incorrectly 口罩佩戴不标准

<br>

下载好的数据集默认带两个文件夹：  
`annotations` 标注坐标文件，xml 格式，需要转换为 txt 类型  
`images` 顾名思义，图片

为此我们在任意位置新建一个文件夹 mask，并将这俩文件夹全部丢进去

<br>

#### xml2txt

mask 文件夹下新增文件 `xml2txt.py`

copy 以下代码，请注意修改对应的绝对位置和相对位置，不对其中内容做详细解析

> 编写完后运行即可

```py

import os
import xml.etree.ElementTree as ET
from os import getcwd

sets = ['train', 'val', 'test']

# 我们的数据集有三个类别，故在此处填入
classes = ["with_mask", "without_mask", "mask_worn_incorrectly"]
abs_path = os.getcwd()
print(abs_path)


def convert(size, box):
    dw = 1. / (size[0])
    dh = 1. / (size[1])
    x = (box[0] + box[1]) / 2.0 - 1
    y = (box[2] + box[3]) / 2.0 - 1
    w = box[1] - box[0]
    h = box[3] - box[2]
    x = x * dw
    w = w * dw
    y = y * dh
    h = h * dh
    return x, y, w, h


def convert_annotation(image_id):
    in_file = open('F:/[2]others/Yolov/mask/Annotations/%s.xml' % (image_id), encoding='UTF-8')
    out_file = open('F:/[2]others/Yolov/mask/labels/%s.txt' % (image_id), 'w')
    tree = ET.parse(in_file)
    root = tree.getroot()
    size = root.find('size')
    w = int(size.find('width').text)
    h = int(size.find('height').text)
    for obj in root.iter('object'):
        difficult = obj.find('difficult').text
        # difficult = obj.find('Difficult').text
        cls = obj.find('name').text
        if cls not in classes or int(difficult) == 1:
            continue
        cls_id = classes.index(cls)
        xmlbox = obj.find('bndbox')
        b = (float(xmlbox.find('xmin').text), float(xmlbox.find('xmax').text), float(xmlbox.find('ymin').text),
             float(xmlbox.find('ymax').text))
        b1, b2, b3, b4 = b
        # 标注越界修正
        if b2 > w:
            b2 = w
        if b4 > h:
            b4 = h
        b = (b1, b2, b3, b4)
        bb = convert((w, h), b)
        out_file.write(str(cls_id) + " " + " ".join([str(a) for a in bb]) + '\n')


wd = getcwd()
for image_set in sets:
    if not os.path.exists('F:/[2]others/Yolov/mask/labels/'):
        os.makedirs('F:/[2]others/Yolov/mask/labels/')
    image_ids = open('F:/[2]others/Yolov/mask/ImageSets/Main/%s.txt' % (image_set)).read().strip().split()

    if not os.path.exists('F:/[2]others/Yolov/mask/dataSet_path/'):
        os.makedirs('F:/[2]others/Yolov/mask/dataSet_path/')

    list_file = open('dataSet_path/%s.txt' % (image_set), 'w')

    for image_id in image_ids:
        list_file.write('F:/[2]others/Yolov/mask/images/%s.png\n' % (image_id))
        convert_annotation(image_id)
    list_file.close()

```

<br>

#### 生成路径

新建文件 `split_train_val.py`

这是生成训练集合变量文件的代码，直接抄，内容不改动！

```py
# coding:utf-8

import argparse
import os
import random

parser = argparse.ArgumentParser()
# xml文件的地址，根据自己的数据进行修改 xml一般存放在Annotations下
parser.add_argument('--xml_path', default='Annotations', type=str, help='input xml label path')
# 数据集的划分，地址选择自己数据下的ImageSets/Main
parser.add_argument('--txt_path', default='ImageSets/Main', type=str, help='output txt label path')
opt = parser.parse_args()

trainval_percent = 1.0  # 训练集和验证集所占比例。 这里没有划分测试集
train_percent = 0.9  # 训练集所占比例，可自己进行调整
xmlfilepath = opt.xml_path
txtsavepath = opt.txt_path
total_xml = os.listdir(xmlfilepath)
if not os.path.exists(txtsavepath):
    os.makedirs(txtsavepath)

num = len(total_xml)
list_index = range(num)
tv = int(num * trainval_percent)
tr = int(tv * train_percent)
trainval = random.sample(list_index, tv)
train = random.sample(trainval, tr)

file_trainval = open(txtsavepath + '/trainval.txt', 'w')
file_test = open(txtsavepath + '/test.txt', 'w')
file_train = open(txtsavepath + '/train.txt', 'w')
file_val = open(txtsavepath + '/val.txt', 'w')

for i in list_index:
    name = total_xml[i][:-4] + '\n'
    if i in trainval:
        file_trainval.write(name)
        if i in train:
            file_train.write(name)
        else:
            file_val.write(name)
    else:
        file_test.write(name)

file_trainval.close()
file_train.close()
file_val.close()
file_test.close()

```

<br>

#### yaml

mask 文件夹下新建 `mask.yaml`

用其表示文件结构

```yaml
# split_train_val生成的两个文件的绝对位置
train: F:/[2]others/Yolov/mask/dataSet_path/train.txt
val: F:/[2]others/Yolov/mask/dataSet_path/val.txt

# 类型数目
nc: 3

# 类型具体名称，一定要按照顺序填写！！！
names: ["with_mask", "without_mask", "mask_worn_incorrectly"]
```

<br>

### 训练调参

将 mask 文件夹拷贝到 `yolov5 根目录` 下；

同时将权重文件 `yolov5s.pt` 也移动到 yolov5 根目录

<br>

#### 修改 anchors

打开文件 `models/yolov5s.yaml`

我们仅需要修改其中的 nc 为我们数据集的类型数目即可！  
修改后直接保存并关闭文件：

```yaml
# YOLOv5 🚀 by Ultralytics, GPL-3.0 license

# Parameters
nc: 3 # 遵照我们前面写的，一共3个类型

# 后面的这些玩意还不需要了解，统统保持默认不管
depth_multiple: 0.33 # model depth multiple
width_multiple: 0.50 # layer channel multiple
anchors:
  - [10, 13, 16, 30, 33, 23] # P3/8
  - [30, 61, 62, 45, 59, 119] # P4/16
  - [116, 90, 156, 198, 373, 326] # P5/32
```

<br>

#### 开始训练！

目前我们还没有安装 CUDA 和 cudnn，故没法使用 GPU 训练而只能用 CPU

所以性能一定要调的低一点才可以正常运行，否则溢出就报错

来到 yolov5 根目录下，在此处打开命令提示符（管理员模式下）  
输入以下指令

```
python train.py --weights yolov5s.pt  --cfg models/yolov5s.yaml  --data mask/mask.yaml --epoch 10 --batch-size 1 --img 640   --device cpu

```

<br>

参数分析（这些参数都可以在 train.py 里面直接设置）：

1. weights 权重文件
2. cfg 配置文件
3. data 数据源配置文件，依据此来读取数据！
4. epoch 训练次数（由于图片数量 800+，仅训练 10 次精度就足够了，数值大小与等的时间成正比）
5. batch-size 建议 CPU 渲染值控制在 1-2 之间，太大了直接报错
6. img 图像分辨率
7. device 选择训练使用的处理器

<br>

#### 训练结果分析

train.py 训练完毕之后，来到 `runs/train/对应的exp文件夹`

目录下的 weights 里面就是生成好的权重 pt 文件，我们拷贝过来就可直接用了

<br>

### 检测测试

随机从网上下载一张口罩的图片，放入 data/images 文件夹下

拷贝刚刚训练好的 pt 文件 best.pt（我这边的文件大小为 13mb）到 yolov5 根目录下

执行该代码，以完成检测流程：  
`python detect.py --weights best.pt --source data/images/mask.jpg`

<br>

我们可以人为修改参数以屏蔽检测概率过低的结果

打开 `detect.py` 文件

在下方代码对应位置找到 `conf-thres（可以理解为识别置信度）` 参数的设置，默认值改为 `0.60`

```py
def parse_opt():
    parser = argparse.ArgumentParser()
    parser.add_argument('--weights', nargs='+', type=str, default=ROOT / 'yolov5s.pt', help='model path(s)')
    parser.add_argument('--source', type=str, default=ROOT / 'data/images', help='file/dir/URL/glob, 0 for webcam')
    parser.add_argument('--data', type=str, default=ROOT / 'data/coco128.yaml', help='(optional) dataset.yaml path')
    parser.add_argument('--imgsz', '--img', '--img-size', nargs='+', type=int, default=[640], help='inference size h,w')

    # 置信度，default调节为0.60表示仅显示检测概率为0.60以上的结果！
    parser.add_argument('--conf-thres', type=float, default=0.60, help='confidence threshold')

    parser.add_argument('--iou-thres', type=float, default=0.45, help='NMS IoU threshold')
    parser.add_argument('--max-det', type=int, default=1000, help='maximum detections per image')
    parser.add_argument('--device', default='', help='cuda device, i.e. 0 or 0,1,2,3 or cpu')
    parser.add_argument('--view-img', action='store_true', help='show results')
    parser.add_argument('--save-txt', action='store_true', help='save results to *.txt')
    parser.add_argument('--save-conf', action='store_true', help='save confidences in --save-txt labels')
    parser.add_argument('--save-crop', action='store_true', help='save cropped prediction boxes')
    parser.add_argument('--nosave', action='store_true', help='do not save images/videos')
    parser.add_argument('--classes', nargs='+', type=int, help='filter by class: --classes 0, or --classes 0 2 3')
    parser.add_argument('--agnostic-nms', action='store_true', help='class-agnostic NMS')
    parser.add_argument('--augment', action='store_true', help='augmented inference')
    parser.add_argument('--visualize', action='store_true', help='visualize features')
    parser.add_argument('--update', action='store_true', help='update all models')
    parser.add_argument('--project', default=ROOT / 'runs/detect', help='save results to project/name')
    parser.add_argument('--name', default='exp', help='save results to project/name')
    parser.add_argument('--exist-ok', action='store_true', help='existing project/name ok, do not increment')
    parser.add_argument('--line-thickness', default=3, type=int, help='bounding box thickness (pixels)')
    parser.add_argument('--hide-labels', default=False, action='store_true', help='hide labels')
    parser.add_argument('--hide-conf', default=False, action='store_true', help='hide confidences')
    parser.add_argument('--half', action='store_true', help='use FP16 half-precision inference')
    parser.add_argument('--dnn', action='store_true', help='use OpenCV DNN for ONNX inference')
    opt = parser.parse_args()
    opt.imgsz *= 2 if len(opt.imgsz) == 1 else 1  # expand
    print_args(vars(opt))
    return opt
```

<br>

> END，静候检测结果吧 QWQ
