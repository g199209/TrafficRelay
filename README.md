# TrafficRelay

## Introduction 简介

This is a very simple program to relay TCP or UDP network traffic.

这个简单小程序的主要用途是中继转发 TCP 或 UDP 网络流量。

There are two endpoints, each endpoint can be one of the following state:

1. TCP passive mode, listen a port and waiting other host to establish a connection.
2. TCP active mode, connect to other host to establish a connection.
3. UDP passive mode
4. UDP active mode

系统由两个端点组成，每个端点可以是以下几种状态之一：

1. TCP 被动模式，监听某端口，等待其他主机来建立连接；
2. TCP 主动模式，主动去连接特定主机的某个端口；
3. UDP 被动模式
4. UDP 主动模式

When the connection of both two endpoint are established, transfer network traffic between these two connections. That is to say, receive data from one connection and send it to another connection.

That's all this program done!

当两个端点的网络连接都建立好后，系统会在这两个连接之间转发数据，也就是说会把一个连接收到的数据发到另一个连接上。

这就是此程序的所有作用了！

## How to use 如何使用

This program support Linux & macOS, Windows is not supported yet. You can download prebuilt binary or build by yourself.

本程序支持 Linux 及 Mac，暂不支持 Windows。可直接下载预编译文件使用也可自行编译。

Configure the EP by setting corresponding `gflags`, refer to `configure_flags.cc` file for details.

使用 `gflags` 来配置 EP，详情可查阅 `configure_flags.cc` 文件。

```bash
./TrafficRelay --FirstEpAddr=192.168.0.1:22 --FirstEpProto=TCP --FirstEpDir=Connect --SecondEpAddr=127.0.0.1:1000 --SecondEpProto=UDP --SecondEpDir=Listen
```

## How to build 如何编译

```bash
mkdir build
cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
make -j8
```

This program depends on [spdlog](https://github.com/gabime/spdlog) & [gflags](https://github.com/gflags/gflags)

依赖于 [spdlog](https://github.com/gabime/spdlog) & [gflags](https://github.com/gflags/gflags)

