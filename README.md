# NS3网络模拟实验
##   &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;  ------ 同济大学计算机网络实验课程实验报告
### 0.背景介绍
NS系列软件是网络模拟的一整套软件，它摆脱了实际设备的约束，用代码来生成网络拓扑结构，来进行网络实现的模拟仿真，来研究网络的优化算法或者优化结构等等。不需要投入过多的设备或者人力进行实际设备的铺设，NS系列的出现让研究者能更多地关注到网络结构的逻辑实现，从而提高了效率。另外，要值得一提的是，NS3是完全不同与NS2的新结构，NS3中有很多内容在NS2中是不支持的，所以移植也存在一定的困难，而且NS3支持C++和Python的开发，NS2使用C++和Otcl开发。所以，这次的计算机网络实验我们选择用NS3来模拟实验，摆脱实验室的硬件困扰。
### 1. NS3实验环境的搭建
首先，最主要的内容就是去NS3的官网(https://www.nsnam.org)去下载ns3-allinone的压缩包，获得源码。NS3对Linux的系统支持较好，所以选择了ubuntu desktop 16.04,接着打开终端，分别解压源文件和下载编译所需的工具，具体代码如下
```
tar -xvf ns-allinone-3.27.tar.bz2

sudo apt-get install gcc g++ python python-dev mercurial bzr gdb 
valgrind gsl-bin libgsl0-dev libgsl2 flex bison tcpdump sqlite 
sqlite3 libsqlite3-dev libxml2 libxml2-dev libgtk2.0-0 libgtk2.0-dev 
uncrustify doxygen graphviz imagemagick texlive texlive-latex-extra 
texlive-generic-extra texlive-generic-recommended texinfo dia texlive 
texlive-latex-extra texlive-extra-utils texlive-generic-recommended 
texi2html python-pygraphviz python-kiwi python-pygoocanvas 
libgoocanvas-dev python-pygccxml
```
接着进入到ns-allinone的根目录下面，输入以下命令./build.py --enable-examples --enable-tests 经过很久很久很久的编译之后，如果没有错误信息的出现的话，说明环境已经搭好啦。另外的话，ns-allinone中还提供了netanim的软件，以提供网络实验可视化的支持。如果对这个软件有兴趣的话，具体的内容需要去youtube上面去看印度人的tutorial,印度人嘛，口音有点重，不过讲的是真不错，附上网址(https://www.youtube.com/watch?v=HRAjKRrvgh4&list=PLRAV69dS1uWQEbcHnKbLldvzrjdOcOIdY&index=5)该视频还有教学如何用eclipse来配置开发环境。

### 2. 开发Version1
```
NodeContainer p2pNodes;                         //两个access point 用p2p通信
    p2pNodes.Create(2);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));     //数据流的速率是5Mb/s
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));         //传输的延迟是2MS

    NetDeviceContainer p2pDevices;                                           //在两个node上要装上p2pDevice来模拟p2p点对点通信
    p2pDevices = pointToPoint.Install (p2pNodes);

    NodeContainer wifiStaNodes;                              //这个wifiStaNodes对应的就是每个ap下面的可移动点
      wifiStaNodes.Create (nLwifi);
      NodeContainer wifiApNode = p2pNodes.Get (0);

      YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
      YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
      phy.EnablePcapAll("left",true);
      phy.SetChannel (channel.Create ());

      WifiHelper wifi;                                                      //创建一个wifiHelper
      wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

      WifiMacHelper mac;
      Ssid ssid = Ssid ("ns-3-ssid");
      mac.SetType ("ns3::StaWifiMac",
                   "Ssid", SsidValue (ssid),
                   "ActiveProbing", BooleanValue (false));

      NetDeviceContainer staDevices;
      staDevices = wifi.Install (phy, mac, wifiStaNodes);

      mac.SetType ("ns3::ApWifiMac",
                   "Ssid", SsidValue (ssid));

      NetDeviceContainer apDevices;
      apDevices = wifi.Install (phy, mac, wifiApNode);

      MobilityHelper mobility;

      mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                     "MinX", DoubleValue (0.0),
                                     "MinY", DoubleValue (0.0),
                                     "DeltaX", DoubleValue (5.0),
                                     "DeltaY", DoubleValue (10.0),
                                     "GridWidth", UintegerValue (3),
                                     "LayoutType", StringValue ("RowFirst"));

      mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                                 "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
      mobility.Install (wifiStaNodes);

      mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
      mobility.Install (wifiApNode);
```
第一个版本的具体内容就是参考了tutorial里面的third.cc的内容，在third中是设置了一个accesspoint和csma的局域网进行通信，我改造成了两个AP进行通信。以上的代码是其中一边的AP以及其下所有可移动点的设置。以下是输出的信息
![](https://github.com/fangweihao123/Photo-Repo/raw/master/version1output.png)
### 3. 开发Version2
第二版本是我觉得第一版只能进行点对点通信局限性太大了，不符合实际无线网络的情况，所以，模拟教材中的基础性无线局域网的结构，将所有AP形成一个局域网，可以进行多个AP之间的通信。并且将代码进行了重构，写了一个AP的类。
```
class AccessPoint:public Node{
    private:
        NodeContainer wifiApNode;
        NodeContainer wifiStaNodes;
        YansWifiChannelHelper channel;
        YansWifiPhyHelper phy;
        WifiHelper wifi;
        WifiMacHelper mac;
        NetDeviceContainer staDevices;
        NetDeviceContainer apDevices;
        MobilityHelper mobility;
        Ssid ssid;
    public:
        AccessPoint();
        AccessPoint(uint16_t deviceNum,uint16_t id,Ipv4AddressHelper address,Ptr<Node> apNode,
                std::string netBase,std::string subnetMask);
        void init();
        NodeContainer& getWifiApNode();
        //NodeContainer& getWifiStaNode(uint16_t deviceId);
        NodeContainer& getWifiStaNodes();
        NetDeviceContainer& getStaDevices();
        NetDeviceContainer& getApDevices();

};
......
AccessPoint ap1 = AccessPoint(3,1,address,apNodes.Get(0),"10.1.1.0", "255.255.255.0");  //这行代码可以创建一个新的AP对象
```
以下是输出的信息<br>
![](https://github.com/fangweihao123/Photo-Repo/raw/master/PC9%5DUF%60%24LU%7DW%7B1T80527W2C.png)
### 4. NS3架构分析
![](https://github.com/fangweihao123/Photo-Repo/raw/master/IMG_8532.JPG)
![](https://github.com/fangweihao123/Photo-Repo/raw/master/IMG_8533.JPG
)
<br>
整个NS3的核心是node这个对象，作为最抽象的基类，node作为结点分布在node二叉树上，在每一次的run()函数中，遍历整个树，进行对应的操作，在一次run结束之后，时间的属性加一，从而实现了时间的概念。
### 5. 个人心得
一开始我们小组选择这个项目是纯属意外，因为一直困扰于实验室近乎于玄学的实验设备，有时候上一次做实验的时候静态路由还可以配备，下一节课静态路由配置就出了问题，完全不知道从何下手。所以，我们选择了老师推荐的NS3模拟实验，完全脱离了硬件，通过软件来实现现实中的模拟。感觉收获很多，对于linux系统有更多的了解了，对于一些开源软件的环境配置也有了自己的理解。对代码的不断重构，让我们的最终效果实现的很好，总而言之，这个实验课程收获很多，不仅对于老师上课说的很多抽象概念有了实质性的理解，而且也提升了我们的动手能力。