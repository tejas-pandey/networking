#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PACKETSIZE 5242880

typedef struct
{
    int server_sock, client_sock, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

} Transfer;

void server_init(Transfer &transfer, int portno);
void client_init(Transfer &transfer, const char *hostname, int portno);
void server_listen(Transfer &transfer, void (*callback)(int, Transfer &));
void close_connection(Transfer &transfer);
void serialize(const char type, const char *buffer, const int buffer_size, char *packet);
int deserialize_msg(const char *packet);
void deserialize_buffer(const char *packet, char *buffer, int buffer_size);
