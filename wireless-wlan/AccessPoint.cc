#include "AccessPoint.h"

AccessPoint::AccessPoint(){

}


AccessPoint::AccessPoint(uint16_t deviceNum,uint16_t id,Ipv4AddressHelper address,Ptr<Node> apNode,
		std::string netBase,std::string subnetMask){
	wifiStaNodes.Create(deviceNum);
	wifiApNode = NodeContainer(apNode);
	//wifiApNode = apNodeContainer;
	Ssid ("ns-3-"+id);
	init();
}


void AccessPoint::init(){
	channel = YansWifiChannelHelper::Default ();
	phy = YansWifiPhyHelper::Default ();
	phy.EnablePcapAll("left",true);
	phy.SetChannel (channel.Create ());
	wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
	mac.SetType ("ns3::StaWifiMac",
					   "Ssid", SsidValue (ssid),
					   "ActiveProbing", BooleanValue (false));
	staDevices = wifi.Install (phy, mac, wifiStaNodes);
	mac.SetType ("ns3::ApWifiMac",
					   "Ssid", SsidValue (ssid));
	apDevices = wifi.Install (phy, mac, wifiApNode);
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

}

NodeContainer& AccessPoint::getWifiApNode(){
	return wifiApNode;
}

NodeContainer& AccessPoint::getWifiStaNodes(){
	return wifiStaNodes;
}

NetDeviceContainer& AccessPoint::getStaDevices(){
	return staDevices;
}

NetDeviceContainer& AccessPoint::getApDevices(){
	return apDevices;
}
