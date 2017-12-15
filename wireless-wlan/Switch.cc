#include "Switch.h"

Switch::Switch(){

}

Switch::Switch(NodeContainer& apSet){
	 uint32_t nNodes = apSet.GetN ();
	 for (uint32_t i = 0 ;i < nNodes; ++i){
		 //NetDeviceContainer link = csma.Install (NodeContainer (terminals.Get (i), csmaSwitch)); //有两个节点的容器，为它们安装csma设备和信道，它们属于一个局域网
		 //terminalDevices.Add (link.Get (0));//将对应设备添加到终端设备，设备的编号是按这个安装先后顺序编号。
		 //switchDevices.Add (link.Get (1));//添加到网桥
	 }
}

void Switch::init(){

}
