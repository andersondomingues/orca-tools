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
#ifndef _HERMES_PROTO_H
#define _HERMES_PROTO_H

#include <UdpProto.h>

namespace Orca::NetClient{

/**
 * @brief This class implements the Hermes 
 * protocol over UDP/IP. It is a simple 
 * wrap that parses the content of received 
 * UDP packets treating data as if the data 
 * were tranferred through the Hermes network
 * on-chip.
 */
class HermesProto{

private:
    /**
     * @brief An instance for the underlying UDP handler.
     */
    UdpProto* _udp_proto;

    std::string _dest_addr;
    int _dest_port;

public:
    /**
     * @brief Deafult constructor, requires and instance of 
     * UdpProto to work.
     * @param udp An instance of UdpProto, the underlying
     * protocol handler.
     */
    HermesProto(UdpProto* udp, std::string destination_addr, int destination_port);

    /**
     * @brief Destroy the Hermes Proto object.
     */
    ~HermesProto();

    /**
     * @brief Sends data through the virtual network-on-chip
     * 
     * @param x X-axis address of the target processor core
     * @param y Y-axix address of the tatget processor core
     * @param data A pointer to the data to be tranferred
     * @param size The number of bytes to tranfer.
     * @return uint32_t Zero if succeffuly tranferred, otherwise error code
     */
    uint32_t Send(uint16_t x, uint16_t y, char* data, uint16_t size);

    /**
     * @brief Receives data from the virtual network-on-chip
     * 
     * @param x The X-axis address of the processor core which sent the message
     * @param y The Y-axis address of the processor core which sent the message
     * @param data A pointer to the place where the message will be stored
     * @param size The number of received bytes
     * @return uint32_t Zero if received succefully, error code otherwise
     */
    uint32_t Recv(uint16_t* x, uint16_t* y, char* data, int* size);
};	
	
} // end of namespace

#endif // _HERMES_PROTO_H