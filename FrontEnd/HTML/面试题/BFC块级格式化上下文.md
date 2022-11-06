---
title: BFC块级格式化上下文
date: 2022-08-12 09:12:37
tags:

categories:
  - 前端面试八股
---

#### 如何理解 BFC

块格式化上下文（Block Formatting Context，BFC）针对的是 CSS 渲染过程中的块级盒子

BFC 具有以下特性：  
阻止相邻元素的 margin 重叠现象；  
浮动的元素也参与 BFC 的高度运算；  
BFC 会在垂直方向上逐一放置；

<br>

#### 如何触发

1. 根元素（HTML 标签）
2. float
3. overflow != visible
4. display = inline-block、table-cell、table-caption、table、inline-table、flex、inline-flex、grid、inline-grid
5. postion = fixed / absolute
