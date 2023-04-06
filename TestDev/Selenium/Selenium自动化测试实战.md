### Selenium WebDriver 基本使用

#### 安装与测试

> 使用 pycharm 作为开发 IDE

请自行配置好 python 环境变量，避免因为多解释器的问题导致 pip 安装位置错误

安装 selenium 框架：`pip install selenium`

<br>

除此之外我们还需要安装对应浏览器的驱动（chrome、firefox、ie、opera、edge 等）

假如我们使用火狐，则百度搜索 `selenium firefox driver`

找到对应的 github 项目，下载 win64 版本的 zip 压缩包，解压后双击运行对应的驱动文件即可安装

<br>

打开 pycharm，新建一个文件，测试我们的第一个自动化脚本

脚本基本实现了：打开百度、找到指定元素、发送文本、模拟点击、退出，这么一个完整的流程

```py
from selenium import webdriver
from selenium.webdriver.common.by import By

driver = webdriver.Chrome()
driver.get("https://www.baidu.com/")
driver.find_element(By.ID, "kw").click()
driver.find_element(By.ID, "kw").send_keys("hello world")
driver.find_element(By.ID, "su").click()
driver.quit()
```

<br>

#### 基本 API

获取浏览器信息

```py
from selenium import webdriver

driver = webdriver.Chrome()

print("获取位置对象：", driver.get_window_position())
print("获取位置坐标x值：", driver.get_window_position()["x"])
print("获取位置坐标y值：", +driver.get_window_position()["y"])

print("获取大小对象：", driver.get_window_size())
print("获取宽度值", driver.get_window_size()["width"])
print("获取高度值", driver.get_window_size()["height"])

print("获取位置及大小对象：", driver.get_window_rect())
print("获取位置坐标x值：", driver.get_window_rect()["x"])
print("获取位置坐标y值：", driver.get_window_rect()["y"])
print("获取宽度值：", driver.get_window_rect()["width"])
print("获取高度值：", driver.get_window_rect()["height"])
```

查找页面元素  
不建议使用 class 查找，因为 class 多有重名的情况，不唯一

```py
# 按id、name、class、tag查找
driver.find_element_by_id("id属性值")
driver.find_element_by_name("name属性值")
driver.find_element_by_class_name("class属性值")
driver.find_element_by_tag_name("HTML标签名称")

# 按链接文本查找，即页面中a标签中的文本
driver.find_element_by_link_text("链接的文本")
# 按链接文本模糊查找
driver.find_element_by_partial_link_text("链接的一部分文本")
```

<br>

#### XPath 查找

> 基本表达式：`driver.find_element_by_xpath("XPath表达式")`

索引或属性定位查找

```py
driver.find_element_by_xpath("//span/input[1]")  #选取第1个与表达式//span/input匹配的元素
driver.find_element_by_xpath("//span/input[last()]")  #选取最后1个与表达式匹配的元素

driver.find_element_by_xpath("//input[@id='kw']")
driver.find_element_by_xpath("//input[@name='wd']")
driver.find_element_by_xpath("//input[@class='s_ipt']")
```

函数或表达式定位

```py
driver.find_element_by_xpath("//a[text()='hao123']")  #查找文本为hao123的a元素
driver.find_element_by_xpath("//a[contains(@href,'www.hao123.com')]")  #查找href属性
#包含www.hao123.com的a元素
driver.find_element_by_xpath("//a[contains(text(),'ao12')]")#查找文本包含ao12的a元素
driver.find_element_by_xpath("//a[starts-with(@href,'https://www.hao')]")  #查找href
#属性以https://www.hao开始的a元素
```

<br>

#### CSS 选择器查找

```py
# 层级定位
driver.find_element_by_css_selector("span > input")

# 关键属性定位
driver.find_element_by_css_selector("#kw")  #符号"#"代表使用id匹配，即匹配id为kw的元素

# 属性模糊定位
driver.find_element_by_css_selector("[href^='https://www.hao']")  #查找href属性值以
#https://www.hao开头的元素
driver.find_element_by_css_selector("[href$='123.com']")  #查找href属性值以123.com结尾
#的元素

# 组合定位
driver.find_element_by_css_selector("span > input[class='s_ipt'][name='wd']")  #查找
# 任意span下Class名称为s_ipt，Name属性为wd的input元素
```

<br>

#### By 对象查找

by 实现动态查找的功能，他需要预先导入 selenium 中的对应包

```py
from selenium import webdriver
from selenium.webdriver.common.by import By # 记住导入这个包

driver = webdriver.Chrome()
driver.get("https://www.baidu.com")

# 根据类型自动查找
driver.find_element(By.ID, "kw")
driver.find_element(By.NAME, "wd")
driver.find_element(By.CLASS_NAME, "s_ipt")
driver.find_element(By.LINK_TEXT, "地图")
driver.find_element(By.PARTIAL_LINK_TEXT, "地")
driver.find_element(By.TAG_NAME, "input")
driver.find_element(By.XPATH, "//input[@id='kw']")
driver.find_element(By.CSS_SELECTOR, "#kw")
```

<br>

#### 页面元素操作

```py
# 单击
webElement.click()

# 向元素输入内容或上传附件
webElement.send_keys("要输入的内容")

# 清空元素内容
webElement.clear()

# 提交表单元素
webElement.submit()

# 下拉框操作
selectWebElement.select_by_visible_text("选项的文本")  #按文本选择
selectWebElement.select_by_value("选项的值")  #按选项值选择
selectWebElement.select_by_index(选项的索引)  #按选项索引选择，索引从0开始
```

<br>

#### 获取页面元素内容

```py
# 获取元素选中状态
webElement.is_selected()

# 获取元素可编辑状态
webElement.is_enabled()

# 元素是否显示
webElement.is_displayed()

# 获取HTML属性
webElement.get_attribute("属性名称")

# 获取DOM属性
webElement.get_property("属性名称")

# 获取CSS属性
webElement.value_of_css_property("CSS属性名称")

webElement.location #获取位置对象
webElement.size #获取大小对象
webElement.rect #获取位置及大小对象
```

<br>

#### 浏览器弹出框

首先通过以下代码获取 alert 对象后，才可以进行后续操作

```py
driver.switch_to.alert
```

<br>

对 Alert 对象的操作（通过上面获取到对象后）

```py
Alert.accept()  #单击"确认"按钮
Alert.dismiss()  #单击"取消"按钮

Alert.text # 获取文本

Alert.send_keys("要输入的内容")
```

<br>

### Selenium WebDriver 高级运用

#### selenium 等待机制

页面级等待机制：定义 Selenium 等待页面加载完毕的超时时间  
使用此函数设置等待时间：`driver.set_page_load_timeout(最长等待秒数)`

<br>

元素级等待：常用于现代页面中的 ajax 请求返回整个过程的等待时间

- 强制等待：直接 time.sleep(3)强制睡眠
- 隐式等待：使用该代码设置宽限时间：`webdriver.implicitly_wait(等待秒数)`
- 显式等待（最推荐使用）：`WebDriverWait(WebDriver实例, 超时秒数, 检测时间间隔[可选], 可忽略异常集合[可选] )`

<br>

脚本级等待机制：设置异步脚本超时时间  
`driver.set_script_timeout(最长等待秒数)`

<br>

#### 键鼠模拟

> 操作链中涉及的所有 WebElement 元素在操作链执行时必须同时存在，且处于可操作状态

ActionChains 操作链，使用流的形式依次处理模拟事件

```py
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains # 记住导包

driver = webdriver.Chrome()
driver.get("https://www.baidu.com")

# ActionChains接收一个webdriver对象作为参数
# 链式调用的形式模拟点击
ActionChains(driver).click(driver.find_element(By.LINK_TEXT, "登录")).perform()
```

<br>

复杂操作链模拟：拖放操作

```html
<html>
	<head>
		<style type="text/css">
			#div1 {
				width: 300px;
				height: 100px;
				padding: 10px;
				border: 1px solid #aaaaaa;
			}
		</style>
		<script src="https://cdn.bootcss.com/jquery/3.2.1/jquery.min.js"></script>
		<script src="http://apps.bdimg.com/libs/jqueryui/1.10.4/jquery-ui.min.js"></script>
		<script type="text/javascript">
			$(function () {
				$("#drag1").draggable();
				$("#div1").droppable({
					drop: function (event, ui) {
						alert("图片放置成功");
					},
				});
			});
		</script>
	</head>

	<body>
		<p>请把图片拖放到方框中：</p>
		<div id="div1"></div>
		<br />
		<img
			id="drag1"
			src="https://cdn.ptpress.cn/pubcloud/3/app/0718A6B0/cover/20191204BD
54009A.png"
		/>
	</body>
</html>
```

```py
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains

driver = webdriver.Chrome()
driver.get("D:\\TestDragAndDrop.html")

ActionChains(driver)\
    .click_and_hold(driver.find_element(By.ID, "drag1"))\
    .move_to_element(driver.find_element(By.ID, "div1"))\
    .release()\
    .perform()
```

<br>

组合键的实现，`actionchains` 精准控制组合键按下与弹起的动作

```py
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.common.keys import Keys

driver = webdriver.Chrome()
driver.get("https://www.baidu.com")

baiduSearchInput = driver.find_element(By.ID, "kw")
baiduSearchInput.send_keys("hello world")

ActionChains(driver)\
    .key_down(Keys.CONTROL)\
    .send_keys("a")\
    .key_up(Keys.CONTROL) \
    .pause(3)\
    .key_down(Keys.CONTROL) \
    .send_keys("x") \
    .key_up(Keys.CONTROL) \
    .pause(3) \
    .key_down(Keys.CONTROL) \
    .send_keys("v") \
    .key_up(Keys.CONTROL) \
    .pause(3) \
    .send_keys(Keys.BACKSPACE)\
    .pause(3)\
    .send_keys(Keys.ENTER)\
    .perform()
```

<br>

#### 操作 Cookie

获取 cookie 的两种方式：

```py
driver.get_cookies()  #获取所有的cookie对象集合
driver.get_cookie(cookie名称)   #根据名称获取单个cookie

driver.add_cookie(cookie对象) # 增加cookie
driver.delete_all_cookies()  #删除全部Cookie
driver.delete_cookie(cookie名称)  #按名称删除指定Cookie
```

<br>

#### 浏览器启动参数

浏览器实例时可以附加很多参数，这是所有参数的默认值

```py
driver = webdriver.Chrome(executable_path='chromedriver', port=0, options=None,
service_args=None, desired_capabilities=None, service_log_path=None, chrome_options=
None, keep_alive=True)
```

<br>
