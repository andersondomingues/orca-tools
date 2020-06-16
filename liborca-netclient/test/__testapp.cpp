#include <string>
#include <thread>
#include <chrono>

#include <UdpProto.h>
#include <HermesProto.h>
#include <HellfireProto.h>

using namespace Orca::NetClient;

#define WAIT_RATE 1

int main(int argc [[gnu::unused]], char** argv [[gnu::unused]]){

	std::string addr = "127.0.0.1";

 	UdpProto* udp = new UdpProto(9999);
 	HermesProto* hermes = new HermesProto(udp, addr, 8888);	

	int counter = 0;

	uint16_t x, y;
	int size;
	char buffer[2000];

	char message[] = "acdefghi\0";

	while(1){
		
		auto start_time = std::chrono::high_resolution_clock::now();

		//ping
		hermes->Send(0,0,message, sizeof(message));
		printf("test-app: sent \"%s\" (%d)\n", message, counter++);

		//pong 
		int res = 0; 
		while(!res) //emulate blocking behavior
			res = hermes->Recv(&x, &y, buffer, &size);

		printf("test-app: received %s (%d), from x=%d y=%d (%d bytes)\n", 
			&(buffer[4]), counter, x, y, size);

		auto end_time = std::chrono::high_resolution_clock::now();
		auto time = end_time - start_time;

		printf("test-app: RTT of ");
		std::cout << time/std::chrono::milliseconds(1) << " milliseconds" << std::endl;
	}

	return 0;
}