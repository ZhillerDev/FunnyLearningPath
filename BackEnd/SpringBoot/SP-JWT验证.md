### 基本验证

#### 个人理解的基本过程

1. 用户首次登陆，请求 token，后端颁发
2. 用户登录后携带 token 进行接口请求，后端检测到 token 则返回结果，否则予以拒绝
3. 用户没 token 无法请求对应接口
4. token 必须要有过期时长，不可以一直使用
5. 不要在前端进行 token 的校验判断，这些都是后端的工作

<br>

#### 依赖

```xml
<!-- https://mvnrepository.com/artifact/com.auth0/java-jwt -->
<dependency>
    <groupId>com.auth0</groupId>
    <artifactId>java-jwt</artifactId>
    <version>3.9.0</version>
</dependency>

<!-- https://mvnrepository.com/artifact/org.json/json -->
<dependency>
    <groupId>org.json</groupId>
    <artifactId>json</artifactId>
    <version>20190722</version>
</dependency>
```

<br>

#### 拦截器

新增文件夹 config

添加全局拦截器 webconfiguration.java

```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Configuration;
import org.springframework.scheduling.concurrent.ConcurrentTaskExecutor;
import org.springframework.web.servlet.config.annotation.AsyncSupportConfigurer;
import org.springframework.web.servlet.config.annotation.CorsRegistry;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Executors;

@Configuration
public class WebConfiguration implements WebMvcConfigurer {
    @Autowired
    private TokenInterceptor tokenInterceptor;

    @Override
    public void addCorsMappings(CorsRegistry registry) {
        registry.addMapping("/**")
                .allowCredentials(true)
                .allowedMethods("*")
                .allowedHeaders("*")
                .allowedOriginPatterns("*");
    }

    @Override
    public void configureAsyncSupport(AsyncSupportConfigurer configurer) {
        configurer.setTaskExecutor(new ConcurrentTaskExecutor(Executors.newFixedThreadPool(3)));
        configurer.setDefaultTimeout(3000);
    }

    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        List<String> excludePath = new ArrayList<>();
        //排除拦截，除了注册登录(此时还没token)，其他都拦截
        excludePath.add("/api/login");  //登录
        excludePath.add("/api/register");     //注册
        excludePath.add("/doc.html");     //swagger
        excludePath.add("/swagger-ui.html");     //swagger
        excludePath.add("/swagger-resources/**");     //swagger
        excludePath.add("/v2/api-docs");     //swagger
        excludePath.add("/webjars/**");     //swagger
        registry.addInterceptor(tokenInterceptor)
                .addPathPatterns("/**")
                .excludePathPatterns(excludePath);
        WebMvcConfigurer.super.addInterceptors(registry);
    }
}
```

<br>

token 拦截器 tokeninterceptor.java

```java
import com.zhiyiyi.backend.utils.TokenUtil;
import org.json.JSONObject;
import org.springframework.stereotype.Component;
import org.springframework.web.servlet.HandlerInterceptor;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

@Component
public class TokenInterceptor implements HandlerInterceptor {
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {

        //跨域请求会首先发一个option请求，直接返回正常状态并通过拦截器
        if (request.getMethod().equals("OPTIONS")) {
            response.setStatus(HttpServletResponse.SC_OK);
            return true;
        }
        response.setCharacterEncoding("utf-8");
        String token = request.getHeader("token");
        if (token != null) {
            boolean result = TokenUtil.verify(token);
            if (result) {
                System.out.println("通过拦截器");
                return true;
            }
        }
        response.setContentType("application/json; charset=utf-8");
        try {
            JSONObject json = new JSONObject();
            json.put("msg", "token verify fail");
            json.put("code", "500");
            response.getWriter().append(json.toString());
            System.out.println("认证失败，未通过拦截器");
        } catch (Exception e) {
            return false;
        }
        /**
         * 还可以在此处检验用户存不存在等操作
         */
        return false;
    }
}
```

<br>

#### 前端处理

以下代码为封装 axios 请求

注册请求拦截器，并为请求头添加 token 参数；  
从 localstorage 里面找，找得到就返回，找不到返回 null；

```js
service.interceptors.request.use(
  (config) => {
    config.headers["token"] = getToken("token") ? getToken("token") : "null";
    return config;
  },
  (error) => {
    return Promise.reject(error);
  }
);
```

<br>

至于登陆校验 validate 以及 axios 请求 post 这些玩意请参考其他文章，这里不做解析

> 请记住这一点，目前每一条请求都附带一个本用户 token，或者为 null

<br>

#### 后端处理

token 工具类，进行 token 颁发以及验证

```java
import com.auth0.jwt.JWT;
import com.auth0.jwt.algorithms.Algorithm;
import com.auth0.jwt.exceptions.JWTCreationException;
import com.auth0.jwt.exceptions.JWTVerificationException;
import com.auth0.jwt.interfaces.DecodedJWT;
import com.auth0.jwt.interfaces.JWTVerifier;
import com.zhiyiyi.backend.entity.User;

import java.util.Date;

public class TokenUtil {

    // 设置token过期时间为10h
    private static final long EXPIRE_TIME = 10 * 60 * 60 * 1000;
    // 加盐
    private static final String TOKEN_SECRET = "ljdyai13daseqi?:Aqwe12*&*^";


    // 颁发token
    public static String sign(User user) {

        String token = null;
        try {
            Date expireAt = new Date(System.currentTimeMillis() + EXPIRE_TIME);
            token = JWT.create()
                    //发行人
                    .withIssuer("auth0")
                    //存放数据
                    .withClaim("username", user.getUsername())
                    //过期时间
                    .withExpiresAt(expireAt)
                    // 加密方式
                    .sign(Algorithm.HMAC256(TOKEN_SECRET));
        } catch (IllegalArgumentException | JWTCreationException ignored) {

        }
        return token;
    }


    // 验证token
    public static Boolean verify(String token) {

        try {
            //创建token验证器
            JWTVerifier jwtVerifier = JWT.require(Algorithm.HMAC256(TOKEN_SECRET)).withIssuer("auth0").build();
            // JWT解码token，验证通过即在后端展示出来
            DecodedJWT decodedJWT = jwtVerifier.verify(token);
            System.out.println("认证通过：");
            System.out.println("username: " + decodedJWT.getClaim("username").asString());
            System.out.println("过期时间：      " + decodedJWT.getExpiresAt());
        } catch (IllegalArgumentException | JWTVerificationException e) {
            //抛出错误即为验证不通过
            return false;
        }
        return true;
    }

}
```

<br>

在任意一个 controller 内获取 token

下面展示了一个无参 POST 请求；

`@RequestHeader("token")` 表示获取 header 中 key 为 token 的值

使用工具类对 token 校验，校验成功则返回对应信息

```java
@PostMapping("/info")
public String getInfo(@RequestHeader("token") String header) {
    if (TokenUtil.verify(header)) {
        return "你有权限访问该接口！";
    }
    return "你无权访问！";
}
```

<br>
