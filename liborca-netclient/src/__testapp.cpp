#include <UdpProto.h>
#include <HermesProto.h>
#include <HellfireProto.h>

using namespace Orca::NetClient;

int main(int argc, char** argv){
	
	
	UdpProto udp = UdpProto(5000);
	HermesProto hermes = HermesProto(udp);
	HellfireProto hf = HellfireProto(hermes);
	
	
	while(1){
		
		char[] message = "acdefghi";
		
		hf->Send(message, 8, "127.0.0.1", 2001);
		
	}
 	
	
	return 0;
}