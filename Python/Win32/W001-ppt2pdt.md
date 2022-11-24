### 基本款

使用 `win32com` 库，后台运行 ppt 并调用其内部方法转换 `ppt|pptx` 为 pdf

遍历程序运行所在文件夹内的所有文件，只要遇到 `ppt|pptx` 后缀的文件就执行转换方法！

注意绝对路径以及文件名、后缀名的获取方法，别搞错了

```py
import os

import win32com.client as wc


def ppt2pdf(input_path, output_path):
    # 挂载app
    ppt_application = wc.Dispatch("PowerPoint.Application")
    # 使用app打开指定绝对路径下文件
    ppt_file = ppt_application.Presentations.Open(input_path)
    # 32表示保存文pdf格式
    ppt_file.SaveAs(output_path, 32)
    print("导出pdf成功")
    # 用完记得退出
    ppt_application.Quit()


# 获取当前目录下所有文件
file_names = os.listdir(os.getcwd())

if __name__ == "__main__":
    for file in file_names:
        # 获取文件名以及后缀
        name, suffix = file.split(".")
        # 如果文件格式不对，直接跳过
        if suffix != "ppt" and suffix != "pptx":
            continue
        # 设置输入文件绝对路径以及输出文件绝对路径
        input_name = os.getcwd() + fr'/{name}.{suffix}'
        output_name = os.getcwd() + fr'/{name}.pdf'
        # 一一转换
        ppt2pdf(input_name, output_name)
```

<br>
