## 简介

为便于直接 cv，加快开发速度，减少学习成本，现提取 codelabs 案例中的重点实现部分代码，加以简要分析，有助于快速复制并直接实现对应效果

<br>

## HarmonyOS 第一课 保存应用数据

> [课程地址](https://developer.huawei.com/consumer/cn/training/course/slightMooc/C101667367018821971?ha_linker=eyJ0cyI6MTY5Njc0MjkwMjk0NSwiaWQiOiIzM2RhYmRhODgzYzA2NTU3NGIxOWMwNGU3NDE1NjVmMiJ9)

<br>

### GlobalContext

> 这里可以理解为 springboot 中在 application.yaml 中配置 mysql 连接

因为使用 preference 存储功能，必须提供一个唯一的上下文实例，每一个实例对应一个独立的 preference 存储区

`GlobalContext` 可以理解为一个全局的，供 preference 使用的上下文对象

下方代码采用惰性初始化的方式获取了一个单例对象

GlobalContext 对象拥有一个 map，我们的数据就都存储到该 map 里面，而 preference 就相当于帮助我们进行存储读取操作的桥梁

```ts
export class GlobalContext {
	private constructor() {}
	private static instance: GlobalContext;
	private _objects = new Map<string, Object>();

	// 初始化
	public static getContext(): GlobalContext {
		if (!GlobalContext.instance) {
			GlobalContext.instance = new GlobalContext();
		}
		return GlobalContext.instance;
	}

	// 获取对象中存储值的方法
	getObject(value: string): Object | undefined {
		return this._objects.get(value);
	}

	// 设置对象中值的方法
	setObject(key: string, objectClass: Object): void {
		this._objects.set(key, objectClass);
	}
}
```

<br>

### PreferenceUtil

此部分代码比较长，现分段讲述，并在文末给出完整代码

#### 数据库名与字段名

调用 Preference 是必须设置 PREFERENCES_NAME 来定义当前数据库的名称

此外还要定义我们需要执行存储的字段名称，因为案例是修改字体大小，所以这里就定义字段名称 KEY_APP_FONT_SIZE

```ts
const TAG = "[PreferencesUtil]";
const PREFERENCES_NAME = "myPreferences";
const KEY_APP_FONT_SIZE = "appFontSize";
```

<br>

#### 数据库操作

> 这里可以简单理解为后端的 service 层

代码含义全部在注释内部呈现

```ts
export class PreferencesUtil {
	// 创建preference数据库
	// 根据上面我们创建的GlobalContext来设置
	createFontPreferences(context: Context) {
		let fontPreferences: Function = () => {
			let preferences: Promise<dataPreferences.Preferences> =
				dataPreferences.getPreferences(context, PREFERENCES_NAME);
			return preferences;
		};

		// GlobalContext.getContext()获取单例对象
		// setObject将我们的preference数据库存到该对象内，后续直接通过getFontPreferences来调用数据库
		GlobalContext.getContext().setObject("getFontPreferences", fontPreferences);
	}

	// 保存默认字体大小
	saveDefaultFontSize(fontSize: number) {
		let getFontPreferences: Function = GlobalContext.getContext().getObject(
			"getFontPreferences"
		) as Function;
		getFontPreferences()
			.then((preferences: dataPreferences.Preferences) => {
				preferences
					.has(KEY_APP_FONT_SIZE)
					.then(async (isExist: boolean) => {
						Logger.info(TAG, "preferences has changeFontSize is " + isExist);
						if (!isExist) {
							await preferences.put(KEY_APP_FONT_SIZE, fontSize);
							preferences.flush();
						}
					})
					.catch((err: Error) => {
						Logger.error(TAG, "Has the value failed with err: " + err);
					});
			})
			.catch((err: Error) => {
				Logger.error(TAG, "Get the preferences failed, err: " + err);
			});
	}

	// 当字体大小改变后保存
	saveChangeFontSize(fontSize: number) {
		// 获取数据库的promise方法
		let getFontPreferences: Function = GlobalContext.getContext().getObject(
			"getFontPreferences"
		) as Function;
		// 使用promise风格依次处理
		getFontPreferences()
			.then(async (preferences: dataPreferences.Preferences) => {
				await preferences.put(KEY_APP_FONT_SIZE, fontSize);
				preferences.flush();
			})
			.catch((err: Error) => {
				Logger.error(TAG, "put the preferences failed, err: " + err);
			});
	}

	// 异步获取改变后的字体大小
	async getChangeFontSize() {
		let fontSize: number = 0;
		let getFontPreferences: Function = GlobalContext.getContext().getObject(
			"getFontPreferences"
		) as Function;
		fontSize = await (
			await getFontPreferences()
		).get(KEY_APP_FONT_SIZE, fontSize);
		return fontSize;
	}

	// 删除改变后的字体大小
	async deleteChangeFontSize() {
		let getFontPreferences: Function = GlobalContext.getContext().getObject(
			"getFontPreferences"
		) as Function;
		const preferences: dataPreferences.Preferences = await getFontPreferences();
		let deleteValue = preferences.delete(KEY_APP_FONT_SIZE);
		deleteValue
			.then(() => {
				Logger.info(TAG, "Succeeded in deleting the key appFontSize.");
			})
			.catch((err: Error) => {
				Logger.error(
					TAG,
					"Failed to delete the key appFontSize. Cause: " + err
				);
			});
	}
}

export default new PreferencesUtil();
```

<br>

### 实际调用

> 前面说过，定义 PreferencesUtil 可以理解为 service 层，那么这里就可以认为是 controller 层（这样比喻其实不算恰当，因为 ArkTS 是声明式 UI 语言，而目前主推的前后端分离是不会在 controller 里面定义页面渲染的）

比如，我们在主页面 onPageShow 钩子函数内，调用 PreferencesUtil 来获取我们的字体大小

```dart
@Entry
@Component
struct HomePage {
  @State changeFontSize: number = CommonConstants.SET_SIZE_NORMAL;

  onPageShow() {
    PreferencesUtil.getChangeFontSize().then((value) => {
      this.changeFontSize = value;
      Logger.info(TAG, 'Get the value of changeFontSize: ' + this.changeFontSize);
    });
  }
}
```

<br>

## HarmonyOS 第一课 初识 ArkTS

> 这一阶段将详细学习 app 开发中常见的 viewmodel 模型开发  
> [官网教程](https://developer.huawei.com/consumer/cn/training/course/slightMooc/C101667356568959645?ha_linker=eyJ0cyI6MTY5Njc0NTE1NjYzNiwiaWQiOiIzM2RhYmRhODgzYzA2NTU3NGIxOWMwNGU3NDE1NjVmMiJ9)

<br>

### ViewModel

![](./img/ohos-first/f1.png)

上面为官方示例代码 RankingDemo 的文件结构  
这是一个标准的 viewmodel 开发结构，编写小型项目时我们可以直接套用该结构

- `bean` 存放数据模型（相当于后端的实体类 entity）
- `constant` 存放全局常量，如字体大小等
- `model` 模型，设置实体类对应的数据
- `view` 存放单个组件（类似于 vue 中的 component）
- `viewmodel` 视图模型

<br>

#### 数据类（实体类）

先处理实体类

定义一个清单实体，包含三个字段，因为我们后续需要对其进行实例化，所以记得添加构造函数 constructor

代码清单：`/common/bean/RankData.ets`

```ts
export class RankData {
	name: Resource;
	vote: string; // Number of votes
	id: string;

	constructor(id: string, name: Resource, vote: string) {
		this.id = id;
		this.name = name;
		this.vote = vote;
	}
}
```

<br>

#### model

接下来新建模型类，实例化我们的实体类，为其赋予内容

下方代码我们创建了两组数据，rankData1 以及 rankData2

代码清单：`/model/DataModel.ets`

```ts
import { RankData } from "../common/bean/RankData";

export { rankData1, rankData2 };

const rankData1: RankData[] = [
	new RankData("1", $r("app.string.fruit_apple"), "12080"),
	new RankData("2", $r("app.string.fruit_grapes"), "10320"),
	new RankData("3", $r("app.string.fruit_watermelon"), "9801"),
	new RankData("4", $r("app.string.fruit_banana"), "8431"),
	new RankData("5", $r("app.string.fruit_pineapple"), "7546"),
	new RankData("6", $r("app.string.fruit_durian"), "7431"),
	new RankData("7", $r("app.string.fruit_red_grape"), "7187"),
	new RankData("8", $r("app.string.fruit_pears"), "7003"),
	new RankData("9", $r("app.string.fruit_carambola"), "6794"),
	new RankData("10", $r("app.string.fruit_guava"), "6721"),
];

const rankData2: RankData[] = [
	new RankData("11", $r("app.string.fruit_watermelon"), "8836"),
	new RankData("12", $r("app.string.fruit_apple"), "8521"),
	new RankData("13", $r("app.string.fruit_banana"), "8431"),
	new RankData("14", $r("app.string.fruit_grapes"), "7909"),
	new RankData("15", $r("app.string.fruit_red_grape"), "7547"),
	new RankData("16", $r("app.string.fruit_pears"), "7433"),
	new RankData("17", $r("app.string.fruit_pineapple"), "7186"),
	new RankData("18", $r("app.string.fruit_durian"), "7023"),
	new RankData("19", $r("app.string.fruit_guava"), "6794"),
	new RankData("20", $r("app.string.fruit_carambola"), "6721"),
];
```

<br>

#### viewmodel

最后，使用视图模型来管理模型类，提供给 view 获取对应数据的能力

代码清单：`/viewmodel/RankDataViewModel.ets`

```ts
import { RankData } from "../common/bean/RankData";
import { rankData1, rankData2 } from "../model/DataModel";

export class RankViewModel {
	// 加载模型类定义的第一个数据集rankData1
	loadRankDataSource1(): RankData[] {
		return rankData1;
	}

	// 返回第二个数据集
	loadRankDataSource2(): RankData[] {
		return rankData2;
	}
}
```

<br>

### 主页面代码

#### 调用 viewmodel 数据

现在我们已经定义好了 viewmodel，那么来看看主页是怎么写的  
对应代码位于：`/pages/RankPage.ets`

第一行，导入我们的视图模型，定义为全局变量

```ts
let rankModel: RankViewModel = new RankViewModel();
```

然后在入口组件内顶部，定义四个属性

```ts
// 两个数据源对应的变量
// @State实现动态绑定
@State dataSource1: RankData[] = [];
@State dataSource2: RankData[] = [];

// 是否切换到另一个数据源的开关
@State isSwitchDataSource: boolean = true;
// 点击返回键后计时的时间
private clickBackTimeRecord: number = 0;
```

<br>

在页面准备显示的钩子函数内，获取数据源

```ts
aboutToAppear() {
    this.dataSource1 = rankModel.loadRankDataSource1();
    this.dataSource2 = rankModel.loadRankDataSource2();
}
```

<br>

#### 双击两次返回键退出功能实现

下面的代码实现了这个功能：  
用户点击一次返回键，弹出气泡，告诉用户需要再点击一次返回键才可以退出程序  
开启 interval 定时器，当两次点击间隔到达一定时间后，才允许用户退出（避免用户疯狂点击返回键，气泡还没显示出来就直接退出 app 了）

```ts
onBackPress() {
    if (this.isShowToast()) {
      prompt.showToast({
        message: $r('app.string.prompt_text'),
        duration: TIME
      });
      this.clickBackTimeRecord = new Date().getTime();
      return true;
    }
    return false;
  }

isShowToast(): boolean {
    return new Date().getTime() - this.clickBackTimeRecord > APP_EXIT_INTERVAL;
}
```

<br>

## HarmonyOS 第一课 属性动画

> [官方教程](https://developer.huawei.com/consumer/cn/training/course/slightMooc/C101667368091091005?ha_linker=eyJ0cyI6MTY5Njc0NTI1MjkyOCwiaWQiOiIzM2RhYmRhODgzYzA2NTU3NGIxOWMwNGU3NDE1NjVmMiJ9)

<br>

### Want

> 官方的解释：Want 是对象间信息传递的载体，可以用于应用组件间的信息传递。其使用场景之一是作为 startAbility()的参数，包含了指定的启动目标以及启动时需携带的相关数据；当 UIAbilityA 启动 UIAbilityB 并需要传入一些数据给 UIAbilityB 时，Want 可以作为一个载体将数据传给 UIAbilityB

我们需要在 ability 入口文件内，将其 want 参数传递给 globalcontext

与此同时通过 getDefaultDisplaySync 初始化，用于后期自定义动画

```ts
export default class EntryAbility extends Ability {
	onCreate(want: Want) {
		GlobalContext.getContext().setObject("abilityWant", want);
	}

	async onWindowStageCreate(windowStage: window.WindowStage) {
		GlobalContext.getContext().setObject(
			"display",
			await display.getDefaultDisplaySync()
		);
		windowStage.loadContent("pages/TabIndex");
	}
}
```

这里的 GlobalContext 和我们第一期讲述的定义方法是完全一致的  
下面是对应的代码

```ts
export class GlobalContext {
	private constructor() {}
	private static instance: GlobalContext;
	private _objects = new Map<string, Object>();

	public static getContext(): GlobalContext {
		if (!GlobalContext.instance) {
			GlobalContext.instance = new GlobalContext();
		}
		return GlobalContext.instance;
	}

	getObject(value: string): Object | undefined {
		return this._objects.get(value);
	}

	setObject(key: string, objectClass: Object): void {
		this._objects.set(key, objectClass);
	}
}
```

<br>

###
