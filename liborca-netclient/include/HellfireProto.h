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
#ifndef _HELLFIRE_PROTO_H
#define _HELLFIRE_PROTO_H

#include <cstring>
#include <iostream>

//as defined for orca (see hermes noc documentation)
#define FLIT_SIZE 2

#define HELLFIRE_MAX_MSG_BYTES 128

//message format define in hellfireos
#define NOC_PACKET_SIZE_FLITS 64
#define NOC_PACKET_SIZE_BYTES (NOC_PACKET_SIZE_FLITS * FLIT_SIZE)

#define NOC_HEADER_SIZE_FLITS 8
#define NOC_HEADER_SIZE_BYTES (NOC_HEADER_SIZE_FLITS * FLIT_SIZE)

#define NOC_PAYLOAD_SIZE_FLITS (NOC_PACKET_SIZE_FLITS - NOC_HEADER_SIZE_FLITS)
#define NOC_PAYLOAD_SIZE_BYTES (NOC_PACKET_SIZE_BYTES - NOC_HEADER_SIZE_BYTES)

//header codes, define in noc.h  (hellfireos)
#define PKT_TARGET_CPU		0
#define PKT_PAYLOAD			1
#define PKT_SOURCE_CPU		2
#define PKT_SOURCE_PORT		3
#define PKT_TARGET_PORT		4
#define PKT_MSG_SIZE		5
#define PKT_SEQ				6
#define PKT_CHANNEL			7

#ifndef NOC_WIDTH
#define NOC_WIDTH 1
#endif

//as defined in noc.h (hellfireos)
#define NOC_COLUMN(core_n)	((core_n) % NOC_WIDTH)
#define NOC_LINE(core_n)	((core_n) / NOC_WIDTH)

//messages ALWAYS have the same size
#define UDP_MAX_MESSAGE_SIZE 512

namespace Orca::NetClient{

class HellfireProto{

private:
	Orca::NetClient::HermesProto* _hermes;

public:
	HellfireProto(Orca::NetClient::HermesProto* hermes);
	~HellfireProto();

	uint32_t Send(char* data, int size, int target_cpu, int target_port, int target_channel);
	uint32_t Recv(char* data, int* size, int* src_cpu, int* src_port, int* channel);

}; //end of class

} //end of namespace

#endif // _HELLFIRE_PROTO_H