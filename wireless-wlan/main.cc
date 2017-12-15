#include "AccessPoint.h"
#include "Switch.h"
#include <iostream>

NS_LOG_COMPONENT_DEFINE("WLAN");

using namespace std;
int main(int argc, char *argv[]){
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

	NodeContainer apNodes;
	apNodes.Create(3);
	NetDeviceContainer apDevices;

	Ipv4AddressHelper address;
	//AccessPoint is a class that manage a accessPoint and some STA node under this accesspoint
	AccessPoint ap1 = AccessPoint(3,1,address,apNodes.Get(0),"10.1.1.0", "255.255.255.0");
	AccessPoint ap2 = AccessPoint(3,2,address,apNodes.Get(1),"10.1.2.0", "255.255.255.0");
	AccessPoint ap3 = AccessPoint(3,3,address,apNodes.Get(2),"10.1.3.0", "255.255.255.0");

	NodeContainer csmaSwitch;
	 csmaSwitch.Create (1);        //创建一个网桥
	 CsmaHelper csma;//CsmaHelper类，帮助设置设备和信道属性。
	 csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));//每条与网桥连接链路属性设置，数据率有Channel属性指定，而非Device属性指定。
	 csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));

	 NetDeviceContainer switchDevices;  //网桥设备

	 NetDeviceContainer link1 = csma.Install (NodeContainer (apNodes.Get(0), csmaSwitch)); //有两个节点的容器，为它们安装csma设备和信道，它们属于一个局域网
	 apDevices.Add (link1.Get (0));//将对应设备添加到终端设备，设备的编号是按这个安装先后顺序编号。
	 switchDevices.Add (link1.Get (1));//添加到网桥

	 NetDeviceContainer link2 = csma.Install (NodeContainer (apNodes.Get(1), csmaSwitch)); //有两个节点的容器，为它们安装csma设备和信道，它们属于一个局域网
	 apDevices.Add (link2.Get (0));//将对应设备添加到终端设备，设备的编号是按这个安装先后顺序编号。
	 switchDevices.Add (link2.Get (1));//添加到网桥

	 NetDeviceContainer link3 = csma.Install (NodeContainer (apNodes.Get(2), csmaSwitch)); //有两个节点的容器，为它们安装csma设备和信道，它们属于一个局域网
	 apDevices.Add (link3.Get (0));//将对应设备添加到终端设备，设备的编号是按这个安装先后顺序编号。
	 switchDevices.Add (link3.Get (1));//添加到网桥

	 Ptr<Node> switchNode = csmaSwitch.Get (0);//获取网桥节点
	   BridgeHelper bridge;
	   bridge.Install (switchNode, switchDevices);//安装网桥到网桥节点

	InternetStackHelper stack;
	stack.Install (ap1.getWifiApNode());
	stack.Install (ap1.getWifiStaNodes());
	stack.Install (ap2.getWifiApNode());
	stack.Install (ap2.getWifiStaNodes());
	stack.Install (ap3.getWifiApNode());
	stack.Install (ap3.getWifiStaNodes());

	address.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer p2pInterfaces;
	p2pInterfaces = address.Assign (apDevices);

	address.SetBase ("10.1.2.0", "255.255.255.0");				//server
	Ipv4InterfaceContainer serverWifiInterfaces;
	serverWifiInterfaces=address.Assign (ap1.getStaDevices());
	address.Assign (ap1.getApDevices());

	address.SetBase ("10.1.3.0", "255.255.255.0");				//client
	address.Assign (ap2.getStaDevices());
	address.Assign (ap2.getApDevices());

	address.SetBase ("10.1.4.0", "255.255.255.0");				//client
	address.Assign (ap3.getStaDevices());
	address.Assign (ap3.getApDevices());

	UdpEchoServerHelper echoServer (9);
	ApplicationContainer serverApps = echoServer.Install (ap1.getWifiStaNodes().Get (2));
	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (10.0));

	UdpEchoClientHelper echoClient1 (serverWifiInterfaces.GetAddress (2), 9);//the third node 10.1.2.3
	echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
	echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

	ApplicationContainer clientApps1 =
	echoClient1.Install (ap2.getWifiStaNodes().Get (2));				//the third node 10.1.3.3
	clientApps1.Start (Seconds (4.0));
	clientApps1.Stop (Seconds (10.0));

	UdpEchoClientHelper echoClient2 (serverWifiInterfaces.GetAddress (2), 9);//the third node 10.1.2.3
	echoClient2.SetAttribute ("MaxPackets", UintegerValue (2));
	echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

	ApplicationContainer clientApps2 =
	echoClient2.Install (ap3.getWifiStaNodes().Get (2));				//the third node 10.1.3.3
	clientApps2.Start (Seconds (2.0));
	clientApps2.Stop (Seconds (10.0));

	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	Simulator::Stop (Seconds (10.0));

	AnimationInterface anim("anim4.xml");
	anim.SetConstantPosition(apNodes.Get(1),10,10);
	anim.SetConstantPosition(apNodes.Get(2),20,10);
	anim.SetConstantPosition(csmaSwitch.Get(0),10,20);
	anim.UpdateNodeColor(apNodes.Get(0),0,255,0);
	anim.UpdateNodeColor(apNodes.Get(1),0,255,0);
	anim.UpdateNodeColor(apNodes.Get(2),0,255,0);
	anim.UpdateNodeColor(csmaSwitch.Get(0),0,0,255);
	anim.AddSourceDestination(0,"10.1.2.3");
	anim.EnablePacketMetadata (); // Optional
	anim.EnableIpv4RouteTracking ("routingtable-wireless3.xml", Seconds (0), Seconds (5), Seconds (0.25)); //Optional
	anim.EnableWifiMacCounters (Seconds (0), Seconds (10)); //Optional
	anim.EnableWifiPhyCounters (Seconds (0), Seconds (10)); //Optional
	Simulator::Run ();
	Simulator::Destroy ();
	return 0;

}
