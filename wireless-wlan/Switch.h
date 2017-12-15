#include "self-wireless-module.h"

using namespace ns3;
class Switch:public Node{
	private:
		NodeContainer csmaSwitch;
		CsmaHelper csma;			//CsmaHelper类，帮助设置设备和信道属性。
		NetDeviceContainer switchDevices;  //网桥设备
	public:
		Switch();
		Switch(NodeContainer& apSet);
		void init();

};
