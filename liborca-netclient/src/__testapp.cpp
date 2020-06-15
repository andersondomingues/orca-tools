#include <string>

#include <UdpProto.h>
#include <HermesProto.h>
#include <HellfireProto.h>

using namespace Orca::NetClient;

int main(int argc [[gnu::unused]], char** argv [[gnu::unused]]){

	std::string addr = "127.0.0.1";

 	UdpProto* udp = new UdpProto(5000);
 	HermesProto* hermes = new HermesProto(udp, addr, 8888);	

	int counter = 0;
	while(1){
		
		char message[] = "acdefghi\0";

		hermes->Send(0,0,message, sizeof(message));
		printf("message sent: %d (%s) \n", counter++, message);

		sleep(2);
	}

	return 0;

}