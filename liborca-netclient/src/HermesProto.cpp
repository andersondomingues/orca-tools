/******************************************************************************
 * This file is part of project ORCA. More information on the project
 * can be found at the following repositories at GitHub's website.
 *
 * http://https://github.com/andersondomingues/orca-sim
 * http://https://github.com/andersondomingues/orca-software
 * http://https://github.com/andersondomingues/orca-mpsoc
 * http://https://github.com/andersondomingues/orca-tools
 *
 * Copyright (C) 2018-2020 Anderson Domingues, <ti.andersondomingues@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 
******************************************************************************/
#include <HermesProto.h>
#include <UdpProto.h>


Orca::NetClient::HermesProto::HermesProto(Orca::NetClient::UdpProto* udp, 
	std::string destination_addr, int destination_port){
	_udp_proto = udp;
	_dest_addr = destination_addr;
	_dest_port = destination_port;
}

Orca::NetClient::HermesProto::~HermesProto(){

}

uint32_t Orca::NetClient::HermesProto::Recv(uint16_t* x, uint16_t* y, char* data, int* size){

	std::string dummy_addr;
	int dummy_port;

	int res = _udp_proto->Recv(data, size, &dummy_addr, &dummy_port);

	if(res){
		*x = ((uint16_t*)data)[0] >> 4;
		*y = ((uint16_t*)data)[0] & 0x00FF;
	}

	return res;
}

uint32_t Orca::NetClient::HermesProto::Send(uint16_t x, uint16_t y, char* data, uint16_t size){

	//buffer has the same size of payload, plus two entries for the header
	uint16_t buffer[size + 2];

	//make target address (assumes fixe 16-bit length)
	buffer[0] = (x << 4) | y;
	buffer[1] = size;

	//copy remaining data
	char* payload = (char*)&(buffer[2]);

	for(int i = 0; i < size; i++)
		payload[i] = data[i];

	//send via udp
	return _udp_proto->Send((char*)buffer, size + 4, _dest_addr, _dest_port);
}