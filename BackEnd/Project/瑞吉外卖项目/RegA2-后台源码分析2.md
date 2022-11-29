### api

这一块没啥好说的，基本都是重复的内容；

就直接拿 category.js 第一个接口举例吧：

```js
// 查询列表接口
const getCategoryPage = (params) => {
  // 调用axios实例处理该请求并返回一个promise对象，来判定请求结果
  return $axios({
    url: "/category/page",
    method: "get",
    params,
  });
};
```

<br>

### index.html

> 整体文件概览：
> elementui+vue2+axios

<br>

#### html 部分

主体分为左右两大板块，即 sidebar 和 mainbody

mainbody 右包含了一个 navbar 和 appmain，由于没有添加路由依赖，故源码中改为使用 iframe 实现路由效果

基本都由 elementui+css 实现，没有什么技术点

<br>

#### created()

```js
// 实例创建时获取用户信息
created() {
    const userInfo = window.localStorage.getItem('userInfo')
    if (userInfo) {
        this.userInfo = JSON.parse(userInfo)
    }
    // 如果成功取得用户信息，则结束加载状态
    this.closeLoading()
},


// closeLoading() {
//     this.timer = null
//     this.timer = setTimeout(() => {
//         this.loading = false
//     }, 1000)
// }
```

<br>

#### 登出与页面回退

```js
// 登出请求
logout() {
  // axios请求后端登出许可
  // 许可成功，移除本地存储的用户信息，重定向页面到登陆
  logoutApi().then((res) => {
      if (res.code === 1) {
          localStorage.removeItem('userInfo')
          window.location.href = '/backend/page/login/login.html'
      }
  })
},

// 回退方法
goBack() {
  // window.location.href = 'javascript:history.go(-1)'
  const menu = this.menuList.find(item => item.id === this.menuActived)
  // this.goBackFlag = false
  // this.headTitle = menu.name
  this.menuHandle(menu, false)
},
menuHandle(item, goBackFlag) {
  this.loading = true
  this.menuActived = item.id
  this.iframeUrl = item.url
  this.headTitle = item.name
  this.goBackFlag = goBackFlag
  this.closeLoading()
},
```

<br>

### login.html

#### 登陆表单主体

```html
<!-- 绑定登陆规则loginRules -->
<el-form ref="loginForm" :model="loginForm" :rules="loginRules">
  <div class="login-form-title">
    <img
      src="../../images/login/logo.png"
      style="width:139px;height:42px;"
      alt=""
    />
  </div>

  <!-- 账户输入表单，注意双向绑定 -->
  <el-form-item prop="username">
    <el-input
      v-model="loginForm.username"
      type="text"
      auto-complete="off"
      placeholder="账号"
      maxlength="20"
      prefix-icon="iconfont icon-user"
    />
  </el-form-item>
  <el-form-item prop="password">
    <el-input
      v-model="loginForm.password"
      type="password"
      placeholder="密码"
      prefix-icon="iconfont icon-lock"
      maxlength="20"
      @keyup.enter.native="handleLogin"
    />
  </el-form-item>
  <el-form-item style="width:100%;">
    <el-button
      :loading="loading"
      class="login-btn"
      size="medium"
      type="primary"
      style="width:100%;"
      @click.native.prevent="handleLogin"
    >
      <!-- 判断当前变量状态显示不同的信息 -->
      <span v-if="!loading">登录</span>
      <span v-else>登录中...</span>
    </el-button>
  </el-form-item>
</el-form>
```

<br>

#### 登陆验证

这是 elementui 规定的固定校验规则，你只需要套用即可

```js
loginRules() {
  const validateUsername = (rule, value, callback) => {
    if (value.length < 1 ) {
      callback(new Error('请输入用户名'))
    } else {
      callback()
    }
  }
  const validatePassword = (rule, value, callback) => {
    if (value.length < 6) {
      callback(new Error('密码必须在6位以上'))
    } else {
      callback()
    }
  }
  return {
    'username': [{ 'validator': validateUsername, 'trigger': 'blur' }],
    'password': [{ 'validator': validatePassword, 'trigger': 'blur' }]
  }
}
```

<br>

#### 登陆请求

标准登陆请求模板

```js
async handleLogin() {
  // $refs获取指定组件（vue2方法）
  // validate内必须要使用异步方法
  this.$refs.loginForm.validate(async (valid) => {
    // 如果请求数据不为空，直接请求后端
    // 请求到了就保存用户数据到本地，否则返回error
    if (valid) {
      this.loading = true
      let res = await loginApi(this.loginForm)
      if (String(res.code) === '1') {
        localStorage.setItem('userInfo',JSON.stringify(res.data))
        window.location.href= '/backend/index.html'
      } else {
        this.$message.error(res.msg)
        this.loading = false
      }
    }
  })
}
```

<br>
