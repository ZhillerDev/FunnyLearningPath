## 发送邮件

### 注册新浪邮箱

注册流程不用我废话了，直接注册就可以了

然后进入设置，开启 POP3/SMTP 服务  
此时你会拿到一个授权码，这个授权码可以当做密码用，记不记无所谓（如果你不怕密码泄露的话）

<br>

### Mail

前往 maven 中心仓库，找到 `spring-boot-starter-mail`，我们将通过它来发送邮件

复制对应配置代码，粘贴到本地 pom 文件里面即可

代码清单：`pom.xml`

```xml
<!-- https://mvnrepository.com/artifact/org.springframework.boot/spring-boot-starter-mail -->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-mail</artifactId>
    <version>2.7.15</version>
</dependency>
```

<br>

之后再 application.properties 文件配置邮箱链接许可

用户名和密码自己改过来

```
# MailProperties
spring.mail.host=smtp.sina.com
spring.mail.port=465
spring.mail.username=xxx@sina.com
spring.mail.password=xxx
spring.mail.protocol=smtp
spring.mail.properties.mail.smtp.ssl.enable=true
```

<br>

定制一个专用于发送邮件的工具类

代码清单：`/util/MailClient.java`

```java
package com.zhiller.community.zhillercommunity.util;


import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.mail.javamail.JavaMailSender;
import org.springframework.mail.javamail.MimeMessageHelper;
import org.springframework.stereotype.Component;

import javax.mail.MessagingException;
import javax.mail.internet.MimeMessage;

@Component
public class MailClient {
    public static final Logger logger = LoggerFactory.getLogger(MailClient.class);

    @Autowired
    private JavaMailSender mailSender;

    @Value("${spring.mail.username}")
    private String from;

    public void sendMail(String to, String subject, String content) {
        try {
            MimeMessage message = mailSender.createMimeMessage();
            MimeMessageHelper helper = new MimeMessageHelper(message);
            helper.setFrom(from);
            helper.setTo(to);
            helper.setSubject(subject);
            helper.setText(content, true);
            mailSender.send(helper.getMimeMessage());
        } catch (MessagingException e) {
            logger.error("发送邮件失败:" + e.getMessage());
        }
    }
}
```

<br>

测试是否可以正常发送邮件

```java
@RunWith(SpringRunner.class)
@SpringBootTest
@ContextConfiguration(classes = ZhillerCommunityApplication.class)
public class MailTest {
    @Autowired
    private MailClient mailClient;

    @Test
    public void testSendMail() {
        mailClient.sendMail(
                "xxx@qq.com",
                "TEST",
                "SHIT MAN"
        );
    }
}
```

<br>
