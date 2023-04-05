### 经典常用配置

#### 大小类

- w-{size}：设置元素宽度，例如 w-1/2 表示元素宽度为父容器宽度的一半。
- h-{size}：设置元素高度，例如 h-16 表示元素高度为 16 像素。
- max-w-{size}：设置元素最大宽度，例如 max-w-md 表示元素最大宽度为中等屏幕大小。
- max-h-{size}：设置元素最大高度，例如 max-h-screen 表示元素最大高度为屏幕高度。
- min-w-{size}：设置元素最小宽度，例如 min-w-0 表示元素最小宽度为 0。
- min-h-{size}：设置元素最小高度，例如 min-h-full 表示元素最小高度为 100%。

<br>

#### 文本类

- text-{color}：设置文本颜色，例如 text-red-500 表示文本颜色为红色 500。
- text-{size}：设置文本大小，例如 text-sm 表示文本大小为小号字体。
- font-{family}：设置字体系列，例如 font-sans 表示使用无衬线字体。
- font-{weight}：设置字体粗细，例如 font-bold 表示使用粗体字体。
- leading-{size}：设置行间距，例如 leading-6 表示行间距为 6。

<br>

#### 背景类

- bg-{color}：设置背景颜色，例如 bg-gray-300 表示使用灰色背景。
- bg-{image}：设置背景图片，例如 bg-cover 表示使用覆盖整个元素的背景图片。
- bg-{position}：设置背景位置，例如 bg-center 表示将背景图像居中对齐。
- bg-{size}：设置背景尺寸，例如 bg-auto 表示使用原始背景图像大小。

<br>

#### 边框和轮廓类

- border-{color}：设置边框颜色，例如 border-red-500 表示使用红色边框。
- border-{size}：设置边框大小，例如 border-2 表示边框宽度为 2 像素。
- border-{side}：设置边框位置，例如 border-l 表示只在元素左侧添加边框。
- rounded-{size}：设置圆角大小，例如 rounded-full 表示使用完全圆角。

<br>

### Typography

#### 标准文本处理

三大字体家族  
`font-sans font-serif font-mono`

<br>

意大利体  
`italic`  
`not-italic`

<br>

字体磅值  
`font-thin`  
`font-light`  
`font-normal`  
`font-bold`  
`font-extrabold`  
`font-black`

<br>

字符变体（不同的变体呈现不同的字符状态）  
`ordinal`  
`slashed-zero`  
`oldstyle-nums`

<br>

letter-spacing  
`tracking-tight`  
`tracking-normal`  
`tracking-wide`

<br>

强制字符串分成几行来显示  
`line-clamp-3`  
`line-clamp-none`

<br>

line-height  
`leading-4`

<br>

#### 列表处理

li 标签前面显示的序号样式（数字、黑点、无样式）  
`list-decimal`  
`list-disc`  
`list-none`

<br>

#### 文本

文本位置  
`text-center`

文本颜色（斜杠后的数字表示透明度，透明度 0-100）  
`text-transparent`  
`text-inherit`  
`text-slate-50`  
`text-blue-600/25`

文本装饰  
`underline`  
`overline`  
`line-through`  
文本装饰对应的颜色  
`decoration-blue`

文本装饰样式  
`decoration-solid`  
`decoration-double`  
`decoration-dotted`  
`decoration-dashed`  
`decoration-wavy`

<br>

text overflow  
`truncate`  
`text-ellipsis`  
`text-clip`

text-indent  
`indent-0`

vertical-align  
`align-middle`

<br>

### 背景 Background

三大 attachment  
`bg-fixed`  
`bg-local`  
`bg-scroll`

<br>

clip（使用 clip-text 打造渐变文本！）  
`bg-clip-content`  
`bg-clip-text`

position（背景图聚焦位置）  
`bg-bottom`  
`bg-top`

<br>

background-repeat  
`bg-repeat`  
`bg-no-repeat`  
`bg-repeat-x`  
`bg-repeat-y`  
`bg-repeat-round`  
`bg-repeat-space`

background-size  
`bg-auto`  
`bg-cover`  
`bg-contain`

<br>

背景渐变色  
如下代码表示：渐变色从左到右(to-r)，左颜色为 cyan-500，右颜色为 blue-500  
`<div class="h-14 bg-gradient-to-r from-cyan-500 to-blue-500"></div>`

<br>

### Border

ring 一种浅色投影效果  
`ring-0`  
`ring-white`  
`ring-offset-0`

<br>

### Effects

box-shadow  
`shadow-sm`  
`shadow-md`  
`shadow-lg`  
`shadow-xl`  
`shadow-none`

透明度  
`opacity-0`

<br>

### 过滤器 Filters

模糊 `blur-0`

快速阴影 `drop-shadow-xl`

变成黑白 `grayscale`

反色 `invert`

饱和度 `saturate-50`

背景模糊（常用！） `backdrop-blur-sm`

<br>

### 过渡与动画 Transition

全部过渡 `transition-all`

过渡时长（单位 ms） `duration-1000`

过渡曲线 `ease-in-out`

延后过渡（单位 ms） `delay-1000`

<br>

几个预制好的动画  
`animate-spin` 加载环动画  
`animate-ping` 新消息圆点跳动  
`animate-pulse` 骨架屏加载脉冲  
`animate-bounce` 弹跳

<br>

### 大小设置 sizing

宽度 `w-0`  
按比例划分宽度 `w-1/2`

最小宽度  
`min-w-min`  
`min-w-max`  
`min-w-full`

最大宽度  
`max-w-sm`  
`max-w-lg`

> 高度设置只需要把 w 改成 h 即可，剩余属性设置一致

<br>

### 间距 spacing

padding  
`p-0`  
`px-0` x 表示左右  
`py-0` y 表示上下  
`px-px` 表示左右间隔 1px

margin  
`-mt-8` 距顶-8rem

专用于设置 space-between 间距的样式  
`space-x-0`  
`space-y-0`

<br>

### 弹性布局 Flexing&Grid

Flex 常用设置  
`flex`  
`flex-col`  
`flex-row`  
`flex-wrap flex-nowrap`  
`justify-center`  
`items-center`  
`grow-0`  
`shrink-0`

<br>

Grid 常用设置  
`grid-cols-4`

<br>

其他  
`gap-x-px` 左右间距 1px  
`content-center` 设置 align-center 居中

<br>

### 布局 layout

容器 `container`  
列布局 `colums-2`

怪异盒模型 `box-border`  
标准盒模型 `box-content`

剩下的就是一些 display float clear overflow position 这些，都是直接原单词拿来就用，这里不做介绍

<br>
