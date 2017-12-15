/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("EighthScriptExample");

int main(int argc,char *argv[]){
	bool verbose = true;
	uint32_t nLwifi=3;
	uint32_t nRwifi=3;

	CommandLine cmd;
	cmd.AddValue ("nLwifi", "Number of left wifi STA devices", nLwifi);
	cmd.AddValue ("nRwifi", "Number of right wifi STA devices", nRwifi);
	cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
	cmd.Parse(argc,argv);

	if(nLwifi>18||nRwifi>18){
		std::cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box" << std::endl;
		return 1;
	}
	if(verbose){
		LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
		LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
	}

	NodeContainer p2pNodes;
	p2pNodes.Create(2);

	PointToPointHelper pointToPoint;
	pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

	NetDeviceContainer p2pDevices;
	p2pDevices = pointToPoint.Install (p2pNodes);

	NodeContainer wifiStaNodes;
	  wifiStaNodes.Create (nLwifi);
	  NodeContainer wifiApNode = p2pNodes.Get (0);

	  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
	  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
	  phy.EnablePcapAll("left",true);
	  phy.SetChannel (channel.Create ());

	  WifiHelper wifi;
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






	  NodeContainer wifiRightStaNodes;
	  	  wifiRightStaNodes.Create (nRwifi);
	  	  NodeContainer wifiRightApNode = p2pNodes.Get (1);

	  	  YansWifiChannelHelper channelRight = YansWifiChannelHelper::Default ();
	  	  YansWifiPhyHelper phyRight = YansWifiPhyHelper::Default ();
	  	  phyRight.EnablePcapAll("rightwifi",true);
	  	  phyRight.SetChannel (channelRight.Create ());

	  	  WifiHelper wifiRight;
	  	  wifiRight.SetRemoteStationManager ("ns3::AarfWifiManager");

	  	  WifiMacHelper macRight;
	  	  Ssid ssid2 = Ssid ("ns-3-ssid2");
	  	  macRight.SetType ("ns3::StaWifiMac",
	  				   "Ssid", SsidValue (ssid2),
	  				   "ActiveProbing", BooleanValue (false));

	  	  NetDeviceContainer staDevicesRight;
	  	  staDevicesRight = wifiRight.Install (phyRight, macRight, wifiRightStaNodes);

	  	  macRight.SetType ("ns3::ApWifiMac",
	  				   "Ssid", SsidValue (ssid2));

	  	  NetDeviceContainer apDevicesRight;
	  	  apDevicesRight = wifiRight.Install (phyRight, macRight, wifiRightApNode);

	  	  MobilityHelper mobilityRight;

	  	  mobilityRight.SetPositionAllocator ("ns3::GridPositionAllocator",
	  									 "MinX", DoubleValue (0.0),
	  									 "MinY", DoubleValue (0.0),
	  									 "DeltaX", DoubleValue (5.0),
	  									 "DeltaY", DoubleValue (10.0),
	  									 "GridWidth", UintegerValue (3),
	  									 "LayoutType", StringValue ("RowFirst"));

	  	  mobilityRight.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
	  								 "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
	  	  mobilityRight.Install (wifiRightStaNodes);

	  	  mobilityRight.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	  	  mobilityRight.Install (wifiRightApNode);


	  	InternetStackHelper stack;
	  	  stack.Install (wifiApNode);
	  	  stack.Install (wifiStaNodes);
	  	  stack.Install(wifiRightApNode);
	  	  stack.Install(wifiRightStaNodes);

	  	Ipv4AddressHelper address;

	  	address.SetBase ("10.1.1.0", "255.255.255.0");
	  	  Ipv4InterfaceContainer p2pInterfaces;
	  	  p2pInterfaces = address.Assign (p2pDevices);

	  	address.SetBase ("10.1.2.0", "255.255.255.0");
	  	 Ipv4InterfaceContainer serverWifiInterfaces;
	  	serverWifiInterfaces=address.Assign (staDevices);
	  	address.Assign (apDevices);

	  	address.SetBase ("10.1.3.0", "255.255.255.0");
	  	  address.Assign (staDevicesRight);
	  	  address.Assign (apDevicesRight);

	  	 UdpEchoServerHelper echoServer (9);
	  	ApplicationContainer serverApps = echoServer.Install (wifiStaNodes.Get (nLwifi - 1));
	  	  serverApps.Start (Seconds (1.0));
	  	  serverApps.Stop (Seconds (10.0));

	  	  UdpEchoClientHelper echoClient (serverWifiInterfaces.GetAddress (nLwifi-1), 9);
	  	  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
	  	  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	  	  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

	  	ApplicationContainer clientApps =
	  	    echoClient.Install (wifiRightStaNodes.Get (nRwifi - 1));
	  	  clientApps.Start (Seconds (2.0));
	  	  clientApps.Stop (Seconds (10.0));

	  	  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	  	  Simulator::Stop (Seconds (10.0));

	  	  AnimationInterface anim("anim1.xml");
	  	  anim.SetConstantPosition(p2pNodes.Get(1),10,10);
	  	  anim.UpdateNodeColor(p2pNodes.Get(0),0,255,0);
	  	  anim.UpdateNodeColor(p2pNodes.Get(1),0,255,0);
	  	  anim.AddSourceDestination(0,"10.1.2.3");
	  	  anim.EnablePacketMetadata (); // Optional
	  	  anim.EnableIpv4RouteTracking ("routingtable-wireless.xml", Seconds (0), Seconds (5), Seconds (0.25)); //Optional
	  	  anim.EnableWifiMacCounters (Seconds (0), Seconds (10)); //Optional
	  	  anim.EnableWifiPhyCounters (Seconds (0), Seconds (10)); //Optional
	  	  //anim.SetConstantPosition(wifiStaNodes.Get(0),30,10);
	  	  //anim.SetConstantPosition(wifiStaNodes.Get(1),40,10);
	  	  //anim.SetConstantPosition(wifiStaNodes.Get(2),50,10);
	  	  //anim.EnablePacketMetadata(True)
	  	  Simulator::Run ();
	  	  Simulator::Destroy ();
	  	  return 0;
}
