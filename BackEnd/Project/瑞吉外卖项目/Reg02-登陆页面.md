### 数据返回结果类

为了统一化数据在前后端的传输，我们制定一个规范叫做 R，则所有的数据都应该遵守该规范进行返回

新建结果类 `common/R.java`

```java
package com.zhiyiyi.reggie.common;

import lombok.Data;

import java.util.HashMap;
import java.util.Map;

@Data
public class R<T> {
    private Integer code; //编码：1成功，0和其它数字为失败

    private String msg; //错误信息

    private T data; //数据

    private Map<String, Object> map = new HashMap<String, Object>(); //动态数据

    public static <T> R<T> success(T object) {
        R<T> r = new R<T>();
        r.data = object;
        r.code = 1;
        return r;
    }

    public static <T> R<T> error(String msg) {
        R<T> r = new R<T>();
        r.msg = msg;
        r.code = 0;
        return r;
    }

    public R<T> add(String key, Object value) {
        this.map.put(key, value);
        return this;
    }
}
```

<br>

这是前端登陆请求方法，可见他取回了一个遵守 R 规范的结果集，并使用 `res.code===1` 判断执行结果是否成功，而与此同时从 `res.data` 中取出我们想要的结果！

```js
async handleLogin() {
    this.$refs.loginForm.validate(async (valid) => {
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

### controller 登陆方法

此次登陆使用了主要的几个技术：

1. DigestUtils 为 spring 框架自带的工具类，使用 MD5 进行加密
2. LambdaQueryWrapper 一种比较方式，具体使用方式查看 mybatisplus 官方文档
3. getOne 获取指定用户对象

```java
@Autowired
private EmployeeService employeeService;

@PostMapping("/login")
public R<Employee> login(HttpServletRequest servletRequest, @RequestBody Employee employee) {

    // 1. 获取用户密码并对其进行md5加密（加密方法使用spring自带的工具类实现）
    String password = employee.getPassword();
    password = DigestUtils.md5DigestAsHex(password.getBytes());

    // 2. 查询指定用户名
    LambdaQueryWrapper<Employee> queryWrapper = new LambdaQueryWrapper<>();
    queryWrapper.eq(Employee::getUsername, employee.getUsername());

    // 3. 因为我们在数据表里面限制了用户名是唯一的，所以直接使用getOne方法获取唯一一个用户对象
    Employee emp = employeeService.getOne(queryWrapper);

    // 4. 进行普通判断
    if (emp == null) {
        return R.error("查询用户失败！");
    }
    if (!emp.getPassword().equals(password)) {
        return R.error("密码错误！");
    }
    if (emp.getStatus() == 0) {
        return R.error("账户被禁用！");
    }

    // 5. 登陆成功后把结果存到session
    servletRequest.getSession().setAttribute("employee", emp.getId());
    return R.success(emp);
}
```

<br>

### 登出方法 logout

我们需要在 controller 中接收前端传来的登出 POST 请求

`HttpServletRequest` 可以获取 HTTP 请求的相关信息，但推荐全局注入的方式使用 HttpServletRequest，以下代码只是为了方便使用而已；

> 这里通过获取 request 体并删除 session 中的 employee 属性！

```java
@PostMapping("/logout")
public R<String> logout(HttpServletRequest request) {
    request.getSession().removeAttribute("employee");
    return R.success("退出成功");
}
```

<br>

前端登出代码，`logoutApi` 是一个封装好的 axios 请求

```js
logout() {
    logoutApi().then((res)=>{
        if(res.code === 1){
        localStorage.removeItem('userInfo')
        window.location.href = '/backend/page/login/login.html'
        }
    })
},
```

<br>

### 登陆拦截器

拦截流程：

1. 判断用户状态是否需要处理
2. 如果未登录->转到登录页面
3. 如果已登录->转到内部页面

<br>

登陆拦截器代码简要分析

1. AntPathMatcher 一个方便的进行 URI 比对的类
2. filterChain.doFilter 类似于中间件中的 next 方法，即拦截器放行

> 简要概括：这里使用拦截器对对应 URI 范围进行拦截，并判断 session 是否存在来决定用户是否允许被放行，或者直接重定向到登陆页面进行登陆操作！

```java
package com.zhiyiyi.reggie.filter;

import com.alibaba.fastjson.JSON;
import com.zhiyiyi.reggie.common.R;
import lombok.extern.slf4j.Slf4j;
import org.springframework.util.AntPathMatcher;

import javax.servlet.*;
import javax.servlet.annotation.WebFilter;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

@Slf4j
@WebFilter(filterName = "loginCheckFilter", urlPatterns = "/*")
public class LoginCheckFilter implements Filter {
    // 设置通配符匹配器，可以使用正则表达式进行匹配
    public static final AntPathMatcher PATH_MATCHER = new AntPathMatcher();

    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain filterChain) throws IOException, ServletException {
        // 向下转型
        HttpServletRequest request = (HttpServletRequest) servletRequest;
        HttpServletResponse response = (HttpServletResponse) servletResponse;

        // 获取拦截到的URI
        String requestURI = request.getRequestURI();

        // 指定我们需要拦截的URI，不在此列表的URI直接放行！
        String[] urls = new String[]{
                "/employee/login",
                "/employee/logout",
                "/backend/**",
                "/front/**"
        };

        log.info("拦截到请求:{}", request.getRequestURI());

        // 1. 获取URI是否需要拦截的状态
        boolean isCheck = check(urls, requestURI);

        // 2. 如果不需要拦截，直接放行
        if (isCheck) {
            filterChain.doFilter(request, response);
            return;
        }

        // 3. 判断登陆状态
        if (request.getSession().getAttribute("employee") != null) {
            // 如果已登录，直接放行
            filterChain.doFilter(request, response);
            return;
        }

        response.getWriter().write(JSON.toJSONString(R.error("NOTLOGIN")));
        return;
    }

    // 路径匹配检测方法
    public boolean check(String[] urls, String requestURI) {
        for (String url : urls) {
            boolean match = PATH_MATCHER.match(url, requestURI);
            if (match) return true;
        }
        return false;
    }
}

```

<br>
