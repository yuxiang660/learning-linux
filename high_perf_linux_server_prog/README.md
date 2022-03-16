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

## DNS工作原理

![DNS_datagram](./pictures/DNS_datagram.png)
* DNS查询类型
    * 类型A，值是1，表示获取目标主机的IP地址
    * 类型CNAME，值是5，表示获得目标主机的别名
    * 类型PRT，值是12，表示反向查询

### Linux下访问DNS服务
* `/etc/resolv.conf`存放了DNS服务器的IP地址`nameserver`
* `host -t A <addr-name>` - 查询类型A的机器名对应的IP

### 使用tcpdump观察DNS通信过程
* `sudo tcpdump -i ens33 -nt -s 500 port domain`
    * 只抓取`ens33`上的使用domain(域名)服务的数据包，即DNS查询和应答报文
* `host -t A www.baidu.com`执行后，抓取的数据：
    * 第一个数据包中，
        * 56694是DNS查询报文的标识值，因此该值也出现在了DNS应答报文中
        * “+”表示递归查询
        * “A?”表示使用A类型的查询方式
        * 括号中的数值42是DNS查询报文的长度(以字节为单位)
    * 第二个数据包中，
        * “4/0/1”表示该报文中包含4个应答资源记录、0个授权资源记录和1个额外信息记录
            * `CNAME www.a.shifen.com., CNAME www.wshifen.com., A 119.63.197.139, A 119.63.197.151`表示4个应答资源记录
    ```bash
    IP 192.168.179.132.49122 > 192.168.179.2.53: 56694+ [1au] A? www.baidu.com. (42)
    IP 192.168.179.2.53 > 192.168.179.132.49122: 56694 4/0/1 CNAME www.a.shifen.com., CNAME www.wshifen.com., A 119.63.197.139, A 119.63.197.151 (127)
    ```

## socket和TCP/IP协议族的关系
数据链路层、网络层、传输层协议在内核中实现，操作系统提供了两套系统调用来访问这些协议，分别是：
* socket
* XTI (基本不用)

socket API提供了两点功能：
* 将应用程序数据从用户缓冲区中复制到TCP/UDP内核发送缓冲区，以交付内核来发送数据；或从内核TCP/UDP接收缓冲区中复制数据到用户缓冲区，以读取数据
* 应用程序可通过它们来修改内核中各层协议的某些头部信息或其他数据结构，从而精细地控制底层通信的行为

# IP协议详解
本章从两个方面讨论IP协议：
* IP头部信息
    * 用于指定IP通信的源端和目的端
* IP数据报的路由和转发

## IP服务的特点
IP协议为上层协议提供无状态、无连接、不可靠的服务：
* 无状态
    * IP通信双方不同步传输数据的状态信息，没有上下文关系
    * 优点是简单、高效
* 无连接
    * IP通信双方都不长久地维持对方的任何信息，上层协议每次发送数据的时候，都必须明确指定对方的IP地址
* 不可靠
    * IP协议不能保证IP数据报准确地到达接收端，使用IP服务的上层协议需要自己实现数据确认、超时重传等机制以达到可靠传输的目的

## IPv4头部结构

![ipv4_header](./pictures/ipv4_header.png)
* 长度通常为20字节

### 使用tcpdump观察IPv4头部结构
```bash
> sudo tcpdump -ntx -i lo port 23
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on lo, link-type EN10MB (Ethernet), capture size 262144 bytes
IP 127.0.0.1.41684 > 127.0.0.1.23: Flags [S], seq 3956412104, win 65495, options [mss 65495,sackOK,TS val 2922181685 ecr 0,nop,wscale 7], length 0
        0x0000:  4510 003c c09e 4000 4006 7c0b 7f00 0001
        0x0010:  7f00 0001 a2d4 0017 ebd2 0ec8 0000 0000
        0x0020:  a002 ffd7 fe30 0000 0204 ffd7 0402 080a
        0x0030:  ae2c f435 0000 0000 0103 0307
```
* 我们使用telnet登录本机，所以IP数据报的源端IP地址和目的端IP地址都是“127.0.0.1”
* telnet服务器程序使用的端口号是23，而telnet客户端使用临时端口号41684与服务器通信
* `-x`选项输出了二进制码，此数据包共60字节，前20字节是IP头部，后40字节是TCP头部，不包含应用程序数据(length值为0)
    * ![ipv4_data](./pictures/ipv4_data.png)
        * 其中数据报标识是：0xc09e

## IP分片
当IP数据报长度超过MTU时，它会被分片，并最终在目标机器的IP模块中被重新组装。IP头部中下面三个字段给IP的分片和重组提供了足够信息：
* 数据报标识
* 标志
* 片偏移

`ifconfig`命令可查看MTU的值。例如，“192.168.179.132”的MTU值是1500，表明其IP数据报最多是1480字节(IP头部占用20字节)。如果要封装一个长度为1481字节的ICMP报文，则必须分片，如下图。
```bash
> ifconfig
ens33: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.179.132  netmask 255.255.255.0  broadcast 192.168.179.255
```

![fragment](./pictures/fragment.png)

### 使用tcpdump观察IP分片
* `sudo tcpdump -ntv -i ens38 icmp` - 监听ens38上的ICMP报文
* `ping 127.0.0.1 -s 1473` - 向本机发送1473字节的数据
* tcpdump输出的一个IP数据报的两个分片，这两个分片的标识值都是`42695`，第一个分片的偏移值为0，第二个分片的偏移值为1480。`flags [+]`标识(More Fragment, MF)，意味着还有后续分片。`flags [none]`没有设置任何标志位
    ```bash
    IP (tos 0x0, ttl 128, id 42695, offset 0, flags [+], proto ICMP (1), length 1500)
    192.168.179.1 > 192.168.179.132: ICMP echo request, id 112, seq 1, length 1480
    IP (tos 0x0, ttl 128, id 42695, offset 1480, flags [none], proto ICMP (1), length 21)
    192.168.179.1 > 192.168.179.132: ip-proto-1
    ```

## IP路由
IP协议的一个核心任务是数据报的路由，即决定发送数据报到目标机器的路径。

### IP模块工作流程
![ip_module](./pictures/ip_module.png)
* 从右下角开始分析，当IP模块接收到来自数据链路层的IP数据报时，它首先对该数据报的头部做CRC校验，确认无误后就分析其头部的具体信息
    * 如果该IP数据报的头部设置了源站选路选项，则IP模块调用数据报转发子模块来处理该数据报
    * 如果该IP数据报的头部中目标IP地址时本机的某个IP地址，或者是广播地址，即该数据报时发送给本机的，则IP模块就根据数据报头部中的协议字段来决定将它派发给那个上层应用(分用)
* 数据报转发子模块首先检测系统是否允许转发，如果不允许，则丢弃。如果允许，则转发数据报到下一跳路由。
* 上图中虚线箭头显示了路由表更新过程

### 路由机制
* `route`或`netstat`可查看路由表
    ```bash
    > route
    Kernel IP routing table
    Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
    default         _gateway        0.0.0.0         UG    100    0        0 ens33
    default         _gateway        0.0.0.0         UG    101    0        0 ens38
    link-local      0.0.0.0         255.255.0.0     U     1000   0        0 ens33
    172.17.0.0      0.0.0.0         255.255.0.0     U     0      0        0 docker0
    192.168.179.0   0.0.0.0         255.255.255.0   U     100    0        0 ens33
    192.168.179.0   0.0.0.0         255.255.255.0   U     101    0        0 ens38
    ```

![route_table](./pictures/route_table.png)

* 对于给定的目标IP地址，如何匹配路由表中的哪一项？分三步：
    * 查找路由表中和数据报的目标IP是否完全匹配，如果找到，就使用该路由项
    * 具有相同网路ID的网络IP地址
    * 选择默认路由项，通常意味着下一跳路由是网关

### 路由表更新
* `sudo route add -host 192.168.179.1 dev ens38` - 添加主机“192.168.179.1”对应的路由表
    * 发送给“192.168.179.1”的IP数据报将通过网卡ens38直接发送至目标机器的接收网卡
* `sudo route del -host 192.168.179.1 dev ens38` - 删除主机“192.168.179.1”对应的路由表

## 重定向
![icmp](./pictures/icmp.png)

* ICMP重定向报文也能用于更新路由表
* 一般来说，主机只能接收ICMP重定向报文，而路由器只能发送ICMP重定向报文

# TCP协议详解
本章从四个方面讨论TCP协议：
* TCP头部信息 - 用于指定源/目标端口
* TCP状态转移过程
* TCP数据流
    * 交互数据流
    * 成块数据流
* TCP数据流的控制
    * 超时重传
    * 拥塞控制

## TCP服务的特点
TCP协议相对于UDP协议的特点是：
* 面向连接
    * 双方需要管理连接的状态和连接上数据的传输
    * 无连接协议UDP更适合广播和多播
* 字节流
    * 发送端执行的写操作次数和接收端执行的读操作次数之间没有任何数量关系，这就是字节流的概念
    * 下图显示了TCP字节流传输和UDP数据传输的区别
* 可靠传输
    * 应答机制
    * 超时重传机制
    * 对接收到的TCP报文重排整理后才交付给应用层

![tcp_udp_send_recv](./pictures/tcp_udp_send_recv.png)

## TCP头部结构

![tcp_header](./pictures/tcp_header.png)
* 16位端口号(port number)
    * 客户端通常使用系统自动选择的临时端口号
    * 服务器则使用知名的服务端口号，可在/etc/services中找到
* 32位序号(sequence number)
    * 一次TCP通信(从TCP连接建立到断开)过程中某**一个传输方向**上的字节流的每个字节的编号。例如，A发送给B的第一个报文被初始化位某个随机值ISN(Initial Sequence Number)，那么后续A到B再次传输1025~2048字节的TCP报文，此报文的序号值就是"ISN+1025"
* 32位确认号(acknowledgement number)
    * 用于对另一方发送来的TCP报文段的响应，其值是收到的TCP报文段的序号值加1。因此，A发送给B的报文，不仅携带自己的序号，也包含B发送来的TCP报文段的确认号
* 4位头部长度(header length)
    * 单位是32bit字，TCP头部最大长度是60字节
* 6位标志位
    * URG，紧急指针是否有效
    * ACK，是否携带ACK标志
    * PSH，接收端要立即从TCP接收缓冲区中读走数据
    * RST，重新建立连接
    * SYN，请求建立一个连接
    * FIN，通知对方本端要关闭连接了
* 16位窗口大小(window size)
    * 接收方告诉发送方接收缓冲区还能容纳多少字节的数据，这样对方就可以控制发送数据的速度

### TCP头部选项
![tcp_option](./pictures/tcp_option.png)

### 使用tcpdump观察TCP头部信息
在分析IP头部信息的实验中，我们得到了如下二进制报文，其中前20字节是IP头部，后40字节是TCP头部：
```bash
> sudo tcpdump -ntx -i lo port 23
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on lo, link-type EN10MB (Ethernet), capture size 262144 bytes
IP 127.0.0.1.41684 > 127.0.0.1.23: Flags [S], seq 3956412104, win 65495, options [mss 65495,sackOK,TS val 2922181685 ecr 0,nop,wscale 7], length 0
        0x0000:  4510 003c c09e 4000 4006 7c0b 7f00 0001
        0x0010:  7f00 0001 a2d4 0017 ebd2 0ec8 0000 0000
        0x0020:  a002 ffd7 fe30 0000 0204 ffd7 0402 080a
        0x0030:  ae2c f435 0000 0000 0103 0307
```
* tcpdump输出的`Flags[S]`表示该TCP报文段包含SYN标志

![tcp_data](./pictures/tcp_data.png)

## TCP连接的建立和关闭

### 使用tcpdump观察TCP连接的建立和关闭
* 三次握手
    * 55743 -> 23 : SEQ-1632315053
    * 23 -> 55743 : SEQ-848916543, ACK-1632315054
    * 55743 -> 23 : ACK-(ISN+1) // 是相对值，可通过tcpdump -S，打印绝对值
    ```bash
    IP 127.0.0.1.55734 > 127.0.0.1.23: Flags [S], seq 1632315053, win 65495, options [mss 65495,sackOK,TS val 2779934152 ecr 0,nop,wscale 7], length 0
            0x0000:  4510 003c ea0b 4000 4006 529e 7f00 0001
            0x0010:  7f00 0001 d9b6 0017 614b 26ad 0000 0000
            0x0020:  a002 ffd7 fe30 0000 0204 ffd7 0402 080a
            0x0030:  a5b2 6dc8 0000 0000 0103 0307
    IP 127.0.0.1.23 > 127.0.0.1.55734: Flags [S.], seq 848916543, ack 1632315054, win 65483, options [mss 65495,sackOK,TS val 2779934152 ecr 2779934152,nop,wscale 7], length 0
            0x0000:  4500 003c 0000 4000 4006 3cba 7f00 0001
            0x0010:  7f00 0001 0017 d9b6 3299 703f 614b 26ae
            0x0020:  a012 ffcb fe30 0000 0204 ffd7 0402 080a
            0x0030:  a5b2 6dc8 a5b2 6dc8 0103 0307
    IP 127.0.0.1.55734 > 127.0.0.1.23: Flags [.], ack 1, win 512, options [nop,nop,TS val 2779934152 ecr 2779934152], length 0
            0x0000:  4510 0034 ea0c 4000 4006 52a5 7f00 0001
            0x0010:  7f00 0001 d9b6 0017 614b 26ae 3299 7040
            0x0020:  8010 0200 fe28 0000 0101 080a a5b2 6dc8
            0x0030:  a5b2 6dc8
    ```
* 四次挥手(telnet的23端口只有三次，省略了从服务器到客户端的确认报文，所以通过ssh的22端口)
    * 客户端->服务器：发起关闭过程，发送包含FIN标志的TCP包到服务器，SEQ-4586，ACK-4546
    * 服务器->客户端：确认收到客户端的关闭报文，ACK-4587
    * 服务器->客户端：发送关闭报文到客户端，告诉客户端自己以及完成关闭动作，SEQ-4546，ACK-4587
    * 客户端->服务器：确认收到服务器的关闭报文，ACK-4547，结束报文和同步报文一样，也要占用一个序号值
    ```bash
    IP 127.0.0.1.37262 > 127.0.0.1.22: Flags [F.], seq 4586, ack 4546, win 512, options [nop,nop,TS val 2781151112 ecr 2781151111], length 0
            0x0000:  4510 0034 d7b4 4000 4006 64fd 7f00 0001
            0x0010:  7f00 0001 918e 0016 0d25 bfa6 ab2f d17d
            0x0020:  8011 0200 fe28 0000 0101 080a a5c4 ff88
            0x0030:  a5c4 ff87
    IP 127.0.0.1.22 > 127.0.0.1.37262: Flags [.], ack 4587, win 512, options [nop,nop,TS val 2781151112 ecr 2781151112], length 0
            0x0000:  4510 0034 d973 4000 4006 633e 7f00 0001
            0x0010:  7f00 0001 0016 918e ab2f d17d 0d25 bfa7
            0x0020:  8010 0200 fe28 0000 0101 080a a5c4 ff88
            0x0030:  a5c4 ff88
    IP 127.0.0.1.22 > 127.0.0.1.37262: Flags [F.], seq 4546, ack 4587, win 512, options [nop,nop,TS val 2781151117 ecr 2781151112], length 0
            0x0000:  4510 0034 d974 4000 4006 633d 7f00 0001
            0x0010:  7f00 0001 0016 918e ab2f d17d 0d25 bfa7
            0x0020:  8011 0200 fe28 0000 0101 080a a5c4 ff8d
            0x0030:  a5c4 ff88
    IP 127.0.0.1.37262 > 127.0.0.1.22: Flags [.], ack 4547, win 512, options [nop,nop,TS val 2781151117 ecr 2781151117], length 0
            0x0000:  4510 0034 0000 4000 4006 3cb2 7f00 0001
            0x0010:  7f00 0001 918e 0016 0d25 bfa7 ab2f d17e
            0x0020:  8010 0200 50f6 0000 0101 080a a5c4 ff8d
            0x0030:  a5c4 ff8d
    ```

### 半关闭状态
![half_close](./pictures/half_close.png)
* 本端以及完成了数据的发送，但允许继续接收来自对方的数据，这种状态称为半关闭(half-close)状态
* 判断对方是否已经关闭连接的方法
    * `read`系统调用返回0(收到结束报文段)，如果没有关闭只是没有数据，read会阻塞
* `shutdown`函数提供了对半关闭的支持

## TCP状态转移
![tcp_states](./pictures/tcp_states.png)
* 虚线箭头表示服务器动作
* 实线箭头表示客户端动作

### TCP状态转移总图
* 服务器的典型状态转移过程
    * 建立连接
        * 通过`listen`系统调用进入`LISTEN`状态
        * 收到客户端的同步报文段，进入内核等待队列，并向客户端发送带SYN标志的确认报文段，连接处于`SYN_RCVD`状态
        * 收到客户端的确认报文段，进入`ESTABLISHED`状态
    * 关闭连接
        * 客户端通过`close`或`shutdown`系统调用主动关闭连接时，向服务器发送结束段报文，服务器通过返回确认报文段进入`CLOSE_WAIT`状态，等待服务器应用程序关闭连接
        * 服务器端应用程序关闭完成后，向客户端发送一个结束报文，进入`LAST_ACK`状态
        * 收到客户端的确认报文后，连接彻底关闭，进入`CLOSED`状态
* 客户端的典型状态转移过程
    * 建立连接
        * 通过`connect`系统调用，给服务器发送一个同步报文，进入`SYN_SENT`状态
            * 如果目标端口不存在，或者端口被占用，服务器给客户端发送一个复位报文，connect调用失败，进入`CLOSED`状态
            * 如果在超时时间内未收到服务器的确认报文，同样connect调用失败，进入`CLOSED`状态
        * 收到服务器的**同步报文段和确认报文段**，进入`ESTABLISHED`状态
    * 关闭连接
        * 客户端向服务器发送一个结束报文段后，进入`FIN_WAIT_1`状态
        * 收到服务器发送的确认报文段(确认收到结束报文段)后，进入`FIN_WAIT_2`状态，此时服务器处于`CLOSE_WAIT`状态
        * 收到服务器发送的关闭连接，进入`TIME_WAIT`状态
            * `FIN_WAIT_1`状态也可以直接进入`TIME_WAIT`状态，前提是直接收到服务器带确认信息的结束报文段(而不是先收到确认段，再收到结束报文段)
            * 孤儿连接指客户端长时间停留在`FIN_WAIT_2`状态的连接
                * `/proc/sys/net/ipv4/tcp_max_orphans` - 内核能接管的孤儿连接数目
                * `/proc/sys/net/ipv4/tcp_fin_timeout` - 孤儿连接再内核中的生存时间

下图显示了典型的TCP状态转移路线：

![tcp_states_normal](./pictures/tcp_states_normal.png)

### TIME_WAIT状态
在`TIME_WAIT`状态，客户端连接要等待一段长为2MSL(Maximum Segment Life, 报文段最大生存时间, 其值可通过"cat /proc/sys/net/ipv4/tcp_fin_timeout"得到)的时间，才能完全关闭, 原因是:
* 可靠地中止TCP连接
    * 防止服务器无法收到关闭确认报文
* 保证让迟来的TCP报文有足够的时间被识别并丢弃
    * 防止程序建立一个和刚关闭的连接相同的连接，接收到属于原来连接的报文

## 复位报文段
### 访问不存在的端口
```bash
> sudo tcpdump -nt -i lo port 54321
> telnet 127.0.0.1 54321
tcpdump: verbose output suppressed, use -v or -vv for full protocol decode
listening on lo, link-type EN10MB (Ethernet), capture size 262144 bytes
IP 127.0.0.1.34448 > 127.0.0.1.54321: Flags [S], seq 2402185477, win 65495, options [mss 65495,sackOK,TS val 2792730020 ecr 0,nop,wscale 7], length 0
IP 127.0.0.1.54321 > 127.0.0.1.34448: Flags [R.], seq 0, ack 2402185478, win 0, length 0
```

### 异常终止连接
应用程序可以使用socket选项`SO_LINGER`来发送复位报文段，以异常终止一个连接。

### 处理半打开连接
如果客户端(或服务器)往处于半打开状态的连接写入数据，对方将回应一个复位报文段。

## TCP交互数据流
通过TCP连接交换的应用程序数据，按照长度分为：
* 交互数据
    * 仅包含很少的字节，对实时性要求高，比如telnet、ssh等。
* 成块数据
    * 对传输效率要求高，比如ftp

```bash
> sudo tcpdump -nt -i lo port 23
# 客户端向服务器发送1个字节的应用程序数据："l"
IP 127.0.0.1.56048 > 127.0.0.1.23: Flags [P.], seq 178:179, ack 2544, win 512, options [nop,nop,TS val 2793706867 ecr 2793686126], length 1
# 服务器对上面报文的确认，同时回显字母"l"
IP 127.0.0.1.23 > 127.0.0.1.56048: Flags [P.], seq 2544:2545, ack 179, win 512, options [nop,nop,TS val 2793706867 ecr 2793706867], length 1
# 客户端对上面服务器报文的确认
IP 127.0.0.1.56048 > 127.0.0.1.23: Flags [.], ack 2545, win 512, options [nop,nop,TS val 2793706867 ecr 2793706867], length 0
# 客户端向服务器发送1个字节的应用程序数据："s"
IP 127.0.0.1.56048 > 127.0.0.1.23: Flags [P.], seq 179:180, ack 2545, win 512, options [nop,nop,TS val 2793735273 ecr 2793706867], length 1
IP 127.0.0.1.23 > 127.0.0.1.56048: Flags [P.], seq 2545:2546, ack 180, win 512, options [nop,nop,TS val 2793735273 ecr 2793735273], length 1
IP 127.0.0.1.56048 > 127.0.0.1.23: Flags [.], ack 2546, win 512, options [nop,nop,TS val 2793735273 ecr 2793735273], length 0
# 客户端向服务器发送1个字节的应用程序数据：回车符和流结束符EOF
IP 127.0.0.1.56048 > 127.0.0.1.23: Flags [P.], seq 193:195, ack 2650, win 512, options [nop,nop,TS val 2794004738 ecr 2794000323], length 2
IP 127.0.0.1.23 > 127.0.0.1.56048: Flags [P.], seq 2650:2652, ack 195, win 512, options [nop,nop,TS val 2794004738 ecr 2794004738], length 2
IP 127.0.0.1.56048 > 127.0.0.1.23: Flags [.], ack 2652, win 512, options [nop,nop,TS val 2794004738 ecr 2794004738], length 0
# 服务器向客户端发送客户查询的目录的内容(ls命令的输出)
IP 127.0.0.1.23 > 127.0.0.1.56048: Flags [P.], seq 2652:2992, ack 195, win 512, options [nop,nop,TS val 2794004740 ecr 2794004738], length 340
IP 127.0.0.1.56048 > 127.0.0.1.23: Flags [.], ack 2992, win 510, options [nop,nop,TS val 2794004740 ecr 2794004740], length 0
# 服务器向客户端发送：一个回车符、一个换行符、客户端登录用户的PS1环境变量
IP 127.0.0.1.23 > 127.0.0.1.56048: Flags [P.], seq 2992:3076, ack 195, win 512, options [nop,nop,TS val 2794004740 ecr 2794004740], length 84
IP 127.0.0.1.56048 > 127.0.0.1.23: Flags [.], ack 3076, win 510, options [nop,nop,TS val 2794004740 ecr 2794004740], length 0
```
* 由于服务器对客户请求处理很快，所以它发送确认报文段的时总是有数据一起发送(延时确认)
* 而用户输入速度明显慢于客户端程序的处理速度，所以客户端的确认报文段总是不携带任何用用程序数据

## TCP成块数据流
* 服务器端
    ```bash
    > sudo tcpdump -nt -i lo port 21
    IP 127.0.0.1.45646 > 127.0.0.1.21: Flags [P.], seq 131:139, ack 442, win 512, options [nop,nop,TS val 2798958144 ecr 2798930799], length 8: FTP: TYPE I
    IP 127.0.0.1.21 > 127.0.0.1.45646: Flags [P.], seq 442:473, ack 139, win 512, options [nop,nop,TS val 2798958144 ecr 2798958144], length 31: FTP: 200 Switching to Binary mode.
    IP 127.0.0.1.45646 > 127.0.0.1.21: Flags [.], ack 473, win 512, options [nop,nop,TS val 2798958144 ecr 2798958144], length 0
    IP 127.0.0.1.45646 > 127.0.0.1.21: Flags [P.], seq 139:163, ack 473, win 512, options [nop,nop,TS val 2798958144 ecr 2798958144], length 24: FTP: PORT 127,0,0,1,207,103
    IP 127.0.0.1.21 > 127.0.0.1.45646: Flags [P.], seq 473:524, ack 163, win 512, options [nop,nop,TS val 2798958145 ecr 2798958144], length 51: FTP: 200 PORT command successful. Consider using PASV.
    IP 127.0.0.1.45646 > 127.0.0.1.21: Flags [.], ack 524, win 512, options [nop,nop,TS val 2798958145 ecr 2798958145], length 0
    IP 127.0.0.1.45646 > 127.0.0.1.21: Flags [P.], seq 163:177, ack 524, win 512, options [nop,nop,TS val 2798958145 ecr 2798958145], length 14: FTP: RETR p4v.tgz
    IP 127.0.0.1.21 > 127.0.0.1.45646: Flags [P.], seq 524:596, ack 177, win 512, options [nop,nop,TS val 2798958145 ecr 2798958145], length 72: FTP: 150 Opening BINARY mode data connection for p4v.tgz (125781581 bytes).
    IP 127.0.0.1.45646 > 127.0.0.1.21: Flags [.], ack 596, win 512, options [nop,nop,TS val 2798958145 ecr 2798958145], length 0
    IP 127.0.0.1.21 > 127.0.0.1.45646: Flags [P.], seq 596:620, ack 177, win 512, options [nop,nop,TS val 2798960837 ecr 2798958145], length 24: FTP: 226 Transfer complete.
    IP 127.0.0.1.45646 > 127.0.0.1.21: Flags [.], ack 620, win 512, options [nop,nop,TS val 2798960837 ecr 2798960837], length 0
    ```
* 客户端
    ```bash
    > ftp 127.0.0.1
    ftp> get p4v.tgz
    local: p4v.tgz remote: p4v.tgz
    200 PORT command successful. Consider using PASV.
    150 Opening BINARY mode data connection for p4v.tgz (125781581 bytes).
    226 Transfer complete.
    125781581 bytes received in 2.75 secs (43.5517 MB/s)
    ```

## TCP超时重传
```bash
16:34:04.969902 IP 127.0.0.1.40286 > 127.0.0.1.5001: Flags [S], seq 3144583604, win 65495, options [mss 65495,sackOK,TS val 2799914105 ecr 0,nop,wscale 7], length 0
16:34:04.969916 IP 127.0.0.1.5001 > 127.0.0.1.40286: Flags [S.], seq 3922255266, ack 3144583605, win 65483, options [mss 65495,sackOK,TS val 2799914105 ecr 2799914105,nop,wscale 7], length 0
16:34:04.969931 IP 127.0.0.1.40286 > 127.0.0.1.5001: Flags [.], ack 1, win 512, options [nop,nop,TS val 2799914105 ecr 2799914105], length 0
# 客户端发送“1234”到“perf -s”开启的服务器，长度是6，包括回车、换行两个字符
16:34:29.271590 IP 127.0.0.1.40286 > 127.0.0.1.5001: Flags [P.], seq 1:7, ack 1, win 512, options [nop,nop,TS val 2799938406 ecr 2799914105], length 6
16:34:29.271600 IP 127.0.0.1.5001 > 127.0.0.1.40286: Flags [.], ack 7, win 512, options [nop,nop,TS val 2799938406 ecr 2799938406], length 0
```
* `/proc/sys/net/ipv4/tcp_retries1`
    * 在底层IP接管之前TCP最少执行的重传次数，默认是3
* `/proc/sys/net/ipv4/tcp_retries2`
    * 连接放弃前TCP最多可以执行的重传次数，默认是15

## 拥塞控制
拥塞控制的四个部分：
* 慢启动
    * TCP模块刚开始发送数据时并不知道网络的实际情况，需要用一种试探的方式平滑地增加CWND的大小
* 拥塞避免
    * 当CWND的大小超过门限时，进入拥塞避免阶段，减缓CWND的膨胀
* 快速重传
    * 发送端如果连续收到3个重复的确认报文段，就认为拥塞发生了，需要启动快速重传/恢复来处理拥塞
* 快速恢复

常见拥塞控制算法有(可通过"/proc/sys/net/ipv4/tcp_congestion_control"查看)：
* reno
* vegas
* cubic

![congestion](./pictures/congestion.png)
* SMSS（Sender Maximum Segment Size）
* SWND (Send Window)
    * 太小会引起网络延时
    * 太大会导致网络拥塞
* RWND (Receive Window)
    * 接收方可通过其接收通告窗口RWND来控制发送端的SWND
* CWND (Congestion Window)

# TCP/IP通信案例：访问Internet上的Web服务器
代理服务器分为：
* 正向代理服务器
    * 要求客户端自己设置代理服务器的地址，客户的每次请求都将直接发送到该代理服务器
* 反向代理服务器
    * 客户端无须任何设置，代理服务器接收Internet上的连接请求，然后将请求转发给内部网络上的服务器，并将从内部服务器上得到的结果返回给客户端
* 透明代理服务器
    * 只能设置在网关上，可以看作正向代理的一种特殊情况

![proxy_server](./pictures/proxy_server.png)

## HTTP通信
![wget_squid_packets](./pictures/wget_squid_packets.png)
* 客户端只给服务器发送了一个HTTP请求(TCP报文段4)，请求136字节
* 代理服务器用6个TCP报文段给客户端返回了总长度为8522字节的HTTP应答
* 客户端用7个TCP报文段应答了这8522字节

### HTTP请求
![http_request](./pictures/http_request.png)

### HTTP应答
![http_reply](./pictures/http_reply.png)

# Linux网络编程基础API

## socket地址API

### 主机字节序和网络字节序
* 小端字节序
    * 低位字节存在低地址
* 主机字节序
    * 现代PC大多采用小端字节序，因此小端字节序又被称为主机字节序
* 网络字节序
    * 大端字节序也称为网络字节序

### 通用socket地址
socket地址的结构体`sockaddr`定义如下：
```c
#include <bits/socket.h>
struct sockaddr
{
    sa_family_t sa_family;
    char sa_data[14];
}
```

![sa_and_protocol_family](./pictures/sa_and_protocol_family.png)

![protocol_family_val](./pictures/protocol_family_val.png)

* `sa_family_t`是地址族类型，与之对应的还有协议族(protocol family)，两者相同，可以混用
    * `PF_*`和`AF_*`定义相同，可以混用
* `sa_data`用于存放socket地址值，具体内容参见上表
    * 为了容纳IPv6地址(26字节)，Linux定义了新的结构体`sockaddr_storage`

### 专用socket地址
Linux为各个协议族提供了专门的socket地址结构体。但是，所有专用socket地址类型在使用过程中都要转换为通用socket地址类型`sockaddr`，因为所有socket编程接口使用的地址参数的类型都是`sockaddr`。
* UNIX本地域协议族
    ```cpp
    #include <sys/un.h>
    struct sockaddr_un
    {
        sa_family_t sin_family; // 地址族：AF_UNIX
        char sun_path[108];     // 文件路径名
    };
    ```
* TCP/IP协议族
    ```cpp
    struct sockaddr_in
    {
        sa_family_t sin_family; // 地址族：AF_INET
        u_int16_t sin_port;     // 端口号，要用网络字节序表示
        struct in_addr sin_addr;// IPv4地址结构体
    };
    struct in_addr
    {
        u_int32_t s_addr;       // IPv4地址，要用网络字节序表示
    };

    struct sockaddr_in6
    {
        sa_family_t sin6_family;// 地址族: AF_INET6
        u_int16_t sin6_port;    // 端口号，要用网络字节序
        u_int32_t sin6_flowinfo;// 流信息，应设置为0
        struct in6_addr sin6_addr;
        u_int32_t sin6_scope_id;// scope ID
    };
    struct in6_addr
    {
        unsigned char sa_addr[16];  // IPv6地址，要用网络字节序
    };
    ```

### IP地址转换函数
下面几个函数可以用于IPv4地址字符串和整数表示之间的转换：
```cpp
#include <arpa/inet.h>
in_addr_t inet_addr(const char* strptr); // 将用点分十进制字符串的IPv4地址转换为网络字节序整数表示的地址
int inet_aton(const char* cp, struct in_addr* inp); // 和inet_addr功能一样，但将结果存于inp指向的结构中，成功返回1
char* inet_ntoa(struct in_addr in); // 将网络字节序整数表示的IPv4地址转化为用点分十进制字符串的IPv4地址
int inet_pton(int af, const char* src, void* dst);
const char* inet_ntop(int af, const void* src, char* dst, socklen_t cnt); // cnt指定目标存储单元的大小
```
* `inet_ntoa`内部静态变量存了转化结果，所以不可重入，可参考[例子](./code/socket/addr/main.cpp)
* [例子](./code/socket/pton/main.cpp)显示了`inet_pton`和`inet_ntop`的使用方法

## 创建socket
```cpp
#include <sys/types.h>
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
```
* `domain`告诉系统使用哪个底层协议族
    * IPv4: PF_INET
    * IPv6: PF_INET6
    * UNIX本地协议族：PF_UNIX
* `type`用于指定服务类型
    * `SOCK_STREAM`，流服务，用于TCP/IP协议
    * `SOCK_UGRAM`，数据报服务，用于UDP协议
    * `SOCK_NONBLOCK`，非阻塞
    * `SOCK_CLOEXEC`，用fork调用创建子进程时在子进程中关闭该socket
* `protocol`，具体的协议，几乎都是0，表示默认协议

## 命名socket
```cpp
#include <sys/types.h>
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr* my_addr, socklen_t addrlen);
```
* socket命名
    * 在创建socket时，我们给它指定了地址族，但是并未指定使用该地址族中的哪个具体socket地址。将一个socket与socket地址绑定称为给socket命名。
    * 客户端通常不需要命名socket，采用匿名方式，即使用操作系统自动分配的socket地址
* `bind`将`my_addr`所指的socket地址分配给未命名的sockfd文件描述符，`addrlen`参数指出该socket地址的长度
    * 返回0，成功
    * errno == EACCES，表示地址受保护
    * errno == EADDRINUSE，表示绑定地址正在使用中

## 监听socket
```cpp
#include <sys/socket.h>
int listen(int sockfd, int backlog);
```
* `listen`用户创建一个监听队列以存放待处理的客户连接
* `sockfd`指定被监听的socket
* `backlog`表示内核监听队列的最大长度
    * 参见backlog的[例子](./code/socket/listen/main.cpp)

## 接受连接
```cpp
#include <sys/types.h>
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
* `accept`从listen监听队列中接受一个连接
* `sockfd`是执行过`listen`系统调用的监听socket
* `addr`用来获取被接受连接的远端socket地址
    * 该socket地址长度由`addrlen`指出
* 成功时返回一个新的连接socket，服务器可通过读写该socket来与被接受连接对应的客户端通信
* `accept`只是从监听队列中取出连接，不论连接处于何种状态，更不关心任何网络状况的变化
* 参见[例子](./code/socket/accept/main.cpp)

## 发起连接



