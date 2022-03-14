# 序
《Linux高性能服务器编程》 -- 游双

# TCP/IP协议族
## TCP/IP协议族体系结构以及主要协议

![tcp_ip_layer](./pictures/tcp_ip_layer.png)

### 数据链路层
* 目的 - 实现了IP地址和机器物理地址(MAC地址)之间的转换
    * ARP协议(Address Resolve Protocol, 地址解析协议)
    * RARP协议(Reverse Address Resolve Protocol, 逆地址解析协议)
* 抽象 - 数据链路层隐藏了物理媒介的不同

### 网络层
* 目的 - 实现数据包的选路和转发，网络层的任务是选择主机之间的路由节点，以确定两台主机之间的通信路径
    * IP协议(Internet Protocol, 因特网协议)
    * ICMP协议(Internet Control Message Protocol, 因特网控制报文协议)
        * 用于检测网络连接
* 抽象 - 网络层隐藏了网络拓扑连接的细节

### 传输层

![tcp_ip_layer_detail](./pictures/tcp_ip_layer_detail.png)

* 目的 - 为两台主机上的应用程序提供端到端(end to end)的通信，与网络层使用的逐跳通信方式不同，传输层只关心通信的起始端和目的端
    * TCP协议(Transmission Control Protocol, 传输控制协议)
    * UDP协议(User Datagram Protocol，用户数报协议)
    * SCTP协议(Stream Control Transmission Protocol, 流控制传输协议)

### 应用层
* 目的 - 处理应用程序的逻辑，在用户空间实现(其他层级在内核中实现)
    * ping应用程序
        * 利用ICMP报文检测网络连接
    * telnet协议
        * 远程登陆协议
    * OSPF协议(Open Shortest Path First, 开放最短路径优先)
        * 动态路由更新协议，用于路由之间的通信
    * DNS协议(Domain Name Service, 域名服务)
        * 提供机器域名到IP地址的转换

## 封装(encapsulation)
![encapsulation](./pictures/encapsulation.png)

### TCP封装

![tcp_encapsulation](./pictures/tcp_encapsulation.png)

* TCP报文段(TCP message segment) - 经TCP封装后的数据
    * 上图中的虚线框构成了TCP报文段，由TCP头部信息和TCP内核缓冲区构成
* UDP数据报(UDP datagram) - 经UDP封装后的数据
    * UDP不保存数据副本，数据发送成功后，内核缓冲区的数据就丢弃了

### IP封装
* IP数据报(IP datagram) - 经过IP封装后的数据
    * 其数据部分可能是
        * TCP报文段
        * UDP数据报
        * ICMP报文

### 数据链路层封装
* 帧(frame) - 经过数据链路层封装的数据
    * 以太网帧(ethernet frame) - 在以太网上传输
    * 令牌环帧(token ring frame) - 在令牌环网络上传输

![ethernet_frame](./pictures/ethernet_frame.png)
* 最大传输单元(Max Transmit Unit, MTU) - 最多能携带多少上层协议数据
    * 上图的以太网帧的MTU是1500字节，过长的IP数据报需要被分片(fragment)传输

## 分用(demultiplexing)
!(demultiplexing)[./pictures/demultiplexing.png]
* 分用
    * 当帧到达目的主机时，将沿着协议栈自底向上依次传递。各层协议依次处理帧中本层负责的头部数据，以获取所需的信息，并最终将处理后的帧交给目标应用程序。
    * 分用时依靠头部信息中的类型字段实现的
* 如何区分IP、ARP和RARP帧？
    * 帧头部有2字节的类型字段，0x800为IP数据报，0x806为ARP请求应答报文，0x835为RARP请求或应答报文
* 如何区分ICMP、TCP和UDP的IP数据报？
    * IP数据报的头部采用16位的协议字段来区分
* 如何区分TCP/UDP的不同上层应用？
    * 通过头部中的16位的端口号(port number)来区分，53为DNS协议，80为HTTP协议

## 测试网络
本文用虚拟机模拟了两个网卡：
* 主网卡
    * 192.168.179.132
    * 00:0c:29:35:c3:33
    * ens33
* 次网卡
    * 192.168.179.139
    * 00:0c:29:35:c3:3d
    * ens38
* 默认网关
    * 192.168.179.2, _gateway
    * 00:50:56:ee:0b:41
* Host用于与NAT虚拟网络进行通信的虚拟网卡
    * 192.168.179.1
    * 00:50:56:c0:00:08

## ARP协议工作原理
* 工作原理
    * 主机向自己所在的网络广播一个ARP请求，该请求包含目标机器的网络地址。此网络上的其他机器都将收到这个请求，但只有被请求的目标机器会回应一个ARP应答，其中包含自己的物理地址

### 以太网ARP请求/应答报文详解
![ARP_datagram](./pictures/ARP_datagram.png)

### ARP高速缓存的查看和修改
ARP维护一个高速缓存，其中包含经常访问(比如默认网关)或最近访问的机器的IP地址到物理地址的映射。利用`arp -a`可以查看ARP高速缓存，例如：
```bash
> arp -a
? (192.168.179.1) at 00:50:56:c0:00:08 [ether] on ens33
? (192.168.179.1) at 00:50:56:c0:00:08 [ether] on ens38
_gateway (192.168.179.2) at 00:50:56:ee:0b:41 [ether] on ens33
_gateway (192.168.179.2) at 00:50:56:ee:0b:41 [ether] on ens3
? (192.168.179.254) at 00:50:56:f9:51:f7 [ether] on ens33
? (192.168.179.254) at 00:50:56:f9:51:f7 [ether] on ens38
```
* 其中，前两条描述的是HOST端的虚拟网卡，中间两条描述的是默认网关，最后两条描述的是路由
    * 默认网关的IP是：192.168.179.2, MAC地址是：00:50:56:ee:0b:41, 这和`ip`命令的结果是一致的：
    ```bash
    > ip neigh
    192.168.179.2 dev ens33 lladdr 00:50:56:ee:0b:41 STALE
    192.168.179.2 dev ens38 lladdr 00:50:56:ee:0b:41 STALE
    > ip route
    default via 192.168.179.2 dev ens33 proto dhcp metric 100
    default via 192.168.179.2 dev ens38 proto dhcp metric 101 
    ```

### 使用tcpdump观察ARP通信过程
* 实验
    * 在Host端执行telnet命令登录到虚拟机192.168.179.139，并用tcpdump抓取这个过程中两台机器之间交换的以太网帧
* 命令
    * 虚拟机
        * 清楚ARP cache: `sudo ip -s -s neigh flush all`
        * 捕获目标或者源是192.168.179.132的ARP包：`sudo tcpdump -i ens33 -ent '(arp and dst 192.168.179.132) or (arp and src 192.168.179.132)'`
    * HOST：telnet -a 192.168.179.132
    * 捕获如下ARP包：
        * `00:0c:29:35:c3:33`是主虚拟网卡`192.168.179.132`的MAC地址
        * `00:50:56:ee:0b:41`是默认网关的MAC地址
        * 主虚拟网卡向默认网关要其MAC地址
        * 默认网关返回ARP Reply包给主虚拟网卡，并告知其MAC地址是`00:50:56:ee:0b:41`
        * `arp -n`中添加了如下条目：
            * `192.168.179.2            ether   00:50:56:ee:0b:41   C                     ens33`
    ```bash
    00:0c:29:35:c3:33 > 00:50:56:ee:0b:41, ethertype ARP (0x0806), length 42: Request who-has 192.168.179.2 tell 192.168.179.132, length 28
    00:50:56:ee:0b:41 > 00:0c:29:35:c3:33, ethertype ARP (0x0806), length 60: Reply 192.168.179.2 is-at 00:50:56:ee:0b:41, length 46
    ```

![arp_com](./pictures/arp_com.png)
* 上图中，路由器也收到了以太网帧1，因为该帧是一个广播帧，不过路由器并没有回应其中的ARP请求。
