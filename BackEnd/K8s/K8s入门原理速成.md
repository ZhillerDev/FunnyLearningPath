### K8s 基本组成

> kubernetes 是一个基于容器技术的分布式架构方案

<br>

#### Master

> 一旦 master 宕机，整个服务架构直接崩溃

集群控制节点，负责整个集群的管理和控制，基本上接收 Kubernetes 的所有控制命令；  
它包含三个组件

1. Kubernetes API Server(kube-apiserver) 集群的统一入口，各组件协调者，以 HTTP API 提供接口服务，处理所有 CRUD 请求
2. Kubernetes Controller Ｍ anager(kube-controller-manager) 所有资源对象的自动化控制中心
3. kube-scheduler 负责资源调度（Pod 调度）的进程

<br>

#### Node

> 一个 node 宕机，会被 master 转移给其他的 node

工作负载节点，除了 master 节点外的机器被称为 Node

1. kubelet 管理本机运行容器的生命周期，负责 Pod 对应容器的创建、启停等任务
2. kube-proxy 在 Node 节点上实现 Pod 网络代理，实现负载均衡机制
3. docker 负责本机的容器的创建和管理工作

<br>

#### Etcd

Kubernetes 提供默认的存储系统，保存所有集群数据

<br>

#### Replication Controller

它能够保证 Pod 持续运行，并且在任何时候都有指定数量的 Pod 副本

<br>

#### ReplicaSet

ReplicaSet 实现了 Pod 的多副本管理。使用 Deployment 时会自动创建 ReplicaSet

<br>

#### Service

它一组逻辑 pod 的抽象，为一组 pod 提供统一入口；

他可以实现如下两种常用的入口

1. ClusterIP：Service 在集群内的唯一 ip 地址
2. NodeIP+NodePort：Service 会在集群的每个 Node 上都启动一个端口，通过 NodeIP:NodePort 访问后端的 Pod

<br>

#### Volume

它是 Pod 中能够被多个容器共享的磁盘目录；

有 N 多个类型，这里就不一一例举了
