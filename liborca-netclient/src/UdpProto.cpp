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
#include <UdpProto.h>
#include <Utils.h>
#include <iostream>

Orca::NetClient::UdpProto::UdpProto(int port){

    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        throw UdpProtoException::SOCKET_OPEN;
    }

    int flags = fcntl(socket_fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(socket_fd, F_SETFL, flags);
    
    //file descriptor set to monitoring for reading
    FD_ZERO(&original_socket);
    FD_ZERO(&readfds);
    
    //file descriptor set to monitoring for writing
    FD_ZERO(&writefds);
    
    //add the receiving socket to the receiving set 
    FD_SET(socket_fd, &original_socket);
    FD_SET(socket_fd, &readfds);
    
    //add the empty set to the writing monitoring list
    FD_SET(0, &writefds);
    
    // since we got s2 second, it's the "greater", so we use that for
    // // the n param in select()
    numfd = socket_fd + 1;
    
    // wait until either socket has data ready to be recv()d (timeout 10.5 secs)
    tv.tv_sec = 1;
    tv.tv_usec = 500000;
    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_address.sin_zero),8);

    address_length = sizeof(struct sockaddr);
    
    if (bind(socket_fd,(struct sockaddr *)&server_address, sizeof(struct sockaddr)) == -1) {
        throw UdpProtoException::SOCKET_BIND;
    } 
}

uint32_t Orca::NetClient::UdpProto::Send(char* data, int length, std::string addr, int port){

    //fill destination information struct
    sockaddr_in destination;
    destination.sin_port = htons(port);
    destination.sin_addr.s_addr = inet_addr(addr.c_str());
    destination.sin_family = AF_INET;

    Orca::NetClient::Utils::HexDump((int8_t*)data, length);

    //send data using sockets
    return sendto(socket_fd, data, length, 0, 
        (struct sockaddr*)&destination, sizeof(struct sockaddr));
}

uint32_t Orca::NetClient::UdpProto::Recv(char* data, int* length, std::string* src_address, int* src_port){
   
        //copy the receiving set to the working variable
        readfds = original_socket;

        //chech whether the receving set has data
        int res = select(numfd, &readfds, &writefds, NULL, &tv);

        switch(res){

            case -1:
                throw UdpProtoException::SELECT_ERROR;
                break;
            case 0:
                throw UdpProtoException::TIMEOUT_EXCEEDED;
                break;
            default:
                if (FD_ISSET(socket_fd, &readfds)) { 
                    
                    FD_CLR(socket_fd, &readfds);
                    bytes_read = recvfrom(socket_fd, data, sizeof(data), 0,
                        (struct sockaddr *)&client_address, &address_length); 

                    *src_address = inet_ntoa(client_address.sin_addr);
                    *src_port = client_address.sin_port;
                    *length = bytes_read;

                    return 1;
                }
        }
        
        return 0;
}

Orca::NetClient::UdpProto::~UdpProto(){
    close(socket_fd);
}