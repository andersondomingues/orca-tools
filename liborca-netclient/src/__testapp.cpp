#include <string>

#include <UdpProto.h>
#include <HermesProto.h>
#include <HellfireProto.h>

using namespace Orca::NetClient;

int main(int argc [[gnu::unused]], char** argv [[gnu::unused]]){
	
	std::string addr = "127.0.0.1";

	UdpProto* udp = new UdpProto(5000);
	HermesProto* hermes = new HermesProto(udp, addr, 8000);
	HellfireProto* hf = new HellfireProto(hermes);
	
	while(1){
		
		char message[] = "acdefghi";
		hf->Send(message, sizeof(message), 0, 0, 0);
	}
 	
	
	return 0;
}