#include <HellfireProto.h>
#include <HermesProto.h>

Orca::NetClient::HellfireProto::HellfireProto(Orca::NetClient::HermesProto* hermes){
	_hermes = hermes;
}

Orca::NetClient::HellfireProto::~HellfireProto(){
	//nothing to do
}

uint32_t Orca::NetClient::HellfireProto::Send(char* data, int size, 
	int target_cpu, int target_port, int target_channel){

	//generate noc address	
	uint32_t x = NOC_COLUMN(target_cpu);
	uint32_t y = NOC_LINE(target_cpu);
		
	//generate header
	char* msg = new char[HELLFIRE_MAX_MSG_BYTES];

	msg[1] = 0x00;  //src_cpu
	msg[2] = 0x00;  

	msg[3] = 0xe8;	//src_port (5000) ----- task
	msg[4] = 0x03;

	msg[5] = (target_port & 0x000000FF);  //target port
	msg[6] = (target_port >> 8) & 0x000000FF;
	
	msg[7] = (size & 0x000000FF); //msg len
	msg[8] = (size & 0x0000FF00) >> 8; //TODO: maybe >> 8
	
	msg[9] = 0x01; //seq number
	msg[10] = 0x00; 
	
	msg[11] = (target_channel & 0x000000FF); //channel
	msg[12] = (target_channel & 0x0000FF00) >> 8;
	
	//calculate the number of packets
	//packet size = 64 flits = 128 bytes
	//header size =  8 flits =  32 bytes
	//payload size= 128-32 bytes = 96 bytes
	//maximum of 96 bytes of payload per packet (56 flits)
	//int payload_size = (PAYLOAD_SIZE * sizeof(uint16_t));
	uint32_t num_packets = size / NOC_PAYLOAD_SIZE_BYTES;
		
	//add one more packet to handle parts of the payload
	//not added in previous packets
	if(size % NOC_PAYLOAD_SIZE_BYTES != 0) 
		num_packets++;
		
	//copy content into package (while remaining bytes)
	for(uint32_t i = 0; i < num_packets; i++){
			
		int offset = i * NOC_PAYLOAD_SIZE_BYTES;
		
		//copy next 102 bytes
		hf_end_data_copy(&(msg[16]), (char*)&(data[offset]), NOC_PAYLOAD_SIZE_BYTES);
		
		//adjust sequence number (zero recvs ok)
		msg[10] = i + 1;
		
		//send message chunk
		//_hermes->Send(x, y, data, NOC_PACKET_SIZE_BYTES - 2);
		_hermes->Send(x, y, data, size + 32);
	}

	return 0; //TODO: implement error messages
}


// Similar function is implemented into hellfire, but for 
// a fix when printing negative values. We define such a 
// function here so that one may compare packets in both
// the sides of communication at the application level.
void hf_hexdump(char* _mem, uint32_t base, uint32_t length){
	
	uint32_t k, l;
	
	//mask is necessary to correct a bug(?) 
	//when printing negative values.
	uint32_t mask = 0x000000FF; 
	int8_t ch;
	
	for(k = 0; k < length; k += 16){
		printf("\n%08x ", base + k);
		for(l = 0; l < 16; l++){
			printf("%02x ", _mem[k + l] & mask );
			if (l == 7) putchar(' ');
		}
		printf(" |");
		for(l = 0; l < 16; l++){
			ch = _mem[k + l];
			if ((ch >= 32) && (ch <= 126))
				putchar(ch);
			else
				putchar('.');
		}
		putchar('|');
	}
}

//treat endianess for the payload
//TODO: maybe we can treat the endiness for the whole packet
void Orca::NetClient::HellfireProto::hf_end_data_copy(char* target, char* source, size_t bytes){
	
	for(uint i = 0; i < bytes; i+=2){
			target[i] = source[i+1];
			target[i+1] = source[i];
	}
}

uint32_t Orca::NetClient::HellfireProto::Recv(char* data,
	int* size, int* source_cpu, int* source_port, int* channel){
	
	//uint16_t seq = 0, packet = 0, packets = 0;
	uint16_t packets = 0, packet = 0;

	//recv from udp
	uint16_t x, y;

	uint32_t res = _hermes->Recv(&x, &y, data, size);

	if(res == 0){
		
		//copy next 102 bytes
		hf_end_data_copy((char*)data, (char*)data, UDP_MAX_MESSAGE_SIZE);

		uint16_t* bbuf = (uint16_t*)data;

		*source_cpu = bbuf[PKT_SOURCE_CPU];
		*source_port = bbuf[PKT_SOURCE_PORT];
		*size = bbuf[PKT_MSG_SIZE];
		
		*channel = bbuf[PKT_CHANNEL];
		
		*source_port = bbuf[PKT_SOURCE_PORT];
		
		std::cout << "size = " << *size << std::endl;
		std::cout << "source_port = " << *source_port << std::endl;
		std::cout << "source_cpu = " << std::hex << *source_cpu << std::endl;
		
		//seq = buf[PKT_SEQ];
		
		hf_hexdump((char*)data, 0, UDP_MAX_MESSAGE_SIZE);
			
		packets = (*size % NOC_PAYLOAD_SIZE_BYTES == 0) 
			? (*size / NOC_PAYLOAD_SIZE_BYTES) 
			: (*size / NOC_PAYLOAD_SIZE_BYTES + 1);

		if(packet > 1){
			printf("Warning: packet fragmentation not implemented yet. Possible loss of data.\n");
		}

		//!!!!! 
		memcpy(data, &(data[16]), *size);
	}

	return res + packets;
}


