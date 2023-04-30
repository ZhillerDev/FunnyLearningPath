## 1. 安装 Docker

在安装 Docker 之前，请确保您的系统符合最低系统要求，并且您已登录为具有 sudo 权限的用户。可以参考前面的问题“如何在 Ubuntu 上安装 Docker”来安装 Docker。

## 2. 下载 Docker 镜像

要使用 Docker，您需要从 Docker Hub 或其他 Docker 镜像仓库中下载需要的 Docker 镜像。可以使用以下命令来下载 Docker 镜像：

```
docker pull image_name
```

其中，`image_name`是要下载的 Docker 镜像的名称。

例如，要下载 Ubuntu 20.04 的 Docker 镜像，可以使用以下命令：

```
docker pull ubuntu:20.04
```

## 3. 运行 Docker 容器

要运行 Docker 容器，可以使用以下命令：

```
docker run [OPTIONS] image_name [COMMAND] [ARG...]
```

其中，`image_name`是要运行的 Docker 镜像的名称，`[COMMAND] [ARG...]`是在容器内执行的命令。

例如，要运行 Ubuntu 20.04 的 Docker 容器并在容器内启动 Bash 终端，可以使用以下命令：

```
docker run -it --rm ubuntu:20.04 /bin/bash
```

其中，`-it`选项表示以交互模式运行容器并分配一个伪终端，`--rm`选项表示容器退出后自动删除，`/bin/bash`命令表示在容器内启动 Bash 终端。

## 4. 查看 Docker 容器

要查看正在运行的 Docker 容器，可以使用以下命令：

```
docker ps [OPTIONS]
```

其中，`OPTIONS`是一些可选参数，例如：

- `-a`：显示所有容器，包括已停止的容器。
- `-q`：仅显示容器 ID，而不显示其他详细信息。

例如，要查看所有正在运行的 Docker 容器，可以使用以下命令：

```
docker ps -a
```

## 5. 停止 Docker 容器

要停止正在运行的 Docker 容器，可以使用以下命令：

```
docker stop container_id
```

其中，`container_id`是要停止的 Docker 容器的 ID。

例如，要停止 ID 为`abcd1234`的 Docker 容器，可以使用以下命令：

```
docker stop abcd1234
```

## 6. 删除 Docker 容器

要删除已停止的 Docker 容器，可以使用以下命令：

```
docker rm container_id
```

其中，`container_id`是要删除的 Docker 容器的 ID。

## 7. 查看 Docker 镜像

要查看已下载的 Docker 镜像，可以使用以下命令：

```
docker images [OPTIONS] [REPOSITORY[:TAG]]
```

其中，`OPTIONS`是一些可选参数，例如：

- `-a`：显示所有镜像，包括中间镜像层。
- `--digests`：显示镜像的摘要信息。
- `--no-trunc`：显示完整的镜像 ID。

`REPOSITORY`和`TAG`可以用于过滤镜像列表。

例如，要查看所有已下载的 Docker 镜像，可以使用以下命令：

```
docker images -a
```

## 8. 删除 Docker 镜像

要删除已下载的 Docker 镜像，可以使用以下命令：

```
docker rmi [OPTIONS] IMAGE [IMAGE...]
```

其中，`OPTIONS`是一些可选参数，例如：

- `-f`：强制删除镜像，即使它正在被使用。
- `--no-prune`：不删除被标记为“<none>”的镜像。

`IMAGE`是要删除的 Docker 镜像的名称或 ID。

例如，要删除名为`ubuntu`的 Docker 镜像，可以使用以下命令：

```
docker rmi ubuntu
```

## 9. 构建 Docker 镜像

如果您需要自定义 Docker 镜像，可以使用 Dockerfile 构建自己的 Docker 镜像。Dockerfile 是一种文本文件，用于定义如何构建 Docker 镜像。可以使用以下命令构建 Docker 镜像：

```
docker build [OPTIONS] PATH | URL | -
```

其中，`OPTIONS`是一些可选参数，例如：

- `-t`：为镜像指定名称和标记。
- `-f`：指定要使用的 Dockerfile 路径。

`PATH`或`URL`是包含 Dockerfile 的路径或 URL，或者使用“-”表示从标准输入中读取 Dockerfile。

例如，假设您有一个包含 Dockerfile 的目录，该 Dockerfile 将在 Ubuntu 20.04 基础映像上安装 Apache Web 服务器。您可以使用以下命令构建自己的 Docker 镜像：

```
docker build -t my-apache-image ./path/to/Dockerfile
```

## 10. 导出和导入 Docker 镜像

如果您需要在不同的 Docker 主机之间共享 Docker 镜像，可以将其导出为 tar 文件，并在其他主机上导入。可以使用以下命令导出 Docker 镜像：

```
docker save [OPTIONS] IMAGE [IMAGE...]
```

其中，`OPTIONS`是一些可选参数，例如：

- `-o`：指定输出文件名。

`IMAGE`是要导出为 tar 文件的 Docker 镜像的名称或 ID。

例如，要导出名为`ubuntu`的 Docker 镜像并将其保存到`ubuntu.tar`文件中，可以使用以下命令：

```
docker save -o ubuntu.tar ubuntu
```

要在其他主机上导入 Docker 镜像，请使用以下命令

```
docker load [OPTIONS]
```

其中，`OPTIONS`是一些可选参数，例如：

- `-i`：指定输入文件名。

例如，要在其他主机上导入名为`ubuntu`的 Docker 镜像，请使用以下命令：

```
docker load -i ubuntu.tar
```

## 11. 运行 Docker 容器

要运行 Docker 容器，可以使用以下命令：

```
docker run [OPTIONS] IMAGE [COMMAND] [ARG...]
```

其中，`OPTIONS`是一些可选参数，例如：

- `-d`：在后台运行容器。
- `-p`：将容器端口映射到主机端口。
- `-v`：将主机目录挂载到容器内部。
- `-e`：设置环境变量。

`IMAGE`是要运行的 Docker 镜像的名称或 ID。

`COMMAND`是要在容器内运行的命令，默认情况下是 Docker 镜像中指定的命令。

`ARG`是传递给命令的参数。

例如，要在后台运行一个名为`ubuntu`的 Docker 容器，请使用以下命令：

```
docker run -d ubuntu
```

要在容器中运行命令，请使用以下命令：

```
docker exec [OPTIONS] CONTAINER COMMAND [ARG...]
```

其中，`OPTIONS`是一些可选参数，例如：

- `-i`：保持 STDIN 打开，即使没有连接。
- `-t`：分配一个伪终端。

`CONTAINER`是要在其中运行命令的容器的名称或 ID。

例如，要在名为`my-container`的 Docker 容器中运行`ls`命令，请使用以下命令：

```
docker exec -it my-container ls
```

## 12. 停止和删除 Docker 容器

要停止正在运行的 Docker 容器，可以使用以下命令：

```
docker stop [OPTIONS] CONTAINER [CONTAINER...]
```

其中，`OPTIONS`是一些可选参数，例如：

- `-t`：指定停止容器的超时时间。

`CONTAINER`是要停止的 Docker 容器的名称或 ID。

例如，要停止名为`my-container`的 Docker 容器，请使用以下命令：

```
docker stop my-container
```

要删除已停止的 Docker 容器，请使用以下命令：

```
docker rm [OPTIONS] CONTAINER [CONTAINER...]
```

其中，`OPTIONS`是一些可选参数，例如：

- `-f`：强制删除正在运行的容器。

`CONTAINER`是要删除的 Docker 容器的名称或 ID。

例如，要删除名为`my-container`的 Docker 容器，请使用以下命令：

```
docker rm my-container
```
