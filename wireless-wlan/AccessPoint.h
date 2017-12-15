#include "self-wireless-module.h"

using namespace ns3;
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
