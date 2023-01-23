### 快速上手

> 安装 jwt：`github.com/dgrijalva/jwt-go`

<br>

#### 原理简介

[建议参考该文章](https://blog.csdn.net/weixin_45070175/article/details/118559272?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522167443179416800215055357%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=167443179416800215055357&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-118559272-null-null.142^v71^insert_chatgpt,201^v4^add_ask&utm_term=jwt&spm=1018.2226.3001.4187)

优点：不使用 cookie，直接用请求头存储 token，避免跨域以及 CSRF 攻击

<br>

新建文件 `jwt.go`，我们将在里面编写关于 token 生成、校验的基础方法

<br>

#### 定义 claim 以及属性

claim 即 jwt 的载荷（payload）

Name 属性为我们自定义的载荷，而 `jwt.StandardClaims` 则为 jwt 自带的属性（比如生命周期以及 token 开始作用的时间啥的）

```go
type customClaim struct {
	Name string `json:"name"`
	jwt.StandardClaims
}
```

<br>

除此之外我们还需要两个关键参数：密钥以及生命周期

密钥作为第三个参数传递给 jwt 直接进行编码加密；  
生命周期可检测 token 是否已经过期；

注意密钥的类型必须是 `[]byte`

```go
var (
	secret        = []byte("hellojwt")  // 密钥
	expTime int64 = 30                  // 生命周期，单位为秒
)
```

<br>

#### 生成 token

由于我们定义的 claim 仅有一个自定义属性，则生成 token 的形参我们也仅需传递该值即可

这里定义了两个 jwt 字段，ExpiresAt（生命周期），以及 Issuer（签发者）；  
字段都不是必须的，但是为了完整性最好添加；

```go
func GenerateToken(name string) (string, error) {

    // 实例化claim
	claim := customClaim{
        // 定义自定义属性
		Name: name,
        // 定义jwt自带的属性
		StandardClaims: jwt.StandardClaims{
			ExpiresAt: int64(time.Now().Unix() + expTime), // 生命周期
			Issuer:    "zhiller",                          // 签发者
		},
	}

    // 通过claim创建原始token
    // 参数一：加密编码；参数二：使用的claim
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claim)

    // 使用指定的密钥对原始token进行签名，获得加密后的token
	return token.SignedString(secret)
}
```

<br>

#### 解析 token

没什么好说的，直接拿到 token，使用指定的 claim 结构配合密钥进行解密即可

```go
func ParseToken(token string) (*jwt.Token, *customClaim, error) {
	claim := &customClaim{}

    // 解析token的固定格式，第三个参数golang会自动生成
	t, err := jwt.ParseWithClaims(token, claim, func(token *jwt.Token) (interface{}, error) {
		return secret, nil
	})

	return t, claim, err
}
```

<br>

#### 校验 token

步骤较多，请直接看注释

这里使用到了上面我们写的 ParseToken 作为解析 token

```go
func ValidateToken(token string) bool {

    // ParseToken解析token
	_, _, err := ParseToken(token)

    // 如果解析出了问题
	if err != nil {
        // 获得错误类型
		if tp, ok := err.(*jwt.ValidationError); ok {
            // 将获取的类型与指定错误类型进行位与，非0表示确为该类型错误
            // jwt.ValidationErrorExpired表示超出生命周期错误
			if tp.Errors&jwt.ValidationErrorExpired != 0 {
                // 遇到错误了当然返回校验失败了呀
				return false
			}
		}
	}

    // 如果解析无问题，表示校验成功
	return true
}
```

<br>

#### 完整代码

```go
package middleware

import (
	"fmt"
	"github.com/dgrijalva/jwt-go"
	"time"
)

type customClaim struct {
	Name string `json:"name"`
	jwt.StandardClaims
}

var (
	secret        = []byte("hellojwt")
	expTime int64 = 30
)

func GenerateToken(name string) (string, error) {
	claim := customClaim{
		Name: name,
		StandardClaims: jwt.StandardClaims{
			ExpiresAt: int64(time.Now().Unix() + expTime),
			Issuer:    "zhiller",
		},
	}
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claim)
	return token.SignedString(secret)
}

func ParseToken(token string) (*jwt.Token, *customClaim, error) {
	claim := &customClaim{}
	t, err := jwt.ParseWithClaims(token, claim, func(token *jwt.Token) (interface{}, error) {
		return secret, nil
	})
	fmt.Printf("parse the token: %v\n", t)
	return t, claim, err
}

func ValidateToken(token string) bool {
	_, _, err := ParseToken(token)
	if err != nil {
		if tp, ok := err.(*jwt.ValidationError); ok {
			if tp.Errors&jwt.ValidationErrorExpired != 0 {
				return false
			}
		}
	}

	return true
}
```
