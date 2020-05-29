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
#ifndef _ORCA_NET_CLI_H
#define _ORCA_NET_CLI_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>  //use select() for multiplexing
#include <sys/fcntl.h>   // for non-blocking
#include <iostream>

namespace Orca::NetClient{

/**
 * @brief Enumeration of possible exceptions involving
 * UDP proto. Please note that this is a simple wrapper
 * on existing UDP API, so some exception are to included
 * yet.
 */
enum UdpProtoException{
    SOCKET_OPEN,    //Could not open the socket. Common causes include limit to the
                    //number of sockets (there are too much open sockets), or system
                    //permission (user has no rights to open sockets, file permission).
    SOCKET_BIND,    //Could not bind the socket probably due to an existing socket 
                    //using the same port number.
    SELECT_ERROR,
    TIMEOUT_EXCEEDED
};

/**
 * @brief This class handles an instance of UDP client/server and
 * is a wrapper over systems' UDP API. In this wrapper, we implement
 * a non-blocking UDP server. 
 */
class UdpProto{

private:
    fd_set original_socket;
    fd_set original_stdin;
    fd_set readfds;
    fd_set writefds;

    struct timeval tv;
    int numfd;
    int socket_fd, bytes_read;
    unsigned int address_length;
    struct sockaddr_in server_address , client_address;

public:
    /**
     * @brief Creates a new UDP server that 
     * listen bound to a <port>. 
     * 
     * @param port Port to which the server will listen. Please note that 
     * UDP's port range is 0-65535, and some of these ports may be reserved
     * by other processes.
     */
    UdpProto(int port);

    /**
     * @brief Destroy the Udp Proto object
     */
    ~UdpProto();

    /**
     * @brief Sends a message via UDP. 
     * 
     * @param data A pointer to the data to be sent.
     * @param length The number of bytes to send.
     * @param dest_address Destination IP addres, represented as a string
     * @param dest_port Destination port number.
     * @return uint32_t Zero if data got succefully sent, an error code otherwise
     */
    uint32_t Send(char* data, int length, std::string dest_address, int dest_port);

    /**
     * @brief Receives data via UDP
     * 
     * @param data Pointer to the place where received data will be stored
     * @param length (out) Number of received bytes
     * @param src_address (out) IP address of the sender (data source)
     * @param src_port (out) UDP port of the sender (data source)
     * @return uint32_t Zero if data got sucefully received, error code otherwise
     */
    uint32_t Recv(char* data, int* length, std::string* src_address, int* src_port);
};

} //end of namespace

#endif //_ORCA_NET_CLI_H