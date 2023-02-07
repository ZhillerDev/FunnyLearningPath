### 项目结构参考

#### OwnCloudApp

如题，本项目目录参考自 github 开源项目 `OwnCloudApp`

大家可以去为大佬点个 star：https://github.com/owncloud/android/tree/master/owncloudApp/src/main/java/com/owncloud/android

<br>

完整目录结构图

![](./imgs/main/struct/ms1.png)

<br>

### 依赖大全

#### 页面 UI

沉浸式状态栏依赖

```
implementation "com.google.accompanist:accompanist-insets:0.15.0"
implementation "com.google.accompanist:accompanist-insets-ui:0.15.0"
implementation "com.google.accompanist:accompanist-systemuicontroller:0.15.0"
```

<br>

Glide 依赖

```
implementation 'com.github.bumptech.glide:glide:4.11.0'
annotationProcessor 'com.github.bumptech.glide:compiler:4.11.0'
```

<br>

DataStore 依赖

```
implementation "androidx.datastore:datastore-preferences:1.0.0"
```

<br>
